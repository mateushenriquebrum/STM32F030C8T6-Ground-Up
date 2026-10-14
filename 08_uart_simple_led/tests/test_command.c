#include <assert.h>
#include <stddef.h>

#include "command.h"

int main(void) {
    assert(parse_command("LED ON") == LED_ON);
    assert(parse_command("LED OFF") == LED_OFF);
    assert(parse_command("LED TOGGLE") == LED_TOGGLE);
    assert(parse_command("HELP") == HELP);
    assert(parse_command("UNKNOWN") == INVALID);
    assert(parse_command("") == INVALID);
    assert(parse_command(NULL) == INVALID);
}