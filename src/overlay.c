#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <bits/stdint-uintn.h>
#include <pthread.h>

#include <xcb/xcb.h>
#include <cairo.h>
#include <cairo-xcb.h>

#include "overlay.h"
#include "config.h"

xcb_visualtype_t *find_visual(xcb_connection_t *connection, xcb_visualid_t visual)
{
    xcb_screen_iterator_t screen_itr = xcb_setup_roots_iterator(xcb_get_setup(connection));

    for(; screen_itr.rem; xcb_screen_next(&screen_itr)) {
        xcb_depth_iterator_t depth_itr = xcb_screen_allowed_depths_iterator(screen_itr.data);
        for(; depth_itr.rem; xcb_depth_next(&depth_itr)) {
            xcb_visualtype_iterator_t visual_itr = xcb_depth_visuals_iterator(depth_itr.data);
            for(; visual_itr.rem; xcb_visualtype_next(&visual_itr))
                if(visual == visual_itr.data->visual_id) {
                    return visual_itr.data;
                }
        }
    }

    return NULL;
}

Window create_window()
{
    Window window_data;
    window_data.connection = xcb_connect(NULL, NULL);

    const xcb_setup_t *setup = xcb_get_setup(window_data.connection);
    xcb_screen_iterator_t itr = xcb_setup_roots_iterator(setup);
    window_data.screen = itr.data;
    window_data.visual = find_visual(window_data.connection, window_data.screen->root_visual);
    window_data.drawable = xcb_generate_id(window_data.connection);

    uint32_t mask = XCB_CW_OVERRIDE_REDIRECT;
    uint32_t values [1] = {1};
    xcb_create_window(window_data.connection, window_data.screen->root_depth, window_data.drawable,
                      window_data.screen->root, 0, 0, settings.size_x, settings.size_y, 0,
                      XCB_WINDOW_CLASS_INPUT_OUTPUT, window_data.screen->root_visual, mask, values);
    xcb_map_window(window_data.connection, window_data.drawable);
    xcb_flush(window_data.connection);

    return window_data;
}

int display_active;

void spawn_overlay() {
    if(!display_active) {
        display_active = 1;
        pthread_t display_thread;
        pthread_create(&display_thread, NULL, run_overlay, NULL);
    }
}

void setup_display_signals() {
    struct sigaction stop_display;
    stop_display.sa_handler = signal_overlay_off;
    sigemptyset (&stop_display.sa_mask);
    stop_display.sa_flags = 0;
    sigaction(SIGUSR2, &stop_display, NULL);

    // Display thread should not receive signal to spawn display thread
    sigset_t sigusr1;
    sigemptyset(&sigusr1);
    sigaddset(&sigusr1, SIGUSR1);
    pthread_sigmask(SIG_BLOCK, &sigusr1, NULL);

    // Display thread must receive signal to close display so nanosleep is interrupted if necessary
    sigset_t sigusr2;
    sigemptyset(&sigusr2);
    sigaddset(&sigusr2, SIGUSR2);
    pthread_sigmask(SIG_UNBLOCK, &sigusr2, NULL);
}

void *run_overlay() {
    setup_display_signals();

    Window window = create_window();
    cairo_surface_t *surface = cairo_xcb_surface_create(window.connection, window.drawable,
                                     window.visual, settings.size_x, settings.size_y);
    cairo_t *context = cairo_create(surface);

    while(display_active) {
        cairo_save(context);
        cairo_set_operator(context, CAIRO_OPERATOR_CLEAR);
        cairo_paint(context);
        cairo_restore(context);
        for(unsigned int i = 0; i < sizeof(display_fncs) / sizeof(display_fncs[0]); ++i) {
            (*display_fncs[i])(context);
        }
        xcb_flush(window.connection);
        nanosleep(&settings.display_freq, NULL);
    }
    cairo_destroy(context);
    cairo_device_finish(cairo_surface_get_device(surface));
    cairo_surface_destroy(surface);
    xcb_disconnect(window.connection);
    return NULL;
}

void signal_overlay_off() {
    display_active = 0;
}
