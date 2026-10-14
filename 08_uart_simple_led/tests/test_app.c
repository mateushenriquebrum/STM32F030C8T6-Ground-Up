#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "app.h"

typedef struct {
    bool led_on;
    char output[256];
} FakeHardware;

static void fake_set_led(bool on, void *context) {
    FakeHardware *fake = context;
    fake->led_on = on;
}

static void fake_write_text(const char *text, void *context) {
    FakeHardware *fake = context;
    int written = snprintf(fake->output, sizeof fake->output, "%s", text);
    assert(written >= 0);
    assert((size_t) written < sizeof fake->output);
}


int main(void) {
    FakeHardware fake = {
        .led_on = false,
        .output = "",
    };

    Platform platform = {
        .set_led = fake_set_led,
        .write_text = fake_write_text,
        .context = &fake
    };

    App app;
    app_init(&app, platform);

    assert(!fake.led_on);
    assert(!app.led_on);

    app_handle_line(&app, "LED ON");
    
    assert(fake.led_on);
    assert(app.led_on);
    assert(strcmp(fake.output, "LED ON\r\n") == 0);

    app_handle_line(&app, "LED OFF");
    
    assert(!fake.led_on);
    assert(!app.led_on);
    assert(strcmp(fake.output, "LED OFF\r\n") == 0);

    app_handle_line(&app, "LED TOGGLE");

    assert(fake.led_on);
    assert(app.led_on);
    assert(strcmp(fake.output, "LED ON\r\n") == 0);

    app_handle_line(&app, "LED TOGGLE");

    assert(!fake.led_on);
    assert(!app.led_on);
    assert(strcmp(fake.output, "LED OFF\r\n") == 0);

    app_handle_line(&app, "UNKNOWN");
    assert(strcmp(fake.output,"Unknown command. Type HELP.\r\n") == 0);

    return 0;
}