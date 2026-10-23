#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// --- WIFI & TELEGRAM CONFIGURATION ---
// Replace these placeholders locally. Never commit real credentials.
const char* ssid = "***********";
const char* password = "*********";
#define BOTtoken "***********************************"
#define CHAT_ID "**********"

// --- PIN DEFINITIONS ---
const int sensorPin  = 23;   // FS-IR02 Water Sensor input
const int relayPin   = 25;   // Relay for Water Valve and Pump
const int buzzerPin  = 15;   // Active Buzzer pin
const int blueLedPin = 21;   // Independent LED for remote control (GPIO 21)
const int redLedPin  = 17;   // Status Indicator LED for Flood

// --- SYSTEM VARIABLES ---
bool isFloodDetected = false;
bool blueLedStatus = false;
unsigned long lastBotCheck;
int botRequestDelay = 1000;

// Timing for non-blocking buzzer
unsigned long lastBuzzerToggle = 0;
bool buzzerState = false;

// --- STABILITY FILTER VARIABLES (TUNED) ---
unsigned long lastStateChangeTime = 0;
const unsigned long stabilityDelay = 1000;
bool lastValidSensorState = LOW;

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Telegram Message Handler
void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID) continue;

    String text = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Welcome " + from_name + " to Flood Guard Bot.\n\n";
      welcome += "COMMANDS:\n";
      welcome += "/status - Check water levels & hardware state\n";
      welcome += "/blueled on - Turn ON the remote LED (GPIO 21)\n";
      welcome += "/blueled off - Turn OFF the remote LED (GPIO 21)\n";
      bot.sendMessage(CHAT_ID, welcome, "");
    }

    if (text == "/blueled on") {
      blueLedStatus = true;
      digitalWrite(blueLedPin, HIGH);
      bot.sendMessage(CHAT_ID, "External LED is now ON (High)", "");
    }

    if (text == "/blueled off") {
      blueLedStatus = false;
      digitalWrite(blueLedPin, LOW);
      bot.sendMessage(CHAT_ID, "External LED is now OFF (Low)", "");
    }

    if (text == "/status") {
      String statusMsg = "--- SYSTEM REPORT ---\n";
      statusMsg += isFloodDetected ? "💧 ALERT: FLOOD DETECTED!" : "✅ STATUS: ALL DRY";
      statusMsg += "\nValve/Pump: ";
      statusMsg += (digitalRead(relayPin) == HIGH) ? "CLOSED (Safe)" : "OPEN (Normal)";
      statusMsg += "\nRed LED (GPIO 17): ";
      statusMsg += (digitalRead(redLedPin) == HIGH) ? "ON" : "OFF";
      statusMsg += "\nRemote LED (GPIO 21): ";
      statusMsg += blueLedStatus ? "ON" : "OFF";
      bot.sendMessage(CHAT_ID, statusMsg, "");
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(sensorPin, INPUT_PULLUP);
  pinMode(relayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);

  digitalWrite(relayPin, LOW);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(blueLedPin, LOW);
  digitalWrite(redLedPin, LOW);

  WiFi.begin(ssid, password);
  // SECURITY NOTE: setInsecure() disables TLS certificate verification.
  // Configure a trusted CA certificate before using this outside a test setup.
  client.setInsecure();

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
  bot.sendMessage(CHAT_ID, "System V0.3 Online: Monitoring with 1s Stability Filter...", "");
}

void loop() {
  // Read instant physical sensor state (HIGH = Water, LOW = Dry)
  int currentRawInput = digitalRead(sensorPin);

  // Stability filter / debounce logic
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

  // Automatic flood response
  if (lastValidSensorState == HIGH) {
    digitalWrite(relayPin, HIGH);
    digitalWrite(redLedPin, HIGH);

    if (millis() - lastBuzzerToggle >= 100) {
      lastBuzzerToggle = millis();
      buzzerState = !buzzerState;
      digitalWrite(buzzerPin, buzzerState);
    }

    if (!isFloodDetected) {
      bot.sendMessage(CHAT_ID, "⚠️ ALERT: 💧 FLOOD DETECTED! Water valve closed and pump activated.", "");
      isFloodDetected = true;
    }
  } else {
    digitalWrite(relayPin, LOW);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(redLedPin, LOW);

    if (isFloodDetected) {
      bot.sendMessage(CHAT_ID, "✅ System Clear: Water level back to normal.", "");
      isFloodDetected = false;
    }
  }

  // Check Telegram commands
  if (millis() > lastBotCheck + botRequestDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastBotCheck = millis();
  }
}
