#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "SevSeg.h"
#include <Time.h>
#include <TimeLib.h>

SevSeg Display;

const char* ssid = "pohhoon65"; // Your SSID
const char* password = "20camry825505"; // Your password
const int ledPin = 4; // GPIO 4 for 2-LEDs
unsigned long previousMillis = 0;
const long interval = 500; // Blink every half second

// Define NTP variables
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 8 * 3600); // Set to international time (GMT +8)

void setup() {
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize NTP client
  timeClient.begin();

  // Set up the pins and display
  byte numDigits = 4;
  byte digitPins[] = {12, 13, 14, 15}; // Display digit pins
  byte segmentPins[] = {18, 19, 21, 22, 23, 25, 26}; // Display segment pins
  bool resistorsOnSegments = false;
  bool updateWithDelays = false;
  byte hardwareConfig = COMMON_CATHODE;
  bool leadingZeros = false; // Display "0" if blank
  bool disableDecPoint = true;
  Display.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments, updateWithDelays, leadingZeros, disableDecPoint);
  Display.setBrightness(50);

  // Set up the LED pin
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Update NTP time every hour
  unsigned long currentMillis = millis();
  static unsigned long lastSyncMillis = 0;
  const long syncInterval = 3600000; // 1小时

  if (currentMillis - lastSyncMillis >= syncInterval) {
    lastSyncMillis = currentMillis;
    
    // Force time synchronization with NTP server
    timeClient.forceUpdate();
  }

  // Get the current time from the NTP server
  timeClient.update();
  int hours = timeClient.getHours();
  int minutes = timeClient.getMinutes();

  // Convert to 12-hour format
  if (hours > 12) {
    hours -= 12;
  }
  if (hours == 0) {
    hours = 12;
  }

  // Display the time in 12-hour format
  int timeToDisplay = hours * 100 + minutes;

  Display.setNumber(timeToDisplay);
  Display.refreshDisplay();

  // Non-blocking LED blinking
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Toggle the LED state
    digitalWrite(ledPin, !digitalRead(ledPin));
  }
}