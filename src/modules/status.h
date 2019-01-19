#ifndef STATUS_H
#define STATUS_H

#include <pthread.h>

#include <cairo.h>

typedef struct {
    double cpu_perc[16];
    double cpu_avg_perc;
    double cpu_avg_perc_min[60];
} Resource_Data_t;

extern pthread_mutex_t mutex_system_resources;
extern Resource_Data_t system_resources;
void update_status();

#endif
