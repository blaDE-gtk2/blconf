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

#include "blconf/blconf-errors.h"
#include "blconf-alias.h"

static GQuark blconf_error_quark = 0;

/**
 * BLCONF_ERROR:
 *
 * The #GError error domain for Blconf.
 **/


/**
 * BLCONF_TYPE_ERROR:
 *
 * An enum GType for Blconf errors.
 **/


/**
 * BlconfError:
 *
 * An enumeration listing the different kinds of errors under the
 * BLCONF_ERROR domain.
 **/

GQuark
blconf_get_error_quark(void)
{
    if(!blconf_error_quark)
        blconf_error_quark = g_quark_from_static_string("blconf-error-quark");
    
    return blconf_error_quark;
}

/* unfortunately glib-mkenums can't generate types that are compatible with
 * dbus error names -- the 'nick' value is used, which can have dashes in it,
 * which dbus doesn't like. */
GType
blconf_error_get_type(void)
{
    static GType type = 0;
    
    if(!type) {
        static const GEnumValue values[] = {
            { BLCONF_ERROR_UNKNOWN, "BLCONF_ERROR_UNKNOWN", "Unknown" },
            { BLCONF_ERROR_CHANNEL_NOT_FOUND, "BLCONF_ERROR_CHANNEL_NOT_FOUND", "ChannelNotFound" },
            { BLCONF_ERROR_PROPERTY_NOT_FOUND, "BLCONF_ERROR_PROPERTY_NOT_FOUND", "PropertyNotFound" },
            { BLCONF_ERROR_READ_FAILURE, "BLCONF_ERROR_READ_FAILURE", "ReadFailure" },
            { BLCONF_ERROR_WRITE_FAILURE, "BLCONF_ERROR_WRITE_FAILURE", "WriteFailure" },
            { BLCONF_ERROR_PERMISSION_DENIED, "BLCONF_ERROR_PERMISSION_DENIED", "PermissionDenied" },
            { BLCONF_ERROR_INTERNAL_ERROR, "BLCONF_ERROR_INTERNAL_ERROR", "InternalError" },
            { BLCONF_ERROR_NO_BACKEND, "BLCONF_ERROR_NO_BACKEND", "NoBackend" },
            { BLCONF_ERROR_INVALID_PROPERTY, "BLCONF_ERROR_INVALID_PROPERTY", "InvalidProperty" },
            { BLCONF_ERROR_INVALID_CHANNEL, "BLCONF_ERROR_INVALID_CHANNEL", "InvalidChannel" },
            { 0, NULL, NULL }
        };
        
        type = g_enum_register_static("BlconfError", values);
    }
    
    return type;
}



#define __BLCONF_ERRORS_C__
#include "blconf-aliasdef.c"
