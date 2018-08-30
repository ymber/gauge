#ifndef OVERLAY_H
#define OVERLAY_H

#include <xcb/xcb.h>
#include <cairo.h>

typedef struct {
    xcb_connection_t *connection;
    xcb_screen_t *screen;
    xcb_window_t drawable;
    xcb_visualtype_t *visual;
} Window;

xcb_visualtype_t *find_visual(xcb_connection_t *connection, xcb_visualid_t visual);
Window create_window();
void draw(cairo_surface_t* surface, cairo_t *context, Window *window);

#endif
