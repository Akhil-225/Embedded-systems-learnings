# Embedded Systems

This repository contains my embedded systems learning path, including Embedded C practice, microcontroller experiments, and practical projects using boards such as ESP32 and Arduino.

## Repository structure

- `embedded_c/` — Embedded C practice
- [`projects/`](./projects/) — Embedded systems projects and project-specific setup notes
  - [Cement Industry Pollutant Monitor](./projects/cement_pollutant_monitor.ino)
  - [IoT-Based Flood Monitoring System](./projects/iot_based_flood_monitoring.ino)

## Prerequisites

- Arduino IDE or PlatformIO
- USB drivers for the development board
- Development boards, jumper wires, breadboard, and appropriate electronic components
- Project-specific libraries listed in the documentation under `projects/`

## Before uploading

Review the project documentation and confirm pin assignments, voltage limits, and relay/load wiring for your exact hardware. Keep Wi-Fi credentials and API/bot tokens out of version control. The flood-monitoring sketch currently disables TLS certificate verification for Telegram; do not rely on that configuration for production use.

## Maintainer

Akhil Challa
