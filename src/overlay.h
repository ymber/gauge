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

void *run_overlay();

#endif
