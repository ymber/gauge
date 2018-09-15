#include <cairo.h>

#include "../overlay.h"

void draw_background(cairo_t *context, Window *window)
{
    cairo_set_source_rgba(context, 1, 1, 1, 0.3);
    cairo_paint(context);
}
