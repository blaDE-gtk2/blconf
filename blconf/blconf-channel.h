/*
 *  blconf
 *
 *  Copyright (c) 2007-2008 Brian Tarricone <bjt23@cornell.edu>
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

#ifndef __BLCONF_CHANNEL_H__
#define __BLCONF_CHANNEL_H__

#if !defined(LIBBLCONF_COMPILATION) && !defined(BLCONF_IN_BLCONF_H)
#error "Do not include blconf-channel.h, as this file may change or disappear in the future.  Include <blconf/blconf.h> instead."
#endif

#include <glib-object.h>

#define BLCONF_TYPE_CHANNEL             (blconf_channel_get_type())
#define BLCONF_CHANNEL(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), BLCONF_TYPE_CHANNEL, BlconfChannel))
#define BLCONF_IS_CHANNEL(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), BLCONF_TYPE_CHANNEL))
#define BLCONF_CHANNEL_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), BLCONF_TYPE_CHANNEL, BlconfChannelClass))
#define BLCONF_IS_CHANNEL_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), BLCONF_TYPE_CHANNEL))
#define BLCONF_CHANNEL_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), BLCONF_TYPE_CHANNEL, BlconfChannelClass))

G_BEGIN_DECLS

typedef struct _BlconfChannel         BlconfChannel;

GType blconf_channel_get_type(void) G_GNUC_CONST;

BlconfChannel *blconf_channel_get(const gchar *channel_name);

BlconfChannel *blconf_channel_new(const gchar *channel_name) G_GNUC_WARN_UNUSED_RESULT;

BlconfChannel *blconf_channel_new_with_property_base(const gchar *channel_name,
                                                     const gchar *property_base) G_GNUC_WARN_UNUSED_RESULT;

gboolean blconf_channel_has_property(BlconfChannel *channel,
                                     const gchar *property);

gboolean blconf_channel_is_property_locked(BlconfChannel *channel,
                                           const gchar *property);

void blconf_channel_reset_property(BlconfChannel *channel,
                                   const gchar *property_base,
                                   gboolean recursive);

GHashTable *blconf_channel_get_properties(BlconfChannel *channel,
                                          const gchar *property_base) G_GNUC_WARN_UNUSED_RESULT;

/* basic types */

gchar *blconf_channel_get_string(BlconfChannel *channel,
                                 const gchar *property,
                                 const gchar *default_value) G_GNUC_WARN_UNUSED_RESULT;
gboolean blconf_channel_set_string(BlconfChannel *channel,
                                   const gchar *property,
                                   const gchar *value);

gint32 blconf_channel_get_int(BlconfChannel *channel,
                              const gchar *property,
                              gint32 default_value);
gboolean blconf_channel_set_int(BlconfChannel *channel,
                                const gchar *property,
                                gint32 value);

guint32 blconf_channel_get_uint(BlconfChannel *channel,
                                const gchar *property,
                                guint32 default_value);
gboolean blconf_channel_set_uint(BlconfChannel *channel,
                                 const gchar *property,
                                 guint32 value);

guint64 blconf_channel_get_uint64(BlconfChannel *channel,
                                  const gchar *property,
                                  guint64 default_value);
gboolean blconf_channel_set_uint64(BlconfChannel *channel,
                                   const gchar *property,
                                   guint64 value);

gdouble blconf_channel_get_double(BlconfChannel *channel,
                                  const gchar *property,
                                  gdouble default_value);
gboolean blconf_channel_set_double(BlconfChannel *channel,
                                   const gchar *property,
                                   gdouble value);

gboolean blconf_channel_get_bool(BlconfChannel *channel,
                                 const gchar *property,
                                 gboolean default_value);
gboolean blconf_channel_set_bool(BlconfChannel *channel,
                                 const gchar *property,
                                 gboolean value);

/* this is just convenience API for the array stuff, where
 * all the values are G_TYPE_STRING */
gchar **blconf_channel_get_string_list(BlconfChannel *channel,
                                       const gchar *property) G_GNUC_WARN_UNUSED_RESULT;
gboolean blconf_channel_set_string_list(BlconfChannel *channel,
                                        const gchar *property,
                                        const gchar * const *values);

/* really generic API - can set some value types that aren't
 * supported by the basic type API, e.g., char, signed short,
 * unsigned int, etc.  no, you can't set arbitrary GTypes. */
gboolean blconf_channel_get_property(BlconfChannel *channel,
                                     const gchar *property,
                                     GValue *value);
gboolean blconf_channel_set_property(BlconfChannel *channel,
                                     const gchar *property,
                                     const GValue *value);

/* array types - arrays can be made up of values of arbitrary
 * (and mixed) types, even some not supported by the basic
 * type API */

gboolean blconf_channel_get_array(BlconfChannel *channel,
                                  const gchar *property,
                                  GType first_value_type,
                                  ...);
gboolean blconf_channel_get_array_valist(BlconfChannel *channel,
                                         const gchar *property,
                                         GType first_value_type,
                                         va_list var_args);
GPtrArray *blconf_channel_get_arrayv(BlconfChannel *channel,
                                     const gchar *property) G_GNUC_WARN_UNUSED_RESULT;

gboolean blconf_channel_set_array(BlconfChannel *channel,
                                  const gchar *property,
                                  GType first_value_type,
                                  ...);
gboolean blconf_channel_set_array_valist(BlconfChannel *channel,
                                         const gchar *property,
                                         GType first_value_type,
                                         va_list var_args);
gboolean blconf_channel_set_arrayv(BlconfChannel *channel,
                                   const gchar *property,
                                   GPtrArray *values);

/* struct types */

gboolean blconf_channel_get_named_struct(BlconfChannel *channel,
                                         const gchar *property,
                                         const gchar *struct_name,
                                         gpointer value_struct);
gboolean blconf_channel_set_named_struct(BlconfChannel *channel,
                                         const gchar *property,
                                         const gchar *struct_name,
                                         gpointer value_struct);

gboolean blconf_channel_get_struct(BlconfChannel *channel,
                                   const gchar *property,
                                   gpointer value_struct,
                                   GType first_member_type,
                                   ...);
gboolean blconf_channel_get_struct_valist(BlconfChannel *channel,
                                          const gchar *property,
                                          gpointer value_struct,
                                          GType first_member_type,
                                          va_list var_args);
gboolean blconf_channel_get_structv(BlconfChannel *channel,
                                    const gchar *property,
                                    gpointer value_struct,
                                    guint n_members,
                                    GType *member_types);

gboolean blconf_channel_set_struct(BlconfChannel *channel,
                                   const gchar *property,
                                   const gpointer value_struct,
                                   GType first_member_type,
                                   ...);
gboolean blconf_channel_set_struct_valist(BlconfChannel *channel,
                                          const gchar *property,
                                          const gpointer value_struct,
                                          GType first_member_type,
                                          va_list var_args);
gboolean blconf_channel_set_structv(BlconfChannel *channel,
                                    const gchar *property,
                                    const gpointer value_struct,
                                    guint n_members,
                                    GType *member_types);

#if 0  /* future (maybe) */

//gboolean blconf_channel_begin_transaction(BlconfChannel *channel);
//gboolean blconf_channel_commit_transaction(BlconfChannel *channel);
//void blconf_channel_cancel_transaction(BlconfChannel *channel);

#endif

G_END_DECLS

#endif  /* __BLCONF_CHANNEL_H__ */
