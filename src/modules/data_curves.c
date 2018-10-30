#include <stdio.h>

#include <xcb/xcb.h>
#include <cairo.h>

#include "data.h"
#include "status.h"
#include "display.h"
#include "data_curves.h"
#include "../overlay.h"

void (*ring_fncs[4])(cairo_t *context) = {
    seconds_ring,
    memory_ring,
    cpu_rings,
    disk_usage_rings
};

void seconds_ring(cairo_t *context) {
    Data_Curve_t seconds_ring = {
        60,
        {1, 1, 1, 0.3},
        {1, 1, 1, 0.6},
        160,
        240,
        65,
        4,
        -(M_PI/2),
        (3.0/2.0) * M_PI
    };
    double coverage = get_time()->tm_sec / seconds_ring.max;
    draw_ring(context, &seconds_ring, coverage);
}

void memory_ring(cairo_t *context) {
    Data_Curve_t memory_ring = {
        1,
        {1, 1, 1, 0.3},
        {1, 1, 1, 0.6},
        160,
        240,
        84,
        22,
        (25.0/36.0) * M_PI,
        (4.0/3.0) * M_PI
    };
    double mem_usage = get_memory_usage();
    double coverage = mem_usage / memory_ring.max;
    draw_ring(context, &memory_ring, coverage);
}

void cpu_rings(cairo_t *context) {
    Data_Curve_t cpu_rings[4] = {
        {
            1,
            {1, 1, 1, 0.3},
            {1, 1, 1, 0.6},
            160,
            240,
            75,
            5,
            (1.0/36.0) * M_PI,
            (2.0/3.0) * M_PI
        },
        {
            1,
            {1, 1, 1, 0.3},
            {1, 1, 1, 0.6},
            160,
            240,
            81,
            5,
            (1.0/36.0) * M_PI,
            (2.0/3.0) * M_PI
        },
        {
            1,
            {1, 1, 1, 0.3},
            {1, 1, 1, 0.6},
            160,
            240,
            87,
            5,
            (1.0/36.0) * M_PI,
            (2.0/3.0) * M_PI
        },
        {
            1,
            {1, 1, 1, 0.3},
            {1, 1, 1, 0.6},
            160,
            240,
            93,
            5,
            (1.0/36.0) * M_PI,
            (2.0/3.0) * M_PI
        }
    };
    for (int i = 0; i < 4; ++i) {
        double coverage = system_resources.cpu_perc[i] / cpu_rings[i].max;
        draw_ring(context, &cpu_rings[i], coverage);
    }

}

void disk_usage_rings(cairo_t *context) {
    Data_Curve_t disk_rings[2] = {
        {
            1,
            {1, 1, 1, 0.3},
            {1, 1, 1, 0.6},
            160,
            240,
            105,
            5,
            -(17.0/36.0) * M_PI,
            (1.0/6.0) * M_PI
        },
        {
            1,
            {1, 1, 1, 0.3},
            {1, 1, 1, 0.6},
            160,
            240,
            105,
            5,
            -(7.0/6.0) * M_PI,
            -(19.0/36.0) * M_PI
        }
    };
    struct statfs stats[2] = {get_disk_stats("/"), get_disk_stats("/home")};
    for(int i = 0; i < 2; ++i) {
        int disk_used = stats[i].f_blocks - stats[i].f_bfree;
        double coverage = (double)disk_used / (double)stats[i].f_blocks;
        draw_ring(context, &disk_rings[i], coverage);
    }
}

void draw_rings(cairo_t *context)
{
    for(unsigned int i = 0; i < sizeof(ring_fncs) / sizeof(ring_fncs[0]); ++i) {
        (*ring_fncs[i])(context);
    }
}
