#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "display.h"

void draw_line_chart(cairo_t *context, line_chart_t *chart, double *point_set, int reverse)
{
    cairo_set_source_rgba(context, chart->rgba.r, chart->rgba.g, chart->rgba.b, chart->rgba.a);
    cairo_set_line_width(context, chart->thickness);
    int point_distance = chart->width / chart->data_count;
    for (int i = 0; i < chart->data_count; ++i)
    {
        int point_height = chart->y - (point_set[i] * (chart->height / chart->max));
        if (!reverse)
        {
            if (i == 0)
            {
                cairo_move_to(context, chart->x + i * point_distance, point_height);
            }
            else
            {
                cairo_line_to(context, chart->x + i * point_distance, point_height);
            }
        }
        else
        {
            if (i == 0)
            {
                cairo_move_to(context, (chart->x + chart->width) - i * point_distance, point_height);
            }
            else
            {
                cairo_line_to(context, (chart->x + chart->width) - i * point_distance, point_height);
            }
        }
    }
    cairo_stroke(context);
}

void draw_ring(cairo_t *context, data_arc_t *arc, double coverage)
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
