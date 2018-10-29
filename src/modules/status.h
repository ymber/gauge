#ifndef STATUS_H
#define STATUS_H

#include <cairo.h>

typedef struct {
    double cpu_perc[16];
} Resource_Data_t;

Resource_Data_t system_resources;
void update_status(cairo_t *context);

#endif
