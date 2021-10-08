/*
 *  blconfd
 *
 *  Copyright (c) 2007 Brian Tarricone <bjt23@cornell.edu>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License ONLY.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>

#include <dbus/dbus-glib-lowlevel.h>
#include <libbladeutil/libbladeutil.h>

#include "blconf-daemon.h"
#include "blconf-backend-factory.h"
#include "blconf-backend.h"
#include "common/blconf-marshal.h"
#include "common/blconf-gvaluefuncs.h"
#include "blconf/blconf-errors.h"
#include "common/blconf-common-private.h"

static void blconf_set_property(BlconfDaemon *blconfd,
                                const gchar *channel,
                                const gchar *property,
                                const GValue *value,
                                DBusGMethodInvocation *context);
static void blconf_get_property(BlconfDaemon *blconfd,
                                const gchar *channel,
                                const gchar *property,
                                DBusGMethodInvocation *context);
static void blconf_get_all_properties(BlconfDaemon *blconfd,
                                      const gchar *channel,
                                      const gchar *property_base,
                                      DBusGMethodInvocation *context);
static void blconf_property_exists(BlconfDaemon *blconfd,
                                   const gchar *channel,
                                   const gchar *property,
                                   DBusGMethodInvocation *context);
static void blconf_reset_property(BlconfDaemon *blconfd,
                                  const gchar *channel,
                                  const gchar *property,
                                  gboolean recursive,
                                  DBusGMethodInvocation *context);
static void blconf_list_channels(BlconfDaemon *blconfd,
                                 DBusGMethodInvocation *context);
static void blconf_is_property_locked(BlconfDaemon *blconfd,
                                      const gchar *channel,
                                      const gchar *property,
                                      DBusGMethodInvocation *context);

#include "blconf-dbus-server.h"

struct _BlconfDaemon
{
    GObject parent;

    DBusGConnection *dbus_conn;

    GList *backends;
};

typedef struct _BlconfDaemonClass
{
    GObjectClass parent;
} BlconfDaemonClass;

enum
{
    SIG_PROPERTY_CHANGED = 0,
    SIG_PROPERTY_REMOVED,
    N_SIGS,
};

static void blconf_daemon_finalize(GObject *obj);

static DBusHandlerResult blconf_daemon_handle_dbus_disconnect(DBusConnection *conn,
                                                              DBusMessage *message,
                                                              void *user_data);

static guint signals[N_SIGS] = { 0, };


G_DEFINE_TYPE(BlconfDaemon, blconf_daemon, G_TYPE_OBJECT)


static void
blconf_daemon_class_init(BlconfDaemonClass *klass)
{
    GObjectClass *object_class = (GObjectClass *)klass;

    object_class->finalize = blconf_daemon_finalize;

    signals[SIG_PROPERTY_CHANGED] = g_signal_new(I_("property-changed"),
                                                 BLCONF_TYPE_DAEMON,
                                                 G_SIGNAL_RUN_LAST,
                                                 0,
                                                 NULL, NULL,
                                                 _blconf_marshal_VOID__STRING_STRING_BOXED,
                                                 G_TYPE_NONE,
                                                 3, G_TYPE_STRING,
                                                 G_TYPE_STRING,
                                                 G_TYPE_VALUE);

    signals[SIG_PROPERTY_REMOVED] = g_signal_new(I_("property-removed"),
                                                 BLCONF_TYPE_DAEMON,
                                                 G_SIGNAL_RUN_LAST,
                                                 0,
                                                 NULL, NULL,
                                                 _blconf_marshal_VOID__STRING_STRING,
                                                 G_TYPE_NONE,
                                                 2, G_TYPE_STRING,
                                                 G_TYPE_STRING);

    dbus_g_object_type_install_info(G_TYPE_FROM_CLASS(klass),
                                    &dbus_glib_blconf_object_info);
    dbus_g_error_domain_register(BLCONF_ERROR, "org.blade.Blconf.Error",
                                 BLCONF_TYPE_ERROR);
}

static void
blconf_daemon_init(BlconfDaemon *instance)
{

}

static void
blconf_daemon_finalize(GObject *obj)
{
    BlconfDaemon *blconfd = BLCONF_DAEMON(obj);
    GList *l;

    for(l = blconfd->backends; l; l = l->next) {
        blconf_backend_register_property_changed_func(l->data, NULL, NULL);
        blconf_backend_flush(l->data, NULL);
        g_object_unref(l->data);
    }
    g_list_free(blconfd->backends);

    if(blconfd->dbus_conn) {
        dbus_connection_remove_filter(dbus_g_connection_get_connection(blconfd->dbus_conn),
                                      blconf_daemon_handle_dbus_disconnect,
                                      blconfd);
        dbus_g_connection_unref(blconfd->dbus_conn);
    }

    G_OBJECT_CLASS(blconf_daemon_parent_class)->finalize(obj);
}

typedef struct
{
    BlconfDaemon *blconfd;
    BlconfBackend *backend;
    gchar *channel;
    gchar *property;
} BlconfPropChangedData;

static gboolean
blconf_daemon_emit_property_changed_idled(gpointer data)
{
    BlconfPropChangedData *pdata = data;
    GValue value = { 0, };

    blconf_backend_get(pdata->backend, pdata->channel, pdata->property,
                       &value, NULL);

    if(G_VALUE_TYPE(&value)) {
        g_signal_emit(G_OBJECT(pdata->blconfd), signals[SIG_PROPERTY_CHANGED],
                      0, pdata->channel, pdata->property, &value);
        g_value_unset(&value);
    } else {
        g_signal_emit(G_OBJECT(pdata->blconfd), signals[SIG_PROPERTY_REMOVED],
                      0, pdata->channel, pdata->property);
    }

    g_object_unref(G_OBJECT(pdata->backend));
    g_free(pdata->channel);
    g_free(pdata->property);
    g_object_unref(G_OBJECT(pdata->blconfd));
    g_slice_free(BlconfPropChangedData, pdata);

    return FALSE;
}

static void
blconf_daemon_backend_property_changed(BlconfBackend *backend,
                                       const gchar *channel,
                                       const gchar *property,
                                       gpointer user_data)
{
    BlconfPropChangedData *pdata = g_slice_new0(BlconfPropChangedData);

    pdata->blconfd = g_object_ref(G_OBJECT(user_data));
    pdata->backend = g_object_ref(G_OBJECT(backend));
    pdata->channel = g_strdup(channel);
    pdata->property = g_strdup(property);

    g_idle_add(blconf_daemon_emit_property_changed_idled, pdata);
}

static void
blconf_set_property(BlconfDaemon *blconfd,
                    const gchar *channel,
                    const gchar *property,
                    const GValue *value,
                    DBusGMethodInvocation *context)
{
    GList *l;
    GError *error = NULL;

    /* if there's more than one backend, we need to make sure the
     * property isn't locked on ANY of them */
    if(G_UNLIKELY(blconfd->backends->next)) {
        for(l = blconfd->backends; l; l = l->next) {
            gboolean locked = FALSE;

            if(!blconf_backend_is_property_locked(l->data, channel, property,
                                                  &locked, &error))
                break;

            if(locked) {
                g_set_error(&error, BLCONF_ERROR,
                            BLCONF_ERROR_PERMISSION_DENIED,
                            _("Permission denied while modifying property \"%s\" on channel \"%s\""),
                            property, channel);
                break;
            }
        }

        /* there is always an error set if something failed or the
         * property is locked */
        if(error) {
            dbus_g_method_return_error(context, error);
            g_error_free(error);
            return;
        }
    }

    /* only write to first backend */
    if(blconf_backend_set(blconfd->backends->data, channel, property,
                          value, &error))
    {
        dbus_g_method_return(context);
    } else {
        dbus_g_method_return_error(context, error);
        g_error_free(error);
    }
}

static void
blconf_get_property(BlconfDaemon *blconfd,
                    const gchar *channel,
                    const gchar *property,
                    DBusGMethodInvocation *context)
{
    GList *l;
    GValue value = { 0, };
    GError *error = NULL;

    /* check each backend until we find a value */
    for(l = blconfd->backends; l; l = l->next) {
        if(blconf_backend_get(l->data, channel, property, &value, &error)) {
            dbus_g_method_return (context, &value);
            g_value_unset(&value);
            return;
        } else if(l->next)
            g_clear_error(&error);
    }

    dbus_g_method_return_error(context, error);
    g_error_free(error);
}

static void
blconf_get_all_properties(BlconfDaemon *blconfd,
                          const gchar *channel,
                          const gchar *property_base,
                          DBusGMethodInvocation *context)
{
    GList *l;
    GHashTable *properties;
    GError *error = NULL;
    gboolean succeed = FALSE;

    properties = g_hash_table_new_full(g_str_hash, g_str_equal,
                                        (GDestroyNotify)g_free,
                                        (GDestroyNotify)_blconf_gvalue_free);

    /* get all properties from all backends.  if they all fail, return FALSE */
    for(l = blconfd->backends; l; l = l->next) {
        if(blconf_backend_get_all(l->data, channel, property_base,
                                  properties, &error))
            succeed = TRUE;
        else if(l->next) {
            g_clear_error(&error);
        }
    }

    if(succeed)
        dbus_g_method_return (context, properties);
    else
        dbus_g_method_return_error(context, error);

    if(error)
        g_error_free(error);
    g_hash_table_destroy(properties);
}

static void
blconf_property_exists(BlconfDaemon *blconfd,
                       const gchar *channel,
                       const gchar *property,
                       DBusGMethodInvocation *context)
{
    gboolean exists = FALSE;
    gboolean succeed = FALSE;
    GList *l;
    GError *error = NULL;

    /* if at least one backend returns TRUE (regardles if |*exists| gets set
     * to TRUE or FALSE), we'll return TRUE from this function */

    for(l = blconfd->backends; !exists && l; l = l->next) {
        if(blconf_backend_exists(l->data, channel, property, &exists, &error))
            succeed = TRUE;
        else if(l->next)
            g_clear_error(&error);
    }

    if(succeed)
        dbus_g_method_return(context, exists);
    else {
        dbus_g_method_return_error(context, error);
        g_error_free(error);
    }
}

static void
blconf_reset_property(BlconfDaemon *blconfd,
                      const gchar *channel,
                      const gchar *property,
                      gboolean recursive,
                      DBusGMethodInvocation *context)
{
    gboolean succeed = FALSE;
    GList *l;
    GError *error = NULL;

    /* while technically all backends but the first should be opened read-only,
     * we need to reset in all backends so the property doesn't reappear
     * later */

    for(l = blconfd->backends; l; l = l->next) {
        if(blconf_backend_reset(l->data, channel, property, recursive, &error))
            succeed = TRUE;
        else if(l->next)
            g_clear_error(&error);
    }

    if(succeed)
        dbus_g_method_return(context);
    else
        dbus_g_method_return_error(context, error);

    if(error)
        g_error_free(error);
}

static void
blconf_list_channels(BlconfDaemon *blconfd,
                     DBusGMethodInvocation *context)
{
    GSList *lchannels = NULL, *chans_tmp, *lc;
    GList *l;
    guint i;
    gchar **channels;
    GError *error = NULL;

    /* FIXME: with multiple backends, this can cause duplicates */
    for(l = blconfd->backends; l; l = l->next) {
        chans_tmp = NULL;
        if(blconf_backend_list_channels(l->data, &chans_tmp, &error))
            lchannels = g_slist_concat(lchannels, chans_tmp);
        else if(l->next)
            g_clear_error(&error);
    }

    if(error && !lchannels) {
        /* no channels and an error, something went wrong */
        dbus_g_method_return_error(context, error);
    } else {
        channels = g_new (gchar *, g_slist_length(lchannels) + 1);
        for(lc = lchannels, i = 0; lc; lc = lc->next, ++i)
            channels[i] = lc->data;
        channels[i] = NULL;

        dbus_g_method_return(context, channels);

        g_strfreev(channels);
        g_slist_free(lchannels);
    }

    if(error)
        g_error_free(error);
}

