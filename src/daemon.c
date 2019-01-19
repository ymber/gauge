#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "config.h"

void *run_daemon() {
    for(;;) {
        for(unsigned int i = 0; i < sizeof(daemon_fncs) / sizeof(daemon_fncs[0]); ++i) {
            (*daemon_fncs[i])();
        }
        nanosleep(&settings.daemon_freq, NULL);
    }
}
