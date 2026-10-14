#include "line_buffer.h"

bool line_buffer_init(LineBuffer *buffer, char *storage, size_t capacity) {
    if (buffer == NULL || storage == NULL || capacity == 0U) {
        return false;
    }

    buffer->data = storage;
    buffer->capacity = capacity;
    buffer->length = 0U;
    buffer->discarding = false;
    buffer->data[0] = '\0';

    return true;
}

LineStatus line_buffer_feed(LineBuffer *buffer, char c) {
    if (buffer == NULL || buffer->data == NULL || buffer->capacity == 0U) {
        return LINE_INCOMPLETE;
    }

    /*Handle line ends*/
    if(c == '\r' || c == '\n') {
        if (buffer->discarding) {
            buffer->discarding = false;
            buffer->length = 0U;
            buffer->data[0] = '\0';
            return LINE_INCOMPLETE;
        }
        if (buffer->length == 0U) {
            return LINE_INCOMPLETE;
        }
        buffer->data[buffer->length] = '\0';
        buffer->length = 0U;
        return LINE_READY;
    }

    /*Ignore character untill the overlong line ends*/
    if (buffer->discarding) {
        return LINE_INCOMPLETE;
    }

    /*Reserve one byte for '\0'*/
    if (buffer->length >= buffer->capacity - 1U) {
        buffer->discarding = true;
        buffer->length = 0U;
        buffer->data[0] = '\0';

        return LINE_TOO_LONG;
    } 

    buffer->data[buffer->length] = c;
    buffer->length++;
    buffer->data[buffer->length] = '\0';

    return LINE_INCOMPLETE;

}
