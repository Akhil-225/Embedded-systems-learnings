# Cement Industry Pollutant Monitor

An ESP32-C6 educational prototype for observing environmental parameters relevant to a cement-industry setting.

## Features

- Temperature and humidity readings using a DHT11
- MQ-135 raw sensor response and approximate ADC voltage
- Sharp GP2Y1010AU0F dust-sensor voltage and an indicative density estimate, if that is the sensor used
- Configurable dust-warning threshold
- Serial Monitor output
- Optional 16x2 I2C LCD support

## Hardware (verify exact models before wiring)

- ESP32-C6 development board
- DHT11 temperature/humidity sensor
- MQ-135 gas sensor module
- Sharp GP2Y1010AU0F dust sensor
- Optional I2C 16x2 LCD
- Appropriate voltage divider and transistor/driver circuit where required by the sensor interfaces

## Software

- Arduino IDE with the ESP32 board package
- Adafruit DHT sensor library
- Adafruit Unified Sensor library
- If enabling the LCD option, a compatible `LiquidCrystal_I2C` library

## Important limitations and safety

- This is a prototype, not a certified industrial safety or occupational-exposure monitor.
- The dust estimate is uncalibrated and must be compared with a reference instrument before meaningful use.
- The MQ-135 output is reported as raw response/voltage; it is **not** a calibrated CO2 or other gas concentration.
- Verify the pinout of the exact ESP32-C6 board. Do not connect sensor outputs above the ESP32-C6 ADC's permitted input voltage. Use a suitable voltage divider when necessary.
- The dust sensor's LED must be driven using an appropriate circuit, not directly from a GPIO if its current requirements exceed the pin's rating.
- LCD support is disabled by default (`USE_I2C_LCD 0`). Confirm your display's I2C address and SDA/SCL wiring before enabling it.
- No Wi-Fi, Firebase credentials, passwords, or authentication tokens are needed for this standalone version.
