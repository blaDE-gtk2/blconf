/*
 * Copyright (c) 2008 Brian Tarricone <bjt23@cornell.edu>
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

#include "blconfperl.h"

#include <common/blconf-gvaluefuncs.h>
#include <common/blconf-common-private.h>

MODULE = Xfce4::Blconf::Binding   PACKAGE = Xfce4::Blconf::Binding  PREFIX = blconf_g_property_
PROTOTYPES: ENABLE

gulong
blconf_g_property_bind(channel, blconf_property, blconf_property_type, object, object_property)
        BlconfChannel * channel
        const gchar *blconf_property
        const gchar *blconf_property_type
        GObject *object
        const gchar *object_property
    CODE:
        RETVAL = 0;

        if(!g_ascii_strcasecmp(blconf_property_type, "gdkcolor")) {
            RETVAL = blconf_g_property_bind_gdkcolor(channel, blconf_property,
                                                     object, object_property);
        } else {
            GType blconf_gtype = _blconf_gtype_from_string(blconf_property_type);
            if(blconf_gtype == G_TYPE_INVALID)
                croak("Xfce4::Blconf::Binding::bind(): can't determine blconf property type from \"%s\"", blconf_property_type);
            if(blconf_gtype == G_TYPE_NONE || blconf_gtype == BLCONF_TYPE_G_VALUE_ARRAY)
                croak("Xfce4::Blconf::Binding::bind(): invalid blconf property type \"%s\" for binding", blconf_property_type);
            RETVAL = blconf_g_property_bind(channel, blconf_property, blconf_gtype,
                                            object, object_property);
        }

void
blconf_g_property_unbind(...)
    CODE:
        if(items != 1 && items != 4)
            croak("Usage: Xfce4::Blconf::Binding::unbind(id) or ::unbind(channel) or ::unbind(object) or ::unbind(channel, blconf_property, object, object_property)");

        if(items == 1) {
            GObject *channel_or_object;

            if((channel_or_object = SvGObject_ornull(ST(0))))
                blconf_g_property_unbind_all(channel_or_object);
            else if(SvIOK(ST(0)))
                blconf_g_property_unbind(SvIV(ST(0)));
        } else if(items == 4) {
            BlconfChannel *channel = SvBlconfChannel(ST(0));
            const gchar *blconf_property = SvGChar(ST(1));
            GObject *object = SvGObject(ST(2));
            const gchar *object_property = SvGChar(ST(3));

            blconf_g_property_unbind_by_property(channel, blconf_property, object, object_property);
        }
