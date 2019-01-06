#include <cairo.h>

#include "config.h"
#include "modules/status.h"
#include "modules/background.h"

Settings_t settings = {
    1024,
    768,
    {1, 0},
    {1, 0}
};

void (*daemon_fncs[1])() = {
    update_status
};

void (*display_fncs[1])(cairo_t*) = {
    draw_background
};
