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

#ifndef __BLCONF_BACKEND_H__
#define __BLCONF_BACKEND_H__

#include <glib-object.h>

#if defined(GETTEXT_PACKAGE)
#include <glib/gi18n-lib.h>
#else
#include <glib/gi18n.h>
#endif

#include <blconf/blconf-errors.h>

#define BLCONF_TYPE_BACKEND                (blconf_backend_get_type())
#define BLCONF_BACKEND(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), BLCONF_TYPE_BACKEND, BlconfBackend))
#define BLCONF_IS_BACKEND(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), BLCONF_TYPE_BACKEND))
#define BLCONF_BACKEND_GET_INTERFACE(obj)  (G_TYPE_INSTANCE_GET_INTERFACE((obj), BLCONF_TYPE_BACKEND, BlconfBackendInterface))

#define blconf_backend_return_val_if_fail(cond, val)  G_STMT_START{ \
    if(!(cond)) { \
        if(error) { \
            g_set_error(error, BLCONF_ERROR, \
                        BLCONF_ERROR_INTERNAL_ERROR, \
                        _("An internal error occurred; this is probably a bug")); \
        } \
        g_return_val_if_fail((cond), (val)); \
        return (val);  /* ensure return even if G_DISABLE_CHECKS */ \
    } \
}G_STMT_END

G_BEGIN_DECLS

typedef struct _BlconfBackend           BlconfBackend;
typedef struct _BlconfBackendInterface  BlconfBackendInterface;

typedef void (*BlconfPropertyChangedFunc)(BlconfBackend *backend,
                                          const gchar *channel,
                                          const gchar *property,
                                          gpointer user_data);

struct _BlconfBackendInterface
{
    GTypeInterface parent;
    
    gboolean (*initialize)(BlconfBackend *backend,
                           GError **error);
    
    gboolean (*set)(BlconfBackend *backend,
                    const gchar *channel,
                    const gchar *property,
                    const GValue *value,
                    GError **error);
    
    gboolean (*get)(BlconfBackend *backend,
                    const gchar *channel,
                    const gchar *property,
                    GValue *value,
                    GError **error);
    
    gboolean (*get_all)(BlconfBackend *backend,
                        const gchar *channel,
                        const gchar *property_base,
                        GHashTable *properties,
                        GError **error);
    
    gboolean (*exists)(BlconfBackend *backend,
                       const gchar *channel,
                       const gchar *property,
                       gboolean *exists,
                       GError **error);
    
    gboolean (*reset)(BlconfBackend *backend,
                      const gchar *channel,
                      const gchar *property,
                      gboolean recursive,
                      GError **error);

    gboolean (*list_channels)(BlconfBackend *backend,
                              GSList **channels,
                              GError **error);

    gboolean (*is_property_locked)(BlconfBackend *backend,
                                   const gchar *channel,
                                   const gchar *property,
                                   gboolean *locked,
                                   GError **error);
    
    gboolean (*flush)(BlconfBackend *backend,
                      GError **error);

    void (*register_property_changed_func)(BlconfBackend *backend,
                                           BlconfPropertyChangedFunc func,
                                           gpointer user_data);
    
    /*< reserved for future expansion >*/
    void (*_xb_reserved0)();
    void (*_xb_reserved1)();
    void (*_xb_reserved2)();
    void (*_xb_reserved3)();
};

GType blconf_backend_get_type(void) G_GNUC_CONST;

gboolean blconf_backend_initialize(BlconfBackend *backend,
                                   GError **error);

gboolean blconf_backend_set(BlconfBackend *backend,
                            const gchar *channel,
                            const gchar *property,
                            const GValue *value,
                            GError **error);

gboolean blconf_backend_get(BlconfBackend *backend,
                            const gchar *channel,
                            const gchar *property,
                            GValue *value,
                            GError **error);

gboolean blconf_backend_get_all(BlconfBackend *backend,
                                const gchar *channel,
                                const gchar *property_base,
                                GHashTable *properties,
                                GError **error);

gboolean blconf_backend_exists(BlconfBackend *backend,
                               const gchar *channel,
                               const gchar *property,
                               gboolean *exists,
                               GError **error);

gboolean blconf_backend_reset(BlconfBackend *backend,
                              const gchar *channel,
                              const gchar *property,
                              gboolean recursive,
                              GError **error);

gboolean blconf_backend_list_channels(BlconfBackend *backend,
                                      GSList **channels,
                                      GError **error);

gboolean blconf_backend_is_property_locked(BlconfBackend *backend,
                                           const gchar *channel,
                                           const gchar *property,
                                           gboolean *locked,
                                           GError **error);

gboolean blconf_backend_flush(BlconfBackend *backend,
                              GError **error);

void blconf_backend_register_property_changed_func(BlconfBackend *backend,
                                                   BlconfPropertyChangedFunc func,
                                                   gpointer user_data);

G_END_DECLS

#endif  /* __BLCONF_BACKEND_H__ */
