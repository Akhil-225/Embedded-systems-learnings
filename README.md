# Embedded Systems

This repository contains my embedded systems learning path, including Embedded C practice, microcontroller experiments, and practical projects using boards such as ESP32 and Arduino.

## Repository structure

- `embedded_c/` — Embedded C practice
- [`projects/`](./projects/) — Embedded systems projects and project-specific setup notes
  - [Cement Industry Pollutant Monitor](./projects/cement_pollutant_monitor.ino)
  - [Standalone Flood Monitoring System](./projects/iot_based_flood_monitoring.ino)

## Prerequisites

- Arduino IDE or PlatformIO
- USB drivers for the development board
- Development boards, jumper wires, breadboard, and appropriate electronic components
- Project-specific libraries listed in the documentation under `projects/`

## Before uploading

Review project documentation and verify pin assignments, voltage limits, sensor logic, and relay polarity for your exact hardware. Use suitable driver circuitry for pumps and valves; never power loads directly from an ESP32 GPIO. These projects are educational prototypes, not certified safety systems.

## Maintainer

- Akhil Challa
- gmail:cakhil_be24@thapar.edu

