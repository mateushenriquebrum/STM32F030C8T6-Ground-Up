#include "command.h"
#include <stddef.h>
#include <stdbool.h>

static bool strings_equal(const char *a, const char *b)
{
    if (a == NULL || b == NULL) {
        return false;
    }

    while (*a != '\0' && *b != '\0') {
        if (*a != *b) {
            return false;
        }

        a++;
        b++;
    }

    return *a == *b;
}

Command parse_command(const char *line)
{
    if (strings_equal(line, "LED ON")) {
        return LED_ON;
    }

    if (strings_equal(line, "LED OFF")) {
        return LED_OFF;
    }

    if (strings_equal(line, "LED TOGGLE")) {
        return LED_TOGGLE;
    }

    if (strings_equal(line, "HELP")) {
        return HELP;
    }

    return INVALID;
}
