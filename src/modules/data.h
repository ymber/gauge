#ifndef DATA_H
#define DATA_H

struct tm *get_time();
void get_cpu_stats(char *cpuN, int* stats);
double get_memory_usage();

#endif
