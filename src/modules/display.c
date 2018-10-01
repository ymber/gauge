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
