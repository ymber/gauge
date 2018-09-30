#include <stdio.h>

#include <xcb/xcb.h>
#include <cairo.h>

#include "data.h"
#include "data_curves.h"
#include "../overlay.h"

#define M_PI 3.14159265358979323846

void (*ring_fncs[3])(cairo_t *context) = {
    seconds_ring,
    memory_ring,
    cpu_rings
};

void seconds_ring(cairo_t *context) {
    Data_Curve_t seconds_ring = {
        60,
        {1, 1, 1, 0.3},
        {1, 1, 1, 0.6},
        110,
        160,
        65,
        4,
        -(M_PI/2),
        (3.0/2.0) * M_PI
    };
    double coverage = get_time()->tm_sec / seconds_ring.max;
    draw_ring(context, &seconds_ring, coverage);
}

void memory_ring(cairo_t *context) {
    Data_Curve_t memory_ring = {
        1,
        {1, 1, 1, 0.3},
        {1, 1, 1, 0.6},
        110,
        160,
        84,
        22,
        (25.0/36.0) * M_PI,
        (4.0/3.0) * M_PI
    };
    double mem_usage = get_memory_usage();
    double coverage = mem_usage / memory_ring.max;
    draw_ring(context, &memory_ring, coverage);
}

void cpu_rings(cairo_t *context) {
    Data_Curve_t cpu_rings[4] = {
        {
            1,
            {1, 1, 1, 0.3},
            {1, 1, 1, 0.6},
            110,
            160,
            75,
            5,
            (1.0/36.0) * M_PI,
            (2.0/3.0) * M_PI
        },
        {
            1,
            {1, 1, 1, 0.3},
            {1, 1, 1, 0.6},
            110,
            160,
            81,
            5,
            (1.0/36.0) * M_PI,
            (2.0/3.0) * M_PI
        },
        {
            1,
            {1, 1, 1, 0.3},
            {1, 1, 1, 0.6},
            110,
            160,
            87,
            5,
            (1.0/36.0) * M_PI,
            (2.0/3.0) * M_PI
        },
        {
            1,
            {1, 1, 1, 0.3},
            {1, 1, 1, 0.6},
            110,
            160,
            93,
            5,
            (1.0/36.0) * M_PI,
            (2.0/3.0) * M_PI
        }
    };
    for (int i = 0; i < 4; ++i) {
        char cpu_string[4];
        sprintf(cpu_string, "cpu%d", i);
        double cpu_usage = get_cpu_utilization(cpu_string);
        double coverage = cpu_usage / cpu_rings[i].max;
        draw_ring(context, &cpu_rings[i], coverage);
    }
}

void draw_ring(cairo_t *context, Data_Curve_t *arc, double coverage)
{
    double filled_arc_length = (arc->end_angle - arc->start_angle) * coverage;

    // Draw arc background
    cairo_set_source_rgba(context, arc->bg_rgba.r, arc->bg_rgba.g, arc->bg_rgba.b, arc->bg_rgba.a);
    cairo_arc(context, arc->x, arc->y, arc->radius, arc->start_angle, arc->end_angle);
    cairo_set_line_width(context, arc->thickness);
    cairo_stroke(context);

    // Draw arc foreground
    cairo_set_source_rgba(context, arc->fg_rgba.r, arc->fg_rgba.g, arc->fg_rgba.b, arc->fg_rgba.a);
    cairo_arc(context, arc->x, arc->y, arc->radius, arc->start_angle, arc->start_angle + filled_arc_length);
    cairo_stroke(context);
}

void draw_rings(cairo_t *context)
{
    for(unsigned int i = 0; i < sizeof(ring_fncs) / sizeof(ring_fncs[0]); ++i) {
        (*ring_fncs[i])(context);
    }
}
