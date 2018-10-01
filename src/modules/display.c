#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "display.h"

void line_chart(cairo_t *context, Line_Chart_t *chart, Point_set_t *point_set) {
    int point_distance = chart->x / chart->point_num;
    for (int i = 0; i < point_set->size; ++i) {
        int point_height = chart->start_y - (point_set->points[i] * (chart->y / chart->max));
        if (i == 0) {
            cairo_move_to(context, chart->start_x + i * point_distance, point_height);
        }
        else {
            cairo_line_to(context, chart->start_x + i * point_distance, point_height);
        }
    }
    cairo_stroke(context);
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
