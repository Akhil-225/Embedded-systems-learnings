#include "circular_buffer.h"

void CircularBuffer_Init(CircularBuffer *buffer)
{
    if (buffer == NULL) {
        return;
    }

    buffer->head = 0U;
    buffer->tail = 0U;
    buffer->count = 0U;
}

bool CircularBuffer_Push(CircularBuffer *buffer, uint8_t value)
{
    if ((buffer == NULL) || CircularBuffer_IsFull(buffer)) {
        return false;
    }

    buffer->data[buffer->head] = value;
    buffer->head = (buffer->head + 1U) % CIRCULAR_BUFFER_CAPACITY;
    buffer->count++;
    return true;
}

bool CircularBuffer_Pop(CircularBuffer *buffer, uint8_t *value)
{
    if ((buffer == NULL) || (value == NULL) || CircularBuffer_IsEmpty(buffer)) {
        return false;
    }

    *value = buffer->data[buffer->tail];
    buffer->tail = (buffer->tail + 1U) % CIRCULAR_BUFFER_CAPACITY;
    buffer->count--;
    return true;
}

bool CircularBuffer_IsEmpty(const CircularBuffer *buffer)
{
    return (buffer == NULL) || (buffer->count == 0U);
}

bool CircularBuffer_IsFull(const CircularBuffer *buffer)
{
    return (buffer != NULL) && (buffer->count >= CIRCULAR_BUFFER_CAPACITY);
}

size_t CircularBuffer_Count(const CircularBuffer *buffer)
{
    return (buffer == NULL) ? 0U : buffer->count;
}
