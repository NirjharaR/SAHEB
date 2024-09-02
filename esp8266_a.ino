#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ACS712.h>

// Blynk authentication token, Wi-Fi credentials
char auth[] = "YourAuthToken";
char ssid[] = "YourSSID";
char pass[] = "YourPassword";

// Pin definitions for the 6 devices
const int device1 = D1;
const int device2 = D2;
const int device3 = D3;
const int device4 = D4;
const int device5 = D5;
const int device6 = D6;

// Sensor pin definitions
const int currentSensorPin = A0;
const int flameSensorPin = D7;
const int smokeSensorPin = A1;

// Initialize ACS712 sensor (20A variant)
ACS712 currentSensor(ACS712_20A, currentSensorPin);

void setup()
{
  // Begin serial communication and Blynk setup
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);

  // Initialize pins for devices
  pinMode(device1, OUTPUT);
  pinMode(device2, OUTPUT);
  pinMode(device3, OUTPUT);
  pinMode(device4, OUTPUT);
  pinMode(device5, OUTPUT);
  pinMode(device6, OUTPUT);

  // Initialize sensor pins
  pinMode(flameSensorPin, INPUT);
  pinMode(smokeSensorPin, INPUT);

  // Calibrate current sensor
  currentSensor.calibrate();
}

// Blynk virtual pin handlers to control devices via the app
BLYNK_WRITE(V1) { digitalWrite(device1, param.asInt()); }
BLYNK_WRITE(V2) { digitalWrite(device2, param.asInt()); }
BLYNK_WRITE(V3) { digitalWrite(device3, param.asInt()); }
BLYNK_WRITE(V4) { digitalWrite(device4, param.asInt()); }
BLYNK_WRITE(V5) { digitalWrite(device5, param.asInt()); }
BLYNK_WRITE(V6) { digitalWrite(device6, param.asInt()); }

void loop()
{
  Blynk.run();

  // Load monitoring with ACS712 sensor
  float loadCurrent = currentSensor.getCurrentAC();
  Serial.print("Load Current: ");
  Serial.println(loadCurrent);

  // Fire detection with flame sensor
  int flameState = digitalRead(flameSensorPin);
  if (flameState == LOW) {
    Serial.println("Flame detected!");
    // Trigger safety actions
    Blynk.notify("Fire detected!");
  }

  // Smoke detection with MQ sensor
  int smokeLevel = analogRead(smokeSensorPin);
  Serial.print("Smoke Level: ");
  Serial.println(smokeLevel);
  if (smokeLevel > 300) { // Adjust threshold as needed
    Serial.println("Smoke detected!");
    // Trigger safety actions
    Blynk.notify("Smoke detected!");
  }

  // Add a small delay to avoid flooding the serial monitor
  delay(1000);
}
