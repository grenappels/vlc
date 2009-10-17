/*****************************************************************************
 * window.c: "vout window" managment
 *****************************************************************************
 * Copyright (C) 2009 Laurent Aimar
 * $Id$
 *
 * Authors: Laurent Aimar <fenrir _AT_ videolan _DOT_ org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

/*****************************************************************************
 * Preamble
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#include <assert.h>

#include <vlc_common.h>
#include <vlc_vout_window.h>
#include <libvlc.h>

vout_window_t *vout_window_New(vlc_object_t *obj,
                               const char *module,
                               const vout_window_cfg_t *cfg)
{
    static char const name[] = "window";
    vout_window_t *window = vlc_custom_create(obj, sizeof(*window),
                                              VLC_OBJECT_GENERIC, name);
    window->cfg = cfg;
    memset(&window->handle, 0, sizeof(window->handle));
    window->control = NULL;
    window->sys = NULL;

    vlc_object_attach(window, obj);

    const char *type;
    switch (cfg->type) {
    case VOUT_WINDOW_TYPE_HWND:
        type = "vout window hwnd";
        break;
    case VOUT_WINDOW_TYPE_XID:
        type = "vout window xid";
        break;
    default:
        assert(0);
    }

    window->module = module_need(window, type,
                                 module, module && *module != '\0');
    if (!window->module) {
        vlc_object_detach(window);
        vlc_object_release(window);
        return NULL;
    }
    return window;
}

void vout_window_Delete(vout_window_t *window)
{
    if (!window)
        return;

    vlc_object_detach(window);

    module_unneed(window, window->module);

    vlc_object_release(window);
}

int vout_window_Control(vout_window_t *window, int query, ...)
{
    va_list args;
    va_start(args, query);
    int ret = window->control(window, query, args);
    va_end(args);

    return ret;
}