static void blconf_is_property_locked(BlconfDaemon *blconfd,
                          const gchar *channel,
                          const gchar *property,
                          DBusGMethodInvocation *context)
{
    GList *l;
    gboolean locked = FALSE;
    GError *error = NULL;
    gboolean succeed = FALSE;

    for(l = blconfd->backends; !locked && l; l = l->next) {
        if(blconf_backend_is_property_locked(l->data, channel, property,
                                             &locked, &error))
            succeed = TRUE;
        else if(l->next)
            g_clear_error(&error);
    }

    if(succeed)
        dbus_g_method_return(context, locked);
    else
        dbus_g_method_return_error(context, error);

    if(error)
        g_error_free(error);
}




static gboolean
blconf_daemon_start(BlconfDaemon *blconfd,
                    GError **error)
{
    int ret;
    DBusError derror;

    blconfd->dbus_conn = dbus_g_bus_get(DBUS_BUS_SESSION, error);
    if(G_UNLIKELY(!blconfd->dbus_conn))
        return FALSE;

    dbus_g_connection_register_g_object(blconfd->dbus_conn,
                                        "/org/blade/Blconf",
                                        G_OBJECT(blconfd));

    dbus_connection_add_filter(dbus_g_connection_get_connection(blconfd->dbus_conn),
                               blconf_daemon_handle_dbus_disconnect,
                               blconfd, NULL);

    dbus_error_init(&derror);
    ret = dbus_bus_request_name(dbus_g_connection_get_connection(blconfd->dbus_conn),
                                "org.blade.Blconf",
                                DBUS_NAME_FLAG_DO_NOT_QUEUE,
                                &derror);
    if(DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) {
        if(dbus_error_is_set(&derror)) {
            if(error)
                dbus_set_g_error(error, &derror);
            dbus_error_free(&derror);
        } else if(error) {
            g_set_error(error, DBUS_GERROR, DBUS_GERROR_FAILED,
                        _("Another Blconf daemon is already running"));
        }

        return FALSE;
    }

    return TRUE;
}

static gboolean
blconf_daemon_load_config(BlconfDaemon *blconfd,
                          gchar * const *backend_ids,
                          GError **error)
{
    gint i;

    for(i = 0; backend_ids[i]; ++i) {
        GError *error1 = NULL;
        BlconfBackend *backend = blconf_backend_factory_get_backend(backend_ids[i],
                                                                    &error1);
        if(!backend) {
            g_warning("Unable to start backend \"%s\": %s", backend_ids[i],
                      error1->message);
            g_clear_error(&error1);
        } else {
            blconfd->backends = g_list_prepend(blconfd->backends, backend);
            blconf_backend_register_property_changed_func(backend,
                                                          blconf_daemon_backend_property_changed,
                                                          blconfd);
        }
    }

    if(!blconfd->backends) {
        if(error) {
            g_set_error(error, BLCONF_ERROR, BLCONF_ERROR_NO_BACKEND,
                        _("No backends could be started"));
        }
        return FALSE;
    }

    blconfd->backends = g_list_reverse(blconfd->backends);

    return TRUE;
}

static DBusHandlerResult
blconf_daemon_handle_dbus_disconnect(DBusConnection *conn,
                                     DBusMessage *message,
                                     void *user_data)
{
    if(dbus_message_is_signal(message, DBUS_INTERFACE_LOCAL, "Disconnected")) {
        BlconfDaemon *blconfd = user_data;
        GList *l;

        DBG("got dbus disconnect; flushing all channels");

        for(l = blconfd->backends; l; l = l->next) {
            GError *error = NULL;
            if(!blconf_backend_flush(BLCONF_BACKEND(l->data), &error)) {
                g_critical("Failed to flush backend on disconnect: %s",
                           error->message);
                g_error_free(error);
            }
        }
    }

    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}




BlconfDaemon *
blconf_daemon_new_unique(gchar * const *backend_ids,
                         GError **error)
{
    BlconfDaemon *blconfd;

    g_return_val_if_fail(backend_ids && backend_ids[0], NULL);

    blconfd = g_object_new(BLCONF_TYPE_DAEMON, NULL);

    if(!blconf_daemon_start(blconfd, error)
       || !blconf_daemon_load_config(blconfd, backend_ids, error))
    {
        g_object_unref(G_OBJECT(blconfd));
        return NULL;
    }

    return blconfd;
}
