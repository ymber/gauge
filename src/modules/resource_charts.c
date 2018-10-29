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
        100,
        400,
        200,
        300,
        300,
        40
    };

    static double points[40] = {0};
    points[0] = system_resources.cpu_avg_perc * 100;
    line_chart(context, &chart, points);
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