#include "app.h"
#include "gpio.h"
#include "command.h"
#include "line_buffer.h"
#include "usart1.h"

#include <stddef.h>

static void platform_set_led(bool on, void *context) {
    (void) context;
    gpio_set_led(on);
}

static void platform_write_text(const char *text, void *context) {
    (void) context;
    usart1_write_string(text);
}
//picocom --echo --imap lfcrlf -b 115200 /dev/ttyUSB0
int main(void) {
    gpio_init();
    usart1_init();

    Platform platform = {
        .set_led = platform_set_led,
        .write_text = platform_write_text,
        .context = NULL
    };

    App app;
    app_init(&app, platform);

    char line_storage[64];
    LineBuffer line_buffer;

    if(!line_buffer_init(&line_buffer, line_storage, sizeof line_storage)) {
        for(;;) {}
    }

    usart1_write_string("\r\nUART LED controller ready\r\n");
    for(;;) {
        char c = usart1_read_character();

        LineStatus status = line_buffer_feed(&line_buffer, c);

        if (status == LINE_READY) {
            app_handle_line(&app, line_storage);
        } else if(status == LINE_TOO_LONG) {
            usart1_write_string("\r\nError: command too long\r\n");
        }
    }
}