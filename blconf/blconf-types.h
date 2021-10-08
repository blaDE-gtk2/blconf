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

#ifndef __BLCONF_TYPES_H__
#define __BLCONF_TYPES_H__

#if !defined(LIBBLCONF_COMPILATION) && !defined(BLCONF_IN_BLCONF_H)
#error "Do not include blconf-types.h, as this file may change or disappear in the future.  Include <blconf/blconf.h> instead."
#endif

#include <glib-object.h>

#define BLCONF_TYPE_UINT16  (blconf_uint16_get_type())
#define BLCONF_TYPE_INT16   (blconf_int16_get_type())

G_BEGIN_DECLS

GType blconf_uint16_get_type(void) G_GNUC_CONST;

guint16 blconf_g_value_get_uint16(const GValue *value);
void blconf_g_value_set_uint16(GValue *value,
                               guint16 v_uint16);

GType blconf_int16_get_type(void) G_GNUC_CONST;

gint16 blconf_g_value_get_int16(const GValue *value);
void blconf_g_value_set_int16(GValue *value,
                              gint16 v_int16);

G_END_DECLS

#endif
