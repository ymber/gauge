#include <pthread.h>

#include "config.h"
#include "daemon.h"
#include "overlay.h"

int main()
{
    pthread_t daemon_thread;
    pthread_create(&daemon_thread, NULL, run_daemon, NULL);

    pthread_t display_thread;
    pthread_create(&display_thread, NULL, run_overlay, NULL);

    pthread_join(daemon_thread, NULL);

    return 0;
}