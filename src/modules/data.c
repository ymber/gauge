#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include <sys/vfs.h>

struct tm *get_time()
{
    time_t current_time;
    struct tm *bd_time;

    current_time = time(NULL);
    bd_time = localtime(&current_time);

    return bd_time;
}

void get_cpu_stats(char *cpuN, int *stats) {
    char *str = NULL;
    size_t len = 0;
    char *token;
    char *stat_line = NULL;
    FILE *fp = fopen("/proc/stat", "r");
    while(getline(&str, &len, fp) < 0) {
        if(!stat_line) {
            stat_line = malloc(strlen(str) + 1);
        }
        strcpy(stat_line, str);
        token = strtok(str, " ");
        if (strcmp(token, cpuN) == 0) {
            free(str);
            fclose(fp);
            break;
        }
        else if (token == NULL) {
            fprintf(stderr, "no cpuid %s found in /proc/stat\n", cpuN);
            free(str);
            fclose(fp);
        }
    }

    token = strtok(stat_line, " ");
    // 10 time values given by kernels >2.6.33
    for(int i = 0; i < 10; ++i) {
        // discard cpuN token
        token = strtok(NULL, " ");
        stats[i] = strtol(token, NULL, 10);
    }

    free(stat_line);
}

double get_memory_usage() {
    char *str = NULL;
    size_t len = 0;
    FILE *fp = fopen("/proc/meminfo", "r");

    // Stores values for MemTotal, MemFree, Buffers, and Cache
    int mem_vals[4];

    int index = 0;
    for (int i = 0; i < 5; ++i) {
        // Don't store MemAvailable value
        if (i == 2) {
            if(getline(&str, &len, fp) < 0) {
                fprintf(stderr, "error reading line\n");
            }
            continue;
        }
        if(getline(&str, &len, fp) < 0) {
                fprintf(stderr, "error reading line\n");
        }
        strtok(str, " ");
        mem_vals[index] = strtol(strtok(NULL, " "), NULL, 10);
        ++index;
    }

    fclose(fp);
    free(str);

    // Do not count cache or buffers as used memory as in free
    double used_fraction = (double)(mem_vals[0] - mem_vals[1] - mem_vals[2] - mem_vals[3]) / (double)mem_vals[0];

    return used_fraction;
}

struct statfs get_disk_stats(char *disk) {
    struct statfs stats;
    if(statfs(disk, &stats) < 0) {
        fprintf(stderr, "failed to get stats for %s\n", disk);
    }
    return stats;
}
