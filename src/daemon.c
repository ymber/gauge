#include <stdlib.h>
#include <unistd.h>

#include "config.h"

void *run_daemon() {
    for(;;) {
        for(unsigned int i = 0; i < sizeof(daemon_fncs) / sizeof(daemon_fncs[0]); ++i) {
            (*daemon_fncs[i])();
        }
        sleep(1 / settings.daemon_freq);
    }
}
