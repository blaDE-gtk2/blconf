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

#ifndef __BLCONF_GVALUEFUNCS_H__
#define __BLCONF_GVALUEFUNCS_H__

#include <glib-object.h>

G_BEGIN_DECLS

G_GNUC_INTERNAL GType _blconf_gtype_from_string(const gchar *type);
G_GNUC_INTERNAL const gchar *_blconf_string_from_gtype(GType gtype);

G_GNUC_INTERNAL gboolean _blconf_gvalue_from_string(GValue *value,
                                                    const gchar *str);

G_GNUC_INTERNAL gchar *_blconf_string_from_gvalue(GValue *value);

G_GNUC_INTERNAL gboolean _blconf_gvalue_is_equal(const GValue *value1,
                                                 const GValue *value2);

G_GNUC_INTERNAL void _blconf_gvalue_free(GValue *value);

G_END_DECLS

#endif  /* __BLCONF_GVALUEFUNCS_H__ */
