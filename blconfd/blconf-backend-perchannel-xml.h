/*
 *  blconf
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

#ifndef __BLCONF_BACKEND_PERCHANNEL_XML_H__
#define __BLCONF_BACKEND_PERCHANNEL_XML_H__

#include <glib-object.h>

#define BLCONF_TYPE_BACKEND_PERCHANNEL_XML             (blconf_backend_perchannel_xml_get_type())
#define BLCONF_BACKEND_PERCHANNEL_XML(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), BLCONF_TYPE_BACKEND_PERCHANNEL_XML, BlconfBackendPerchannelXml))
#define BLCONF_IS_BACKEND_PERCHANNEL_XML(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), BLCONF_TYPE_BACKEND_PERCHANNEL_XML))
#define BLCONF_BACKEND_PERCHANNEL_XML_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), BLCONF_TYPE_BACKEND_PERCHANNEL_XML, BlconfBackendPerchannelXmlClass))
#define BLCONF_IS_BACKEND_PERCHANNEL_XML_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), BLCONF_TYPE_BACKEND_PERCHANNEL_XML))
#define BLCONF_BACKEND_PERCHANNEL_XML_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), BLCONF_TYPE_BACKEND_PERCHANNEL_XML, BlconfBackendPerchannelXmlClass))

#define BLCONF_BACKEND_PERCHANNEL_XML_TYPE_ID          "xfce-perchannel-xml"

G_BEGIN_DECLS

typedef struct _BlconfBackendPerchannelXml         BlconfBackendPerchannelXml;

GType blconf_backend_perchannel_xml_get_type(void) G_GNUC_CONST;

G_END_DECLS

#endif  /* __BLCONF_BACKEND_PERCHANNEL_XML_H__ */
