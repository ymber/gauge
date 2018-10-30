#include <cairo.h>

#include "resource_charts.h"
#include "display.h"
#include "data.h"
#include "status.h"

void (*chart_fncs[1])(cairo_t *context) = {
    cpu_chart
};

void cpu_chart(cairo_t *context) {
    Line_Chart_t chart = {
        1,
        300,
        50,
        285,
        240,
        60,
        {1, 1, 1, 1},
        3
    };

    static double points[60] = {0};
    points[0] = system_resources.cpu_avg_perc;

    // Draw chart boundary decorations
    cairo_set_source_rgba(context, 1, 1, 1, 1);
    cairo_set_line_width(context, chart.line_width);
    cairo_move_to(context, 270, 240);
    cairo_line_to(context, 700, 240);
    cairo_stroke(context);
    cairo_arc(context, -135, 240, 425, -(1.0/24.0) * M_PI, (1.0/48.0) * M_PI);
    cairo_stroke(context);
    cairo_arc(context, 160, 240, 425, -(1.0/24.0) * M_PI, (1.0/48.0) * M_PI);
    cairo_stroke(context);

    line_chart(context, &chart, points, 1);
    int max_index = chart.point_num - 1;
    for(int i = 0; i < max_index; ++i) {
        points[max_index - i] = points[max_index - (i+1)];
    }
}

void draw_charts(cairo_t *context) {
    for(unsigned int i = 0; i < sizeof(chart_fncs) / sizeof(chart_fncs[0]); ++i) {
        (*chart_fncs[i])(context);
    }
}