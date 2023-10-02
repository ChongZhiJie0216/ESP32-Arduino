#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "SevSeg.h"

SevSeg Display;

const int ledPin = 4; //GPIO 4 给2粒点的LED
unsigned long previousMillis = 0;
const long interval = 500; //半秒闪一次

// Define NTP variables
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 8 * 3600); //set 国际时间，这边是 GMT +8

void setup() {
  // Connect to Wi-Fi
  WiFi.begin("ssid", "pswd"); //你的SSID 跟密码 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize NTP client
  timeClient.begin();

  // Set up the pins and display
  byte numDigits = 4;
  byte digitPins[] = {12, 13, 14, 15}; //display 的pin （数位）
  byte segmentPins[] = {18, 19, 21, 22, 23, 25, 26}; //display 的段pin
  bool resistorsOnSegments = false;
  bool updateWithDelays = false;
  byte hardwareConfig = COMMON_CATHODE;
  bool leadingZeros = false; //显示 “0” 如果是空白
  bool disableDecPoint = true;
  Display.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments, updateWithDelays, leadingZeros, disableDecPoint);
  Display.setBrightness(100);

  // Set up the LED pin
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Update NTP time
  timeClient.update();

  // Get the current time from the NTP server
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
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Toggle the LED state
    if (digitalRead(ledPin) == LOW) {
      digitalWrite(ledPin, HIGH);
    } else {
      digitalWrite(ledPin, LOW);
    }
  }
}