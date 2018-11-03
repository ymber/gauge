# Gauge

Gauge is a modular base for rendering a system monitor overlay.

## Configuration

```C
Settings_t settings = {
    1024,
    768,
    {1, 0},
    {1, 0}
};

void (*daemon_fncs[1])() = {
    update_status
};

void (*display_fncs[4])(cairo_t*) = {
    draw_background,
    draw_rings,
    draw_charts,
    date_time
};
```

Gauge is primarily configured at compile time in the `config.c` source file. Declarations for all configuration data are in `config.h`. The `settings` struct is used for global configuration variables. The `daemon_fncs` and `display_fncs` arrays determine the execution order of modules handled by the daemon and display threads.

### settings

The `settings` structure contains all global configuration options for Gauge.

* `size_x` specifies the width of Gauge's window.
* `size_y` specifies the height of Gauge's window.
* `daemon_freq` specifies the length in seconds and nanoseconds of the Gauge daemon's refresh period.
* `display_freq` specifies the time between window redraws in seconds and nanoseconds.

### daemon_fncs

`daemon_fncs` is an array of function pointers. The functions pointed to by its elements are executed on each iteration of the main daemon loop. Each module used should have one root function with a pointer in `daemon_fncs`.

### display_fncs

`display_fncs` is an array of function pointers. The functions pointed to by its elements are executed each time Gauge's window redraws. Each module used should have one root function with a pointer in `daemon_fncs`.

## Base Modules

The `data`, `display`, and `status` modules are provided with Gauge. They provide functions for other modules to get and display data.

### data

`data.h`, `data.c`

* `get_time` returns a pointer to a `tm` (broken-down time structure) with current time data for the time `get_time` was called.
* `get_cpu_stats` takes arguments `char *cpuN` and `int *stats` where `cpuN` is a cpu label in `/proc/stat` and `stats` points to an array that cpu data will be stored in. A snapshot of cpu usage data for the cpu specified by `cpuN` is read from `/proc/stat` and written to the array pointed to by `stats`. This array must have at least 10 elements as kernel versions >2.6.33 provide 10 values.
* `get_memory_usage` returns the fraction of memory in use at the time of calling. Buffers and cache are counted as free memory. Values are read from `/proc/meminfo`.
* `get_disk_stats` takes an argument `char* disk` and returns a `statfs` struct containing usage data for the partition mounted at `disk`.

### display

`display.h`, `display.c`

#### Line Charts

```C
typedef struct {
    double max;
    int x;
    int y;
    int start_x;
    int start_y;
    int point_num;
    RGBA_t rgba;
    int line_width;
} Line_Chart_t;

void line_chart(cairo_t *context, Line_Chart_t *chart, double *point_set, int reverse);
```

* `max` defines the maximum value of a point passed to `line_chart` in `point_set`.
* `x` and `y` define the dimensions of the chart in pixels.
* `start_x` and `start_y` define the coordinates of the top left corner of the chart.
* `point_num` defines the number of points space is allocated for on the chart. This number must be equal to the length of `point_set` passed to `line_chart`.
* `rgba` defines the color and transparency of the chart.
* `line_width` defines the width of the lines connecting points on the chart.

* `line_chart` takes a cairo context to draw to, a `Line_Chart_t` struct defining chart drawing data, a set of points to draw, and a flag for drawing the chart in reverse. If `reverse` evaluates to true the chart will be drawn from right to left, otherwise left to right.

#### Rings

```C
typedef struct {
    RGBA_t bg_rgba;
    RGBA_t fg_rgba;
    int x;
    int y;
    int radius;
    int thickness;
    double start_angle;
    double end_angle;
} Data_Curve_t;

void draw_ring(cairo_t *context, Data_Curve_t *arc, double coverage);
```

* `bg_rgba` and `fg_rgba` define the RGBA values of the background and foreground of the arc. The foreground is the area of the arc filled to represent resource usage.
* `x` and `y` are the coordinates of the center of the circle the arc is drawn from.
* `radius` is defines the radius of the circle the arc is drawn from.
* `thickness` is the thickness in pixels of the drawn arc.
* `start_angle` and `end_angle` define the angle subtending the arc drawn. `end_angle` must be greater than `start_angle`.

* `draw_ring` takes arguments of a cairo context to draw to, a `Data_Curve_t` struct defining arc properties, and the fraction of the arc to fill. `coverage` should be a value between 0 and 1.

### status

`status.h`, `status.c`

```C
typedef struct {
    double cpu_perc[16];
    double cpu_avg_perc;
    double cpu_avg_perc_min[60];
} Resource_Data_t;

extern Resource_Data_t system_resources;
void update_status();
```

`system_resources` should only be accessed via the `mutex_system_resources` mutex.

* `cpu_perc` holds the median utilization over the last daemon refresh period of up to 16 CPU cores (physical or logical).
* `cpu_avg_perc` holds the median utilization over the last daemon refresh period of all detected cores.
* `cpu_avg_perc_min` holds the last 60 `cpu_avg_perc` values, with the first element always being the most recent.
* `update_status` updates the values in `system_resources` with the values from the last daemon refresh period.
