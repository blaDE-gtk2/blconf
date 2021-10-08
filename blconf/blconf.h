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

#ifndef __BLCONF_H__
#define __BLCONF_H__

#include <glib.h>

#define BLCONF_IN_BLCONF_H

#include <blconf/blconf-channel.h>
#include <blconf/blconf-binding.h>
#include <blconf/blconf-errors.h>
#include <blconf/blconf-types.h>

#undef BLCONF_IN_BLCONF_H

G_BEGIN_DECLS

gboolean blconf_init(GError **error);
void blconf_shutdown(void);

void blconf_named_struct_register(const gchar *struct_name,
                                  guint n_members,
                                  const GType *member_types);

void blconf_array_free(GPtrArray *arr);

gchar **blconf_list_channels(void) G_GNUC_WARN_UNUSED_RESULT;

G_END_DECLS

#endif  /* __BLCONF_H__ */
