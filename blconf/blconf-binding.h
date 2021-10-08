/*
 *  blconf
 *
 *  Copyright (c) 2008 Brian Tarricone <bjt23@cornell.edu>
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

#ifndef __BLCONF_BINDING_H__
#define __BLCONF_BINDING_H__

#if !defined(LIBBLCONF_COMPILATION) && !defined(BLCONF_IN_BLCONF_H)
#error "Do not include blconf-binding.h, as this file may change or disappear in the future.  Include <blconf/blconf.h> instead."
#endif

#include <glib-object.h>
#include <blconf/blconf-channel.h>

G_BEGIN_DECLS

gulong blconf_g_property_bind(BlconfChannel *channel,
                              const gchar *blconf_property,
                              GType blconf_property_type,
                              gpointer object,
                              const gchar *object_property);

gulong blconf_g_property_bind_gdkcolor(BlconfChannel *channel,
                                       const gchar *blconf_property,
                                       gpointer object,
                                       const gchar *object_property);

gulong blconf_g_property_bind_gdkrgba(BlconfChannel *channel,
                                      const gchar *blconf_property,
                                      gpointer object,
                                      const gchar *object_property);

void blconf_g_property_unbind(gulong id);

void blconf_g_property_unbind_by_property(BlconfChannel *channel,
                                          const gchar *blconf_property,
                                          gpointer object,
                                          const gchar *object_property);

void blconf_g_property_unbind_all(gpointer channel_or_object);

G_END_DECLS

#endif  /* __BLCONF_BINDING_H__ */
