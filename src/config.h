#ifndef CONFIG_H
#define CONFIG_H

#include <cairo.h>

#include "overlay.h"
#include "modules/background.h"
#include "modules/data_curves.h"
#include "modules/display.h"

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

void (*call[3])(cairo_t*) = {
    draw_background,
    draw_rings,
    draw_chart
};

#endif
