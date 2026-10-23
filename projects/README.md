# Embedded Systems Projects

This folder contains practical microcontroller projects. Check each sketch's header and hardware notes before wiring or uploading.

## Projects

### 1. Cement Industry Pollutant Monitor
File: [`cement_pollutant_monitor.ino`](./cement_pollutant_monitor.ino)

- ESP32-C6 educational prototype using DHT11, MQ-135, and Sharp GP2Y1010AU0F dust sensor
- Serial Monitor output and optional 16x2 I2C LCD
- Dust warning threshold (demo only; sensor estimates require calibration)

### 2. IoT-Based Flood Monitoring System
File: [`iot_based_flood_monitoring.ino`](./iot_based_flood_monitoring.ino)

- ESP32-based water/flood detection with a one-second stability filter
- Relay output for a valve/pump, buzzer, and status LEDs
- Telegram bot commands for status and remote LED control
- Requires Wi-Fi and Telegram bot configuration before use

## Safety and setup notes

- Confirm the GPIO assignments and relay logic for your exact board and hardware before powering connected loads.
- Never commit real Wi-Fi passwords, Telegram bot tokens, chat IDs, or other credentials.
- The flood monitor currently uses `WiFiClientSecure::setInsecure()`, which disables TLS certificate verification. Replace this with a trusted CA certificate before relying on Telegram communication in a real deployment.
- These are educational prototypes, not certified safety or environmental monitoring systems.
