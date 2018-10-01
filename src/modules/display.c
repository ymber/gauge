#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "display.h"

void line_chart(cairo_t *context, Line_Chart_t *chart, Point_set_t *point_set) {
    int point_distance = chart->x / chart->point_num;
    for (int i = 1; i < point_set->size; ++i) {
        int point_height = chart->start_y - (point_set->points[i] * (chart->y / chart->max));
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
    double points[30];
    srand(time(NULL));
    for (int i = 0; i < 30; ++i) {
        points[i] = rand() % 101;
    }
    Point_set_t pointset = {
        30,
        points
    };
    line_chart(context, &chart1, &pointset);
}
