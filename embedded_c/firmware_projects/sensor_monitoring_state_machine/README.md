# Sensor Monitoring State Machine in C

A small standard-C program that converts simulated sensor readings into explicit operating states.

## State rules

- **NORMAL:** reading below 60
- **WARNING:** reading from 60 through 79
- **ALARM:** reading of 80 or above, up to 100
- **ERROR:** sensor read failed or reading is outside 0–100

The thresholds are examples for learning, not calibrated limits for a real sensor or safety system.

## Build and run

With GCC installed, run this command from this directory:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror sensor_monitor.c main.c -o sensor_monitor
```

Run it on Linux/macOS with `./sensor_monitor`, or on Windows with `sensor_monitor.exe`.

## Embedded relevance

A state machine makes firmware behavior easier to understand, test, and extend. This example can later be adapted to process readings from an ESP32 sensor, with hardware-specific input/output code kept separate from the portable decision logic.

## Concepts practiced

Enums, functions, switch statements, threshold checks, error handling, and separating application logic into source and header files.
