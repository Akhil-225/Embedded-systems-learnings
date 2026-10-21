/*
  Cement Industry Pollutant Monitor
  Board: ESP32-C6
  Sensors: DHT11, MQ-135, Sharp GP2Y1010AU0F (if these match your hardware)
  Output: Serial Monitor; optional 16x2 I2C LCD

  IMPORTANT:
  - Verify every GPIO against your exact ESP32-C6 board.
  - Never feed more than the ESP32-C6 ADC's permitted voltage into an input.
  - This is an educational prototype, not a certified industrial safety instrument.
  - MQ-135 readings below are raw sensor response/voltage, NOT calibrated CO2 ppm.
  - Dust density is only an indicative estimate until calibrated against a reference meter.
*/

#include <Arduino.h>
#include <DHT.h>
#include <math.h>

// -------- Pin configuration: verify for your board and wiring --------
#define DHT_PIN       4
#define DHT_TYPE      DHT11
#define MQ135_PIN     0
#define DUST_PIN      1
#define DUST_LED_PIN  3

// Set to 1 only if you have a compatible I2C 16x2 LCD connected.
#define USE_I2C_LCD 0

#if USE_I2C_LCD
  #include <Wire.h>
  #include <LiquidCrystal_I2C.h>
  #define LCD_SDA 6
  #define LCD_SCL 7
  #define LCD_ADDRESS 0x27
  LiquidCrystal_I2C lcd(LCD_ADDRESS, 16, 2);
#endif

DHT dht(DHT_PIN, DHT_TYPE);

// -------- Adjustable parameters; calibrate for your hardware --------
const uint8_t DUST_SAMPLES = 10;
const uint8_t MQ_SAMPLES = 10;
const float DUST_BASELINE_V = 0.90f;
const float DUST_SENSITIVITY_V_PER_MG_M3 = 5.0f;
const float DUST_WARNING_MG_M3 = 0.15f;  // Demo threshold only; not an exposure limit
const float ADC_DIVIDER_RATIO = 1.0f;    // Set >1 only if your actual voltage divider requires it
const unsigned long READ_INTERVAL_MS = 2000;
const unsigned long LCD_PAGE_INTERVAL_MS = 3000;

unsigned long lastReadMs = 0;
unsigned long lastLcdPageMs = 0;
uint8_t lcdPage = 0;

float temperatureC = NAN;
float humidityPct = NAN;
float dustVoltage = NAN;
float dustEstimateMgM3 = NAN;
float mq135RawAverage = NAN;
float mq135Voltage = NAN;
bool dustWarning = false;

float readAverageAdcVoltage(uint8_t pin, uint8_t samples) {
  uint32_t totalMv = 0;
  for (uint8_t i = 0; i < samples; i++) {
    totalMv += analogReadMilliVolts(pin);
    delay(5);
  }
  return ((float)totalMv / samples / 1000.0f) * ADC_DIVIDER_RATIO;
}

// Sharp GP2Y1010AU0F timing: LED on, sample near 280 us, then allow cycle to finish.
// This assumes the sensor LED is driven through a suitable transistor/driver circuit.
float readDustSensorVoltage() {
  digitalWrite(DUST_LED_PIN, LOW);
  delayMicroseconds(280);
  uint32_t millivolts = analogReadMilliVolts(DUST_PIN);
  delayMicroseconds(40);
  digitalWrite(DUST_LED_PIN, HIGH);
  delayMicroseconds(9680);
  return ((float)millivolts / 1000.0f) * ADC_DIVIDER_RATIO;
}

float readDustAverageVoltage() {
  float total = 0.0f;
  for (uint8_t i = 0; i < DUST_SAMPLES; i++) {
    total += readDustSensorVoltage();
  }
  return total / DUST_SAMPLES;
}

float estimateDustMgM3(float voltage) {
  // Simplified indicative estimate. Sensor-specific calibration is required.
  float estimate = (voltage - DUST_BASELINE_V) / DUST_SENSITIVITY_V_PER_MG_M3;
  return estimate < 0.0f ? 0.0f : estimate;
}

