#include <stdio.h>
#include "circular_buffer.h"

int main(void)
{
    CircularBuffer buffer;
    uint8_t value;

    CircularBuffer_Init(&buffer);

    puts("Pushing values 10, 20, 30 into the buffer...");
    (void)CircularBuffer_Push(&buffer, 10U);
    (void)CircularBuffer_Push(&buffer, 20U);
    (void)CircularBuffer_Push(&buffer, 30U);

    printf("Items stored: %zu\n", CircularBuffer_Count(&buffer));

    puts("Reading values in FIFO order:");
    while (CircularBuffer_Pop(&buffer, &value)) {
        printf("%u\n", (unsigned int)value);
    }

    printf("Buffer empty: %s\n",
           CircularBuffer_IsEmpty(&buffer) ? "yes" : "no");
    return 0;
}
