#include <xcb/xcb.h>
#include <cairo.h>

#include "../overlay.h"

void draw_ring(cairo_t *context, Window *window) {
    cairo_set_source_rgba(context, 0, 0, 1, 1);
    cairo_arc(context, 50, 50, 20, 0, 2);
    cairo_stroke(context);
    xcb_flush(window->connection);
}