void readSensors() {
  float newTemp = dht.readTemperature();
  float newHumidity = dht.readHumidity();

  if (!isnan(newTemp)) temperatureC = newTemp;
  if (!isnan(newHumidity)) humidityPct = newHumidity;

  mq135RawAverage = 0.0f;
  for (uint8_t i = 0; i < MQ_SAMPLES; i++) {
    mq135RawAverage += analogRead(MQ135_PIN);
    delay(5);
  }
  mq135RawAverage /= MQ_SAMPLES;
  mq135Voltage = readAverageAdcVoltage(MQ135_PIN, MQ_SAMPLES);

  dustVoltage = readDustAverageVoltage();
  dustEstimateMgM3 = estimateDustMgM3(dustVoltage);
  dustWarning = dustEstimateMgM3 >= DUST_WARNING_MG_M3;
}

void printReadings() {
  Serial.println();
  Serial.println(F("========== Cement Environment Monitor =========="));

  Serial.print(F("Temperature: "));
  if (isnan(temperatureC)) Serial.println(F("Sensor read error"));
  else { Serial.print(temperatureC, 1); Serial.println(F(" C")); }

  Serial.print(F("Humidity: "));
  if (isnan(humidityPct)) Serial.println(F("Sensor read error"));
  else { Serial.print(humidityPct, 1); Serial.println(F(" %")); }

  Serial.print(F("MQ-135 ADC average: "));
  Serial.println(mq135RawAverage, 0);
  Serial.print(F("MQ-135 approximate input voltage: "));
  Serial.print(mq135Voltage, 3);
  Serial.println(F(" V (raw response; not CO2 ppm)"));

  Serial.print(F("Dust sensor output voltage: "));
  Serial.print(dustVoltage, 3);
  Serial.println(F(" V"));

  Serial.print(F("Indicative dust estimate: "));
  Serial.print(dustEstimateMgM3, 3);
  Serial.println(F(" mg/m^3 (uncalibrated)"));

  Serial.print(F("Dust status: "));
  Serial.println(dustWarning ? F("WARNING - threshold exceeded") : F("Below demo threshold"));
  Serial.println(F("================================================"));
}

#if USE_I2C_LCD
void updateLcd() {
  if (millis() - lastLcdPageMs < LCD_PAGE_INTERVAL_MS) return;
  lastLcdPageMs = millis();
  lcdPage = (lcdPage + 1) % 3;
  lcd.clear();

  if (lcdPage == 0) {
    lcd.setCursor(0, 0);
    lcd.print(F("Temp: "));
    if (isnan(temperatureC)) lcd.print(F("--"));
    else { lcd.print(temperatureC, 1); lcd.print((char)223); lcd.print(F("C")); }
    lcd.setCursor(0, 1);
    lcd.print(F("Humidity: "));
    if (isnan(humidityPct)) lcd.print(F("--"));
    else { lcd.print(humidityPct, 0); lcd.print(F("%")); }
  } else if (lcdPage == 1) {
    lcd.setCursor(0, 0);
    lcd.print(F("Dust est mg/m3"));
    lcd.setCursor(0, 1);
    lcd.print(dustEstimateMgM3, 3);
  } else {
    lcd.setCursor(0, 0);
    lcd.print(F("MQ135 raw: "));
    lcd.print((int)mq135RawAverage);
    lcd.setCursor(0, 1);
    lcd.print(dustWarning ? F("DUST: WARNING") : F("Dust: normal"));
  }
}
#endif

void setup() {
  Serial.begin(115200);
  delay(300);

  pinMode(DUST_LED_PIN, OUTPUT);
  digitalWrite(DUST_LED_PIN, HIGH);

  analogReadResolution(12);
  analogSetPinAttenuation(MQ135_PIN, ADC_11db);
  analogSetPinAttenuation(DUST_PIN, ADC_11db);

  dht.begin();

#if USE_I2C_LCD
  Wire.begin(LCD_SDA, LCD_SCL);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(F("Cement Monitor"));
  lcd.setCursor(0, 1);
  lcd.print(F("Starting..."));
#endif

  Serial.println();
  Serial.println(F("Cement Industry Pollutant Monitor starting."));
  Serial.println(F("Allow the dust sensor to stabilize before interpreting readings."));
  delay(10000);
  lastReadMs = millis() - READ_INTERVAL_MS;
}

void loop() {
  if (millis() - lastReadMs >= READ_INTERVAL_MS) {
    lastReadMs = millis();
    readSensors();
    printReadings();
  }

#if USE_I2C_LCD
  updateLcd();
#endif
}
