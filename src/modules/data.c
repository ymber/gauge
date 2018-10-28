#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

struct tm *get_time()
{
    time_t current_time;
    struct tm *bd_time;

    current_time = time(NULL);
    bd_time = localtime(&current_time);

    return bd_time;
}

double get_cpu_utilization(char *cpuN) {
    char *str = NULL;
    size_t len = 0;
    char *token;
    char *stat_line;
    FILE *fp = fopen("/proc/stat", "r");
    while (getline(&str, &len, fp) != -1) {
        stat_line = malloc(strlen(str));
        strcpy(stat_line, str);
        token = strtok(str, " ");
        if (strcmp(token, cpuN) == 0) {
            free(str);
            fclose(fp);
            break;
        }
        else if (token == NULL) {
            fprintf(stderr, "no cpuid %s found in /proc/stat\n", cpuN);
            fclose(fp);
            free(str);
        }
    }

    token = strtok(stat_line, " ");
    double idle = 0;
    double working = 0;
    int i = 0;
    while (token != NULL) {
        token = strtok(NULL, " ");
        if (token == NULL) {
            break;
        }
        // add idle and iowait figures to idle time
        else if (i == 3 || i == 4) {
            idle += strtol(token, NULL, 10);
        }
        // add all other figures to working time
        else {
            working += strtol(token, NULL, 10);
        }
        ++i;
    }
    free(stat_line);

    return working / (idle + working);
}

double get_memory_usage() {
    char *str = NULL;
    size_t len = 0;
    FILE *fp = fopen("/proc/meminfo", "r");

    // Stores values for MemTotal, MemFree, Buffers, and Cache
    int mem_vals[4];

    int index = 0;
    for (int i = 0; i < 5; ++i) {
        // Don't store MemAvailable line
        if (i == 2) {
            getline(&str, &len, fp);
            continue;
        }
        getline(&str, &len, fp);
        strtok(str, " ");
        mem_vals[index] = strtol(strtok(NULL, " "), NULL, 10);
        ++index;
    }

    fclose(fp);
    free(str);

    double used_fraction = (double)(mem_vals[0] - mem_vals[1] - mem_vals[2] - mem_vals[3]) / (double)mem_vals[0];
    printf("%f\n", used_fraction);

    return used_fraction;
}
