#include <assert.h>
#include <string.h>

#include "line_buffer.h"

static void test_complete_line_r_ending(void) {
    char storage[32];
    LineBuffer buffer;

    assert(line_buffer_init(&buffer, storage, sizeof storage));

    const char *input = "LED ON";

    while (*input != '\0') {
        assert(line_buffer_feed(&buffer, *input) == LINE_INCOMPLETE);
        input++;
    }

    assert(line_buffer_feed(&buffer, '\r') == LINE_READY);
    assert(strcmp(storage, "LED ON") == 0);
    
}

static void test_complete_line_n_ending(void) {
    char storage[32];
    LineBuffer buffer;

    assert(line_buffer_init(&buffer, storage, sizeof storage));

    const char *input = "LED ON";

    while (*input != '\0') {
        assert(line_buffer_feed(&buffer, *input) == LINE_INCOMPLETE);
        input++;
    }

    assert(line_buffer_feed(&buffer, '\n') == LINE_READY);
    assert(strcmp(storage, "LED ON") == 0);
    
}

static void test_empty_line(void) {
    char storage[32];
    LineBuffer buffer;

    assert(line_buffer_init(&buffer, storage, sizeof storage));

    assert(line_buffer_feed(&buffer, '\r') == LINE_INCOMPLETE);
    assert(line_buffer_feed(&buffer, '\n') == LINE_INCOMPLETE);
}

static void test_back_to_back_lines(void) {
    char storage[32];
    LineBuffer buffer;
   
    assert(line_buffer_init(&buffer, storage, sizeof storage));
   
    line_buffer_feed(&buffer, 'O');
    line_buffer_feed(&buffer, 'N');
    assert(line_buffer_feed(&buffer, '\r') == LINE_READY);
    assert(strcmp(storage, "ON") == 0);

    line_buffer_feed(&buffer, 'O');
    line_buffer_feed(&buffer, 'F');
    line_buffer_feed(&buffer, 'F');
    assert(line_buffer_feed(&buffer, '\r') == LINE_READY);
    assert(strcmp(storage, "OFF") == 0);
}

static void test_overflow(void) {
    char storage[2];
    LineBuffer buffer;
   
    assert(line_buffer_init(&buffer, storage, sizeof storage));
   
    assert(line_buffer_feed(&buffer, 'A') == LINE_INCOMPLETE);
    assert(line_buffer_feed(&buffer, 'B') == LINE_TOO_LONG);
    assert(line_buffer_feed(&buffer, 'C') == LINE_INCOMPLETE);
    assert(line_buffer_feed(&buffer, '\n') == LINE_INCOMPLETE);
    assert(line_buffer_feed(&buffer, 'Z') == LINE_INCOMPLETE);
    assert(line_buffer_feed(&buffer, '\n') == LINE_READY);
    assert(strcmp(storage, "Z") == 0);
}

int main(void) {
    test_complete_line_r_ending();
    test_complete_line_n_ending();
    test_empty_line();
    test_back_to_back_lines();
    test_overflow();
    return 0;
}