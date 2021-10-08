/*
 *  blconf
 *
 *  Copyright (c) 2007 Brian Tarricone <bjt23@cornell.edu>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; version 2
 *  of the License ONLY.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include <glib-object.h>

#include <dbus/dbus-glib.h>

#include "blconf.h"
#include "common/blconf-marshal.h"
#include "blconf-private.h"
#include "common/blconf-alias.h"

static guint blconf_refcnt = 0;
static DBusGConnection *dbus_conn = NULL;
static DBusGProxy *dbus_proxy = NULL;
static GHashTable *named_structs = NULL;


/* private api */

DBusGConnection *
_blconf_get_dbus_g_connection(void)
{
    if(!blconf_refcnt) {
        g_critical("blconf_init() must be called before attempting to use libblconf!");
        return NULL;
    }

    return dbus_conn;
}

DBusGProxy *
_blconf_get_dbus_g_proxy(void)
{
    if(!blconf_refcnt) {
        g_critical("blconf_init() must be called before attempting to use libblconf!");
        return NULL;
    }

    return dbus_proxy;
}

BlconfNamedStruct *
_blconf_named_struct_lookup(const gchar *struct_name)
{
    return named_structs ? g_hash_table_lookup(named_structs, struct_name) : NULL;
}

static void
_blconf_named_struct_free(BlconfNamedStruct *ns)
{
    g_free(ns->member_types);
    g_slice_free(BlconfNamedStruct, ns);
}



static void
blconf_static_dbus_init(void)
{
    static gboolean static_dbus_inited = FALSE;

    if(!static_dbus_inited) {
        dbus_g_error_domain_register(BLCONF_ERROR, "org.blade.Blconf.Error",
                                     BLCONF_TYPE_ERROR);

        dbus_g_object_register_marshaller(_blconf_marshal_VOID__STRING_STRING_BOXED,
                                          G_TYPE_NONE,
                                          G_TYPE_STRING,
                                          G_TYPE_STRING,
                                          G_TYPE_VALUE,
                                          G_TYPE_INVALID);
        dbus_g_object_register_marshaller(_blconf_marshal_VOID__STRING_STRING,
                                          G_TYPE_NONE,
                                          G_TYPE_STRING,
                                          G_TYPE_STRING,
                                          G_TYPE_INVALID);

        static_dbus_inited = TRUE;
    }
}



/* public api */

/**
 * blconf_init:
 * @error: An error return.
 *
 * Initializes the Blconf library.  Can be called multiple times with no
 * adverse effects.
 *
 * Returns: %TRUE if the library was initialized succesfully, %FALSE on
 *          error.  If there is an error @error will be set.
 **/
gboolean
blconf_init(GError **error)
{
    if(blconf_refcnt) {
        ++blconf_refcnt;
        return TRUE;
    }

#if !GLIB_CHECK_VERSION(2,36,0)
    g_type_init();
#endif

    blconf_static_dbus_init();

    dbus_conn = dbus_g_bus_get(DBUS_BUS_SESSION, error);
    if(!dbus_conn)
        return FALSE;

    dbus_proxy = dbus_g_proxy_new_for_name(dbus_conn,
                                           "org.blade.Blconf",
                                           "/org/blade/Blconf",
                                           "org.blade.Blconf");

    dbus_g_proxy_add_signal(dbus_proxy, "PropertyChanged",
                            G_TYPE_STRING, G_TYPE_STRING, G_TYPE_VALUE,
                            G_TYPE_INVALID);
    dbus_g_proxy_add_signal(dbus_proxy, "PropertyRemoved",
                            G_TYPE_STRING, G_TYPE_STRING,
                            G_TYPE_INVALID);

    ++blconf_refcnt;
    return TRUE;
}

/**
 * blconf_shutdown:
 *
 * Shuts down and frees any resources consumed by the Blconf library.
 * If blconf_init() is called multiple times, blconf_shutdown() must be
 * called an equal number of times to shut down the library.
 **/
void
blconf_shutdown(void)
{
    if(blconf_refcnt <= 0) {
        return;
    }

    if(blconf_refcnt > 1) {
        --blconf_refcnt;
        return;
    }

    _blconf_channel_shutdown();
    _blconf_g_bindings_shutdown();

    if(named_structs) {
        g_hash_table_destroy(named_structs);
        named_structs = NULL;
    }

    g_object_unref(G_OBJECT(dbus_proxy));
    dbus_proxy = NULL;

    dbus_g_connection_unref(dbus_conn);
    dbus_conn = NULL;

    --blconf_refcnt;
}

/**
 * blconf_named_struct_register:
 * @struct_name: The unique name of the struct to register.
 * @n_members: The number of data members in the struct.
 * @member_types: An array of the #GType<!-- -->s of the struct members.
 *
 * Registers a named struct for use with blconf_channel_get_named_struct()
 * and blconf_channel_set_named_struct().
 **/
void
blconf_named_struct_register(const gchar *struct_name,
                             guint n_members,
                             const GType *member_types)
{
    BlconfNamedStruct *ns;

    g_return_if_fail(struct_name && *struct_name && n_members && member_types);

    /* lazy initialize the hash table */
    if(named_structs == NULL)
        named_structs = g_hash_table_new_full(g_str_hash, g_str_equal,
                                              (GDestroyNotify)g_free,
                                              (GDestroyNotify)_blconf_named_struct_free);

    if(G_UNLIKELY(g_hash_table_lookup(named_structs, struct_name)))
        g_critical("The struct '%s' is already registered", struct_name);
    else {
        ns = g_slice_new(BlconfNamedStruct);
        ns->n_members = n_members;
        ns->member_types = g_new(GType, n_members);
        memcpy(ns->member_types, member_types, sizeof(GType) * n_members);

        g_hash_table_insert(named_structs, g_strdup(struct_name), ns);
    }
}

#if 0
/**
 * blconf_array_new:
 * @n_preallocs: Number of entries to preallocate space for.
 *
 * Convenience function to greate a new #GArray to hold
 * #GValue<!-- -->s.  Normal #GArray functions may be used on
 * the returned array.  For convenience, see also blconf_array_free().
 *
 * Returns: A new #GArray.
 **/
GArray *
blconf_array_new(gint n_preallocs)
{
    return g_array_sized_new(FALSE, TRUE, sizeof(GValue), n_preallocs);
}
#endif

/**
 * blconf_array_free:
 * @arr: A #GPtrArray of #GValue<!-- -->s.
 *
 * Properly frees a #GPtrArray structure containing a list of
 * #GValue<!-- -->s.  This will also cause the contents of the
 * values to be freed as well.
 **/
void
blconf_array_free(GPtrArray *arr)
{
    guint i;
    
    if(!arr)
        return;
    
    for(i = 0; i < arr->len; ++i) {
        GValue *val = g_ptr_array_index(arr, i);
        g_value_unset(val);
        g_free(val);
    }
    
    g_ptr_array_free(arr, TRUE);
}



#define __BLCONF_C__
#include "common/blconf-aliasdef.c"
