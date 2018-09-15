#include <xcb/xcb.h>
#include <cairo.h>

#include "data.h"
#include "data_curves.h"
#include "../overlay.h"

Data_Curve_t rings[1] = {
    {
        get_memory_usage,
        1,
        {1, 1, 1, 0.3},
        {1, 1, 1, 0.6},
        110,
        160,
        84,
        22.5,
        4,
        6
    }
};

double get_coverage(Data_Curve_t *arc)
{
    return arc->data_fnc() / arc->max;
}

void draw_ring(cairo_t *context, Data_Curve_t arc)
{
    double filled_arc_length = (arc.end_angle - arc.start_angle) * get_coverage(&arc);

    // Draw arc background
    cairo_set_source_rgba(context, arc.bg_rgba.r, arc.bg_rgba.g, arc.bg_rgba.b, arc.bg_rgba.a);
    cairo_arc(context, arc.x, arc.y, arc.radius, arc.start_angle, arc.end_angle);
    cairo_set_line_width(context, arc.thickness);
    cairo_stroke(context);

    // Draw arc foreground
    cairo_set_source_rgba(context, arc.fg_rgba.r, arc.fg_rgba.g, arc.fg_rgba.b, arc.fg_rgba.a);
    cairo_arc(context, arc.x, arc.y, arc.radius, arc.start_angle, arc.start_angle + filled_arc_length);
    cairo_stroke(context);
}

void draw_rings(cairo_t *context, Window *window)
{
    for(int i = 0; i < sizeof(rings) / sizeof(rings[0]); ++i) {
        draw_ring(context, rings[i]);
    }
}
