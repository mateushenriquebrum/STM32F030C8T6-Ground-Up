#include "app.h"
#include "command.h"

void app_init(App *app, Platform platform) {
    if (app == 0) {
        return;
    }

    app->platform = platform;
    app->led_on = false;

    if(app->platform.set_led != 0) {
        app->platform.set_led(false, app->platform.context);
    }
}

void app_handle_line(App *app, const char *line){
    if(app == 0 || line == 0) {
        return;
    }

    Command command = parse_command(line);

    switch (command){
    
        case LED_ON:
            app->led_on = true;
            if(app->platform.set_led != 0) {
                app->platform.set_led(app->led_on, app->platform.context);
            }
            if(app->platform.write_text != 0) {
                const char *message = "LED ON\r\n";
                app->platform.write_text(message, app->platform.context);
            }
            break;
        
        case LED_OFF:
            app->led_on = false;
            if(app->platform.set_led != 0) {
                app->platform.set_led(app->led_on, app->platform.context);
            }
            if(app->platform.write_text != 0) {
                const char *message = "LED OFF\r\n";
                app->platform.write_text(message, app->platform.context);
            }
            break;
        
        case LED_TOGGLE:
            app->led_on = !app->led_on;
            if(app->platform.set_led != 0) {
                app->platform.set_led(app->led_on, app->platform.context);
            }
            if(app->platform.write_text != 0) {
                const char *message = app->led_on ? "LED ON\r\n" : "LED OFF\r\n";
                app->platform.write_text(message, app->platform.context);
            }
            break;

        case HELP:
            if(app->platform.write_text != 0) {
                const char *message = "Commands:\r\nLED ON\r\nLED OFF\r\nLED TOGGLE\r\nHELP\r\n";
                app->platform.write_text(message, app->platform.context);
            }
            break;          

       case INVALID:
       default:
            if(app->platform.write_text != 0) {
                const char *message = "Unknown command. Type HELP\r\n";
                app->platform.write_text(message, app->platform.context);
            }
            break;          

    }
}
