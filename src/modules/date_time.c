#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <cairo.h>

#include "data.h"

void date_time(cairo_t *context) {
    static char *months[12] = {
        "January",
        "February",
        "March",
        "April",
        "May",
        "June",
        "July",
        "August",
        "September",
        "October",
        "November",
        "December"
    };

    // Ordered as such because tm_wday begins from sunday
    static char *days[7] = {
        "Sunday",
        "Monday",
        "Tuesday",
        "Wednesday",
        "Thursday",
        "Friday",
        "Saturday"
    };
    struct tm *bd_time = get_time();
    char hm[5];
    sprintf(hm, "%02d:%02d", bd_time->tm_hour, bd_time->tm_min);
    double x;
    double y;

    x = 105;
    y = 253;
    cairo_set_source_rgba(context, 1, 1, 1, 1);
    cairo_select_font_face (context, "Droid Sans Mono", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size (context, 36);
    cairo_move_to (context, x, y);
    cairo_show_text (context, hm);
    cairo_stroke (context);

    char *date_string;
    date_string = malloc(snprintf(NULL, 0, "%s, %s %d %d", days[bd_time->tm_wday], months[bd_time->tm_mon], bd_time->tm_mday, bd_time->tm_year + 1900));
    sprintf(date_string, "%s, %s %d %d", days[bd_time->tm_wday], months[bd_time->tm_mon], bd_time->tm_mday, bd_time->tm_year + 1900);
    x = 315;
    y = 310;
    cairo_set_source_rgba(context, 1, 1, 1, 1);
    cairo_select_font_face (context, "Droid Sans Mono", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size (context, 24);
    cairo_move_to (context, x, y);
    cairo_show_text (context, date_string);
    cairo_stroke (context);
    free(date_string);
}
