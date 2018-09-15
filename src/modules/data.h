#ifndef DATA_H
#define DATA_H

struct tm *get_time();
int get_seconds();
double get_cpu_utilization(char *cpuN);
double get_memory_usage();

#endif
