#include <time.h>

struct tm *get_time()
{
    time_t current_time;
    struct tm *bd_time;

    current_time = time(NULL);
    bd_time = localtime(&current_time);

    return bd_time;
}

double get_seconds()
{
    return (double)get_time()->tm_sec;
}
