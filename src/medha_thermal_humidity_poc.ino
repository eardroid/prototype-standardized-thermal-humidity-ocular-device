/*
  Prototype of Standardized Thermal/Humidity Ocular Device for Dry Eye Therapy
  ESP32-S3 / Arduino sketch

  Bench-test controller only. This sketch is not a medical-device controller.
  The high-current heater and atomizer paths are disabled by default until
  their power stages, loads, heatsinking, and safety hardware are verified.
*/

#include <Arduino.h>
#include <DHT.h>

constexpr uint8_t DHT_PIN = 17;
constexpr uint8_t BUZZER_PIN = 4;
constexpr uint8_t SWITCH_PIN = 15;       // active LOW, INPUT_PULLUP
constexpr uint8_t HEATER_PIN = 18;
constexpr uint8_t ATOMIZER_PIN = 19;

constexpr uint8_t DHT_TYPE = DHT11;
constexpr float TEMP_WARNING_C = 35.0f;  // demonstration warning only
constexpr float TEMP_ALARM_C = 40.0f;    // demonstration interlock only
constexpr unsigned long SENSOR_PERIOD_MS = 2000;

// Keep these false until each external power path has been checked safely.
constexpr bool HEATING_AVAILABLE = false;
constexpr bool ATOMIZER_AVAILABLE = false;

DHT dht(DHT_PIN, DHT_TYPE);

bool sessionOn = false;
bool faultLatched = false;
bool lastSwitchState = HIGH;
unsigned long lastSensorRead = 0;

void allOutputsOff() {
  digitalWrite(HEATER_PIN, LOW);
  digitalWrite(ATOMIZER_PIN, LOW);
}

void beep(uint8_t count, unsigned int durationMs) {
  for (uint8_t i = 0; i < count; ++i) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(durationMs);
    digitalWrite(BUZZER_PIN, LOW);
    if (i + 1 < count) delay(120);
  }
}

void printStatus(float temperatureC, float humidityPct) {
  Serial.println("----------- STATUS ----------------");
  Serial.print("Session     : ");
  Serial.println(sessionOn ? "ON" : "OFF");
  Serial.print("Temperature : ");
  Serial.print(temperatureC, 1);
  Serial.println(" C");
  Serial.print("Humidity    : ");
  Serial.print(humidityPct, 1);
  Serial.println(" %");
  Serial.print("Heater path : ");
  Serial.println(HEATING_AVAILABLE ? "ENABLED" : "DISABLED (bench default)");
  Serial.print("Mist path   : ");
  Serial.println(ATOMIZER_AVAILABLE ? "ENABLED" : "DISABLED (bench default)");
  Serial.println("----------------------------------");
}

void setup() {
  Serial.begin(115200);

  pinMode(SWITCH_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(HEATER_PIN, OUTPUT);
  pinMode(ATOMIZER_PIN, OUTPUT);
  allOutputsOff();
  digitalWrite(BUZZER_PIN, LOW);

  dht.begin();

  Serial.println();
  Serial.println("======================================");
  Serial.println("STANDARDIZED THERMAL/HUMIDITY OCULAR DEVICE POC");
  Serial.println("ESP32-S3 CONTROLLER ONLINE");
  Serial.println("======================================");
  Serial.println("Software interlocks are for bench testing only.");
  beep(1, 120);
}

void loop() {
  const bool switchState = digitalRead(SWITCH_PIN);

  if (lastSwitchState == HIGH && switchState == LOW) {
    sessionOn = true;
    faultLatched = false;
    Serial.println("SESSION: ON");
    beep(1, 160);
  } else if (lastSwitchState == LOW && switchState == HIGH) {
    sessionOn = false;
    faultLatched = false;
    allOutputsOff();
    Serial.println("SESSION: OFF");
    beep(2, 100);
  }
  lastSwitchState = switchState;

  if (millis() - lastSensorRead < SENSOR_PERIOD_MS) {
    delay(10);
    return;
  }
  lastSensorRead = millis();

  const float humidityPct = dht.readHumidity();
  const float temperatureC = dht.readTemperature();

  if (isnan(humidityPct) || isnan(temperatureC)) {
    faultLatched = true;
    sessionOn = false;
    allOutputsOff();
    Serial.println("FAULT: DHT11 reading invalid; outputs disabled.");
    beep(3, 90);
    return;
  }

  printStatus(temperatureC, humidityPct);

  if (temperatureC >= TEMP_ALARM_C) {
    faultLatched = true;
    sessionOn = false;
    allOutputsOff();
    Serial.println("FAULT: demonstration temperature alarm; outputs disabled.");
    beep(5, 70);
    return;
  }

  if (temperatureC >= TEMP_WARNING_C) {
    Serial.println("WARNING: demonstration temperature warning.");
  }

  if (!sessionOn || faultLatched) {
    allOutputsOff();
    return;
  }

  // These are intentionally gated by compile-time availability flags.
  digitalWrite(HEATER_PIN, HEATING_AVAILABLE ? HIGH : LOW);
  digitalWrite(ATOMIZER_PIN, ATOMIZER_AVAILABLE ? HIGH : LOW);
}
