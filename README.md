# Gauge

Gauge is a modular base for rendering a system monitor overlay.

## Configuration

```C
Settings_t settings = {
    1024,
    768,
    1
};

void (*call[1])(cairo_t*, Window*) = {
    draw_rings
};
```

Gauge is configured in the `config.h` header file. The `settings` struct and `call` array are used to configure global settings and module execution.

### settings

The `settings` structure contains all global configuration options for Gauge.

* `size_x` specifies the width of Gauge's window.
* `size_y` specifies the height of Gauge's window.
* `frequency` specifies in Hz the frequency at which Gauge redraws its window.

### call

`call` is an array of function pointers. The functions pointed to by its contents are executed on each redraw of Gauge's window. Each module used should have one root function with a pointer in `call`.

## Base Modules

The `data` module is provided with Gauge. It contains functions to get commonly used system information.
