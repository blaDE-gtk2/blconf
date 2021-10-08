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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <libbladeutil/libbladeutil.h>

#include "blconf-backend-factory.h"
#include "blconf-backend.h"

/* i'm not sure i like this method.  perhaps each backend could be a
 * GTypeModule.  i also want the ability to multiplex multiple backends.
 * for example, i'd like to write a MCS backend that can read the old MCS
 * config files to ease migration to the new system, but of course the 'new'
 * backend should be the one that gets written to all the time.
 */

#ifdef BUILD_BLCONF_BACKEND_PERCHANNEL_XML
#include "blconf-backend-perchannel-xml.h"
#endif

static GHashTable *backends = NULL;

static void
blconf_backend_factory_ensure_backends(void)
{
    if(backends)
        return;
    
    backends = g_hash_table_new_full(g_str_hash, g_str_equal,
                                     NULL, (GDestroyNotify)g_free);
    
#ifdef BUILD_BLCONF_BACKEND_PERCHANNEL_XML
    {
        GType *gtype = g_new(GType, 1);
        *gtype = BLCONF_TYPE_BACKEND_PERCHANNEL_XML;
        g_hash_table_insert(backends,
                            (gpointer)BLCONF_BACKEND_PERCHANNEL_XML_TYPE_ID,
                            gtype);
    }
#endif
}


BlconfBackend *
blconf_backend_factory_get_backend(const gchar *type,
                                   GError **error)
{
    BlconfBackend *backend = NULL;
    GType *backend_gtype;
    
    blconf_backend_factory_ensure_backends();
    
    backend_gtype = g_hash_table_lookup(backends, type);
    if(!backend_gtype) {
        if(error) {
            g_set_error(error, BLCONF_ERROR, 0,
                        _("Unable to find Blconf backend of type \"%s\""),
                        type);
        }
        return NULL;
    }
    
    backend = g_object_new(*backend_gtype, NULL);
    if(!blconf_backend_initialize(backend, error)) {
        g_object_unref(G_OBJECT(backend));
        return NULL;
    }
    
    return backend;
}


void
blconf_backend_factory_cleanup (void)
{
  if(backends) {
      g_hash_table_destroy(backends);
      backends = NULL;
  }
}
