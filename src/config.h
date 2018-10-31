#ifndef CONFIG_H
#define CONFIG_H

#include <cairo.h>

#include "overlay.h"
#include "modules/status.h"
#include "modules/background.h"
#include "modules/data_curves.h"
#include "modules/resource_charts.h"
#include "modules/date_time.h"

typedef struct {
    int size_x;
    int size_y;
    double frequency;
} Settings_t;

Settings_t settings = {
    1024,
    768,
    1
};

void (*daemon_fncs[1])() = {
    update_status
};

void (*display_fncs[4])(cairo_t*) = {
    draw_background,
    draw_rings,
    draw_charts,
    date_time
};

#endif
