/*
  IoT-Based Flood Monitoring System (standalone)
  Board: ESP32 (verify GPIO availability on your exact board)
  Sensor: FS-IR02 water sensor
  Outputs: relay, active buzzer, blue status LED, red flood LED
  Interface: Serial Monitor at 115200 baud

#include <Arduino.h>

// Pin assignments — verify against your exact ESP32 board.
const int sensorPin  = 23;  // FS-IR02 water sensor input
const int relayPin   = 25;  // Relay for valve/pump control
const int buzzerPin  = 15;  // Active buzzer
const int blueLedPin = 21;  // Normal/dry status indicator
const int redLedPin  = 17;  // Flood status indicator

// Preserve the original one-second stability filter.
const unsigned long stabilityDelay = 1000;
unsigned long lastStateChangeTime = 0;
bool lastValidSensorState = LOW;  // LOW = dry, HIGH = water (assumed)

// Non-blocking buzzer timing.
const unsigned long buzzerToggleInterval = 100;
unsigned long lastBuzzerToggle = 0;
bool buzzerState = false;

bool isFloodDetected = false;

void printStatus(const char* event) {
  Serial.println();
  Serial.println(F("========== Flood Monitor =========="));
  Serial.print(F("Event: "));
  Serial.println(event);
  Serial.print(F("Sensor state: "));
  Serial.println(lastValidSensorState == HIGH ? F("WATER DETECTED") : F("DRY"));
  Serial.print(F("Flood status: "));
  Serial.println(isFloodDetected ? F("FLOOD ALERT") : F("NORMAL"));
  Serial.print(F("Relay output: "));
  Serial.println(digitalRead(relayPin) == HIGH ? F("HIGH (active per current configuration)") : F("LOW (inactive per current configuration)"));
  Serial.print(F("Blue LED (normal): "));
  Serial.println(digitalRead(blueLedPin) == HIGH ? F("ON") : F("OFF"));
  Serial.print(F("Red LED (flood): "));
  Serial.println(digitalRead(redLedPin) == HIGH ? F("ON") : F("OFF"));
  Serial.println(F("==================================="));
}

void applyDryState() {
  digitalWrite(relayPin, LOW);
  digitalWrite(buzzerPin, LOW);
  buzzerState = false;
  digitalWrite(redLedPin, LOW);
  digitalWrite(blueLedPin, HIGH);
}

void applyFloodState() {
  digitalWrite(relayPin, HIGH);
  digitalWrite(redLedPin, HIGH);
  digitalWrite(blueLedPin, LOW);

  // Toggle buzzer without blocking sensor monitoring.
  if (millis() - lastBuzzerToggle >= buzzerToggleInterval) {
    lastBuzzerToggle = millis();
    buzzerState = !buzzerState;
    digitalWrite(buzzerPin, buzzerState ? HIGH : LOW);
  }
}

void setup() {
  Serial.begin(115200);
  delay(200);

  pinMode(sensorPin, INPUT_PULLUP);
  pinMode(relayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);

  // Start in the original sketch's dry-state output configuration.
  digitalWrite(relayPin, LOW);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(blueLedPin, HIGH);
  digitalWrite(redLedPin, LOW);

  Serial.println();
  Serial.println(F("Standalone Flood Monitoring System starting."));
  Serial.println(F("Wi-Fi and Telegram are not used."));
  Serial.println(F("Serial Monitor baud rate: 115200."));
  Serial.println(F("Sensor assumption: HIGH = water, LOW = dry."));
  Serial.println(F("Allow the sensor to settle; verify sensor and relay logic before use."));

  // Begin in the configured dry-state output mode. The normal stability
  // filter in loop() will confirm any HIGH/water input after one second.
  isFloodDetected = false;
  applyDryState();
  printStatus("System initialized; monitoring started");
}

void loop() {
  const bool currentRawInput = digitalRead(sensorPin);

  // Stability filter: accept a state only after it remains changed
  // continuously for the configured one-second delay.
  if (currentRawInput != lastValidSensorState) {
    if (lastStateChangeTime == 0) {
      lastStateChangeTime = millis();
    }

    if (millis() - lastStateChangeTime >= stabilityDelay) {
      lastValidSensorState = currentRawInput;
      lastStateChangeTime = 0;
    }
  } else {
    lastStateChangeTime = 0;
  }

  // Local flood response — no network calls or remote commands.
  if (lastValidSensorState == HIGH) {
    applyFloodState();

    if (!isFloodDetected) {
      isFloodDetected = true;
      Serial.println(F("ALERT: Flood detected. Local alarm and relay activated."));
      printStatus("Flood detected");
    }
  } else {
    applyDryState();

    if (isFloodDetected) {
      isFloodDetected = false;
      Serial.println(F("Water level returned to dry state. Alarm cleared."));
      printStatus("System clear");
    }
  }
}
