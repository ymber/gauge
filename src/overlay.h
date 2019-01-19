#ifndef OVERLAY_H
#define OVERLAY_H

#include <xcb/xproto.h>
#include <xcb/xcb.h>

typedef struct {
    xcb_connection_t *connection;
    xcb_screen_t *screen;
    xcb_window_t drawable;
    xcb_visualtype_t *visual;
} Window;

extern int display_active;

void spawn_overlay();
void setup_display_signals();
void *run_overlay();
void signal_overlay_off();

#endif
