#include <pthread.h>
#include <signal.h>

#include "config.h"
#include "daemon.h"
#include "overlay.h"

void setup_main_signals() {
    struct sigaction start_display;
    start_display.sa_handler = spawn_overlay;
    sigemptyset (&start_display.sa_mask);
    start_display.sa_flags = 0;
    sigaction(SIGUSR1, &start_display, NULL);

    // Main thread should not receive signal to terminate display thread
    sigset_t sigusr2;
    sigemptyset(&sigusr2);
    sigaddset(&sigusr2, SIGUSR2);
    pthread_sigmask(SIG_BLOCK, &sigusr2, NULL);
}

int main()
{
    setup_main_signals();

    pthread_t daemon_thread;
    pthread_create(&daemon_thread, NULL, run_daemon, NULL);
    pthread_join(daemon_thread, NULL);

    return 0;
}