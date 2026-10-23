# Circular Buffer in C

A fixed-size first-in, first-out (FIFO) ring buffer implemented in standard C. It uses a fixed array and does not allocate memory dynamically, making the design suitable for small embedded applications.

## Features

- Initialize the buffer.
- Push a byte unless the buffer is full.
- Pop the oldest byte unless the buffer is empty.
- Query full, empty, and item count.
- No dynamic memory allocation.

## Build and run

With GCC installed, run this command from this directory:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror circular_buffer.c main.c -o circular_buffer
```

Run it on Linux/macOS with `./circular_buffer`, or on Windows with `circular_buffer.exe`.

## Embedded relevance

Circular buffers are commonly used to queue UART bytes, sensor samples, and data passed between interrupt handlers and main-loop code. This example is a learning implementation; concurrent access from an interrupt and main loop may require platform-specific synchronization or critical sections.

## Concepts practiced

Arrays, structures, pointers, modulo arithmetic, header files, defensive checks, and FIFO data handling.
