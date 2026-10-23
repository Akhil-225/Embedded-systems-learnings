# Embedded C

This folder contains C language exercises and small projects aimed at building a foundation for embedded firmware development.

## Projects

- [Circular Buffer](projects/circular_buffer/README.md) — fixed-size FIFO buffer using an array and indexes; useful for understanding UART and sensor-data queues.
- [Sensor Monitoring State Machine](projects/sensor_monitoring_state_machine/README.md) — converts simulated readings into NORMAL, WARNING, ALARM, or ERROR states.

Both projects use standard C and can be compiled on a desktop with GCC. They are software learning projects and do not require an ESP32 board.
