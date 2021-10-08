/*
 *  blconf
 *
 *  Copyright (c) 2009 Brian Tarricone <brian@tarricone.org>
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

#ifndef __BLCONF_CACHE_H__
#define __BLCONF_CACHE_H__

#include <glib-object.h>

#define BLCONF_TYPE_CACHE             (blconf_cache_get_type())
#define BLCONF_CACHE(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), BLCONF_TYPE_CACHE, BlconfCache))
#define BLCONF_IS_CACHE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), BLCONF_TYPE_CACHE))
#define BLCONF_CACHE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), BLCONF_TYPE_CACHE, BlconfCacheClass))
#define BLCONF_IS_CACHE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), BLCONF_TYPE_CACHE))
#define BLCONF_CACHE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), BLCONF_TYPE_CACHE, BlconfCacheClass))

G_BEGIN_DECLS

typedef struct _BlconfCache         BlconfCache;

G_GNUC_INTERNAL
GType blconf_cache_get_type(void) G_GNUC_CONST;

G_GNUC_INTERNAL
BlconfCache *blconf_cache_new(const gchar *channel_name) G_GNUC_MALLOC;

G_GNUC_INTERNAL
gboolean blconf_cache_prefetch(BlconfCache *cache,
                               const gchar *property_base,
                               GError **error);

G_GNUC_INTERNAL
gboolean blconf_cache_lookup(BlconfCache *cache,
                             const gchar *property,
                             GValue *value,
                             GError **error);

G_GNUC_INTERNAL
gboolean blconf_cache_set(BlconfCache *cache,
                          const gchar *property,
                          const GValue *value,
                          GError **error);

G_GNUC_INTERNAL
gboolean blconf_cache_reset(BlconfCache *cache,
                            const gchar *property_base,
                            gboolean recursive,
                            GError **error);
#if 0
G_GNUC_INTERNAL
void blconf_cache_set_max_entries(BlconfCache *cache,
                                  gint max_entries);
G_GNUC_INTERNAL
gint blconf_cache_get_max_entries(BlconfCache *cache);

G_GNUC_INTERNAL
void blconf_cache_set_max_age(BlconfCache *cache,
                              gint max_age);
G_GNUC_INTERNAL
gint blconf_cache_get_max_age(BlconfCache *cache);
#endif
G_END_DECLS

#endif  /* __BLCONF_CACHE_H__ */
