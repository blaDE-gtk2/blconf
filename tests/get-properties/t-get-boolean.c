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
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "tests-common.h"

#ifdef HAVE_STRING_H
#include <string.h>
#endif

int
main(int argc,
     char **argv)
{
    BlconfChannel *channel;
    
    if(!blconf_tests_start())
        return 1;
    
    channel = blconf_channel_new(TEST_CHANNEL_NAME);
    
    TEST_OPERATION(blconf_channel_get_bool(channel, test_bool_property, FALSE) == test_bool);
    
    g_object_unref(G_OBJECT(channel));
    
    blconf_tests_end();
    
    return 0;
}
