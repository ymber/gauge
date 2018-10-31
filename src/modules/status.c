#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/sysinfo.h>

#include "status.h"
#include "data.h"

double get_cpu_utilization(int* stats1, int * stats2) {
    int stats_diff[10];
    int total = 0;
    int working = 0;

    for(int i = 0; i < 10; ++i) {
        // stats1 may be a later snapshot than stats2 so use absolute values
        stats_diff[i] = abs(stats2[i] - stats1[i]);
        total = total + stats_diff[i];
        // Do not add idle and iowait to working time
        if(i == 3 || i == 4) {
            continue;
        }
        else {
            working = working + stats_diff[i];
        }
    }
    return (double)working / (double)total;
}

void update_cpu_data() {
    static bool clock = 0;
    static int stats[2][16][10] = {0};
    for(int i = 0; i < get_nprocs(); ++i) {
        char cpu_string[5];
        sprintf(cpu_string, "cpu%d", i);
        get_cpu_stats(cpu_string, stats[clock][i]);
        system_resources.cpu_perc[i] = get_cpu_utilization(stats[0][i], stats[1][i]);
    }

    static int core_avg_stats[2][10] = {0};
    get_cpu_stats("cpu", core_avg_stats[clock]);
    system_resources.cpu_avg_perc = get_cpu_utilization(core_avg_stats[0], core_avg_stats[1]);

    for(int i = 0; i < 59; ++i) {
        system_resources.cpu_avg_perc_min[59 - i] = system_resources.cpu_avg_perc_min[59 - (i+1)];
    }
    system_resources.cpu_avg_perc_min[0] = system_resources.cpu_avg_perc;

    clock = !clock;
}

void update_status() {
    update_cpu_data();
}
