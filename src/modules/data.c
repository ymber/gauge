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

int get_seconds()
{
    return get_time()->tm_sec;
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
	getline(&str, &len, fp);
	char *memtotal_line = malloc(strlen(str));
	strcpy(memtotal_line, str);
	getline(&str, &len, fp);
	char *memfree_line = malloc(strlen(str));
	strcpy(memfree_line, str);
	free(str);

	strtok(memtotal_line, " ");
	double memtotal = strtol(strtok(NULL, " "), NULL, 10);
	strtok(memfree_line, " ");
	double memfree = strtol(strtok(NULL, " "), NULL, 10);

	return memfree / memtotal;
}
