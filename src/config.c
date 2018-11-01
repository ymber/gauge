#include <cairo.h>

#include "config.h"
#include "modules/status.h"
#include "modules/background.h"
#include "modules/data_curves.h"
#include "modules/resource_charts.h"
#include "modules/date_time.h"

Settings_t settings = {
    1024,
    768,
    {1, 0},
    {1, 0}
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
