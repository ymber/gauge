#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "display.h"

void line_chart(cairo_t *context, Line_Chart_t *chart, double *points) {
    int point_distance = chart->x / chart->point_num;
    for (int i = 1; i < chart->point_num; ++i) {
        int point_height = chart->start_y - (points[i] * (chart->max / chart->y));
        if (i == 1) {
            cairo_move_to(context, chart->start_x + i * point_distance, point_height);
        }
        else {
            cairo_line_to(context, chart->start_x + i * point_distance, point_height);
        }
    }
    cairo_stroke(context);
}

void draw_chart(cairo_t *context) {
    Line_Chart_t chart1 = {
        100,
        500,
        100,
        200,
        700,
        50
    };
    double points[50];
    srand(time(NULL));
    for (int i = 1; i < 50; ++i) {
        points[i] = rand() % 101;
    }
    line_chart(context, &chart1, points);
}
