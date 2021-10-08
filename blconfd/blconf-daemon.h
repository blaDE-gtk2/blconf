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

#ifndef __BLCONF_DAEMON_H__
#define __BLCONF_DAEMON_H__

#include <glib-object.h>

#define BLCONF_TYPE_DAEMON             (blconf_daemon_get_type())
#define BLCONF_DAEMON(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), BLCONF_TYPE_DAEMON, BlconfDaemon))
#define BLCONF_IS_DAEMON(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), BLCONF_TYPE_DAEMON))
#define BLCONF_DAEMON_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), BLCONF_TYPE_DAEMON, BlconfDaemonClass))
#define BLCONF_IS_DAEMON_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), BLCONF_TYPE_DAEMON))
#define BLCONF_DAEMON_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), BLCONF_TYPE_DAEMON, BlconfDaemonClass))

G_BEGIN_DECLS

typedef struct _BlconfDaemon         BlconfDaemon;

GType blconf_daemon_get_type(void) G_GNUC_CONST;

BlconfDaemon *blconf_daemon_new_unique(gchar * const *backend_ids,
                                       GError **error);

G_END_DECLS

#endif  /* __BLCONF_DAEMON_H__ */
