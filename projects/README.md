# Cement Industry Pollutant Monitor

An ESP32-C6 educational prototype for observing environmental parameters relevant to a cement-industry setting.

## Features

- Temperature and humidity readings using a DHT11
- MQ-135 raw sensor response and approximate ADC voltage
- Sharp GP2Y1010AU0F dust-sensor voltage and an indicative density estimate
- Configurable dust-warning threshold
- Serial Monitor output
- 16x2 I2C LCD support

## Hardware

- ESP32-C6 development board
- DHT11 temperature/humidity sensor
- MQ-135 gas sensor module
- Sharp GP2Y1010AU0F dust sensor
- I2C 16x2 LCD
- Appropriate voltage divider and transistor/driver circuit where required by the sensor interfaces

## Software

- Arduino IDE with the ESP32 board package
- Adafruit DHT sensor library
- Adafruit Unified Sensor library
- a compatible `LiquidCrystal_I2C` library

