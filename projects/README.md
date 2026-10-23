# Embedded Systems Projects

This folder contains practical microcontroller projects. Check each sketch's header and hardware notes before wiring or uploading.

## Projects

### 1. Cement Industry Pollutant Monitor
File: [`cement_pollutant_monitor.ino`](./cement_pollutant_monitor.ino)

- ESP32-C6 educational prototype using DHT11, MQ-135, and Sharp GP2Y1010AU0F dust sensor
- Serial Monitor output and optional 16x2 I2C LCD
- Dust warning threshold (demo only; sensor estimates require calibration)

### 2. Standalone Flood Monitoring System
File: [`iot_based_flood_monitoring.ino`](./iot_based_flood_monitoring.ino)

- Local water/flood detection using the FS-IR02 sensor and the original one-second stability filter
- Relay output, non-blocking buzzer alarm, red flood LED, and blue normal-status LED
- Serial Monitor status messages at 115200 baud
- No Wi-Fi, Telegram, bot token, or network connection required

## Flood monitor hardware assumptions

The sketch preserves the original pin assignments (sensor GPIO 23, relay GPIO 25, buzzer GPIO 15, blue LED GPIO 21, red LED GPIO 17) and assumes HIGH means water detected and LOW means dry. It also assumes the relay should be driven HIGH during a flood and LOW when dry. Verify these assumptions against your exact board, sensor, relay module, and load before use. Relay modules may be active-low, and GPIO availability varies by ESP32 model.

Use an appropriate driver circuit for pumps/valves and never power a load directly from an ESP32 GPIO. Test the outputs without a connected pump or valve first. This is an educational prototype, not a certified flood-protection or safety system.

## General safety notes

- Verify GPIO assignments, voltage limits, sensor logic, and relay polarity before powering connected hardware.
- Keep credentials and API tokens out of version control.
- Sensor estimates and thresholds in educational prototypes require validation and calibration before practical interpretation.
