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

#ifndef __BLCONF_ERRORS_H__
#define __BLCONF_ERRORS_H__

#if !defined(LIBBLCONF_COMPILATION) && !defined(BLCONF_IN_BLCONF_H)
#error "Do not include blconf-errors.h, as this file may change or disappear in the future.  Include <blconf/blconf.h> instead."
#endif

#include <glib-object.h>

#define BLCONF_TYPE_ERROR  (blconf_error_get_type())
#define BLCONF_ERROR       (blconf_get_error_quark())

G_BEGIN_DECLS

typedef enum
{
    BLCONF_ERROR_UNKNOWN = 0,
    BLCONF_ERROR_CHANNEL_NOT_FOUND,
    BLCONF_ERROR_PROPERTY_NOT_FOUND,
    BLCONF_ERROR_READ_FAILURE,
    BLCONF_ERROR_WRITE_FAILURE,
    BLCONF_ERROR_PERMISSION_DENIED,
    BLCONF_ERROR_INTERNAL_ERROR,
    BLCONF_ERROR_NO_BACKEND,
    BLCONF_ERROR_INVALID_PROPERTY,
    BLCONF_ERROR_INVALID_CHANNEL,
} BlconfError;

GType blconf_error_get_type(void) G_GNUC_CONST;
GQuark blconf_get_error_quark(void);

G_END_DECLS

#endif  /* __BLCONF_ERRORS_H__ */
