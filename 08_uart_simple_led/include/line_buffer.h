#ifndef LINE_BUFFER_H
#define LINE_BUFFER_H
#include <stdbool.h>
#include <stddef.h>

typedef enum {
    LINE_INCOMPLETE,
    LINE_READY,
    LINE_TOO_LONG
} LineStatus;

typedef struct {
    char *data;
    size_t capacity;
    size_t length;
    bool discarding;
} LineBuffer;

bool line_buffer_init(LineBuffer *buffer, char *storage, size_t capacity);

LineStatus line_buffer_feed(LineBuffer *buffer, char c);

#endif