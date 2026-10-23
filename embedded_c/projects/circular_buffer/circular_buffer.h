#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define CIRCULAR_BUFFER_CAPACITY 8U

typedef struct {
    uint8_t data[CIRCULAR_BUFFER_CAPACITY];
    size_t head;
    size_t tail;
    size_t count;
} CircularBuffer;

void CircularBuffer_Init(CircularBuffer *buffer);
bool CircularBuffer_Push(CircularBuffer *buffer, uint8_t value);
bool CircularBuffer_Pop(CircularBuffer *buffer, uint8_t *value);
bool CircularBuffer_IsEmpty(const CircularBuffer *buffer);
bool CircularBuffer_IsFull(const CircularBuffer *buffer);
size_t CircularBuffer_Count(const CircularBuffer *buffer);

#endif
