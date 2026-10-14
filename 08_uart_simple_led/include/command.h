#ifndef COMMAND_H
#define COMMAND_H

typedef enum {
    INVALID,
    LED_ON,
    LED_OFF, 
    LED_TOGGLE,
    HELP
} Command;

Command parse_command(const char *line);

#endif