#ifndef APP_H
#define APP_H

#include <stdbool.h>

typedef struct {
    void (*set_led) (bool on, void *context);
    void (*write_text) (const char *text, void *context);
    void *context;
} Platform;

typedef struct {
    Platform platform;
    bool led_on;
} App;

void app_init(App *app, Platform platform);
void app_handle_line(App *app, const char *line);

#endif