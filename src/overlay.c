#include <xcb/xcb.h>
#include <cairo.h>
#include <cairo-xcb.h>
#include <lauxlib.h>
#include <lualib.h>

#include <stdio.h>
#include <unistd.h>

#include "overlay.h"
#include "luastuff.h"
#include "config.h"

xcb_visualtype_t *find_visual(xcb_connection_t *connection, xcb_visualid_t visual)
{
    xcb_screen_iterator_t screen_itr = xcb_setup_roots_iterator(xcb_get_setup(connection));

    for(; screen_itr.rem; xcb_screen_next(&screen_itr)) {
        xcb_depth_iterator_t depth_itr = xcb_screen_allowed_depths_iterator(screen_itr.data);
        for(; depth_itr.rem; xcb_depth_next(&depth_itr)) {
            xcb_visualtype_iterator_t visual_itr = xcb_depth_visuals_iterator(depth_itr.data);
            for(; visual_itr.rem; xcb_visualtype_next(&visual_itr))
                if(visual == visual_itr.data->visual_id) {
                    return visual_itr.data;
                }
        }
    }

    return NULL;
}

Window create_window()
{
    Window window_data;
    window_data.connection = xcb_connect(NULL, NULL);

    const xcb_setup_t *setup = xcb_get_setup(window_data.connection);
    xcb_screen_iterator_t itr = xcb_setup_roots_iterator(setup);
    window_data.screen = itr.data;
    window_data.visual = find_visual(window_data.connection, window_data.screen->root_visual);
    window_data.drawable = xcb_generate_id(window_data.connection);

    uint32_t mask = XCB_CW_OVERRIDE_REDIRECT;
    uint32_t values [1] = {1};
    xcb_create_window(window_data.connection, window_data.screen->root_depth, window_data.drawable,
                      window_data.screen->root, 0, 0, window_data.screen->width_in_pixels,
                      window_data.screen->height_in_pixels, 0,
                      XCB_WINDOW_CLASS_INPUT_OUTPUT, window_data.screen->root_visual, mask, values);
    xcb_map_window(window_data.connection, window_data.drawable);
    xcb_flush(window_data.connection);

    return window_data;
}

void draw(cairo_surface_t* surface, cairo_t *context, Window *window) {
    cairo_set_source_rgba(context, 1, 0, 0, 1);
    cairo_paint(context);
    xcb_flush(window->connection);
}

int main()
{
    Window window = create_window();
    cairo_surface_t *cairo_surface = cairo_xcb_surface_create(window.connection, window.drawable,
                                     window.visual, window.screen->width_in_pixels, window.screen->height_in_pixels);
    cairo_t *cairo_context = cairo_create(cairo_surface);

    draw(cairo_surface, cairo_context, &window);

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    load_settings(L, "config.lua");
    luastuff(L);

    pause();

    cairo_destroy(cairo_context);
    xcb_disconnect(window.connection);

    return 0;
}
