#ifndef DATA_H
#define DATA_H

#include <sys/vfs.h>

struct tm *get_time();
void get_cpu_stats(char *cpuN, int* stats);
double get_memory_usage();
struct statfs get_disk_stats(char *disk);

#endif
