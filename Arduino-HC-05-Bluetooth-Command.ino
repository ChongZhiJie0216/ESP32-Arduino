/*
 * Changing HC05 module's default device name using AT Command
 *
 * Pinout:
 * Key --> PIN 9
 * VCC --> 5V
 * TXD --> PIN 2
 * RXD --> PIN 3
 * GND --> GND
 *
 * AT Command : AT+NAME=<new device name>
 *
 */

#include <SoftwareSerial.h>

SoftwareSerial BTSerial(2, 3);   // RX | TX

void setup() 
{
  Serial.begin(9600);
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH); 
  Serial.println("HC-05 AT Commands:");
  Serial.println("Function                | Command                  | Response         | Parameter");
  Serial.println("Test                    | AT                       | OK               | None");
  Serial.println("Name                    | AT+NAME?                 | +NAME:HC-05, OK  | HC-05 (Default)");
  Serial.println("Password                | AT+PSWD?                 | +PSWD:1234, OK   | 1234 (Default)");
  Serial.println("Change Name             | AT+NAME = <name>         | OK               | None");
  Serial.println("Change Password         | AT+PSWD=<pswd>           | OK               | None");
  Serial.println("UART                    | AT+UART?                 | +UART:38400,0,0  | 38400 (Default)");
  Serial.println("Connection mode         | AT+CMODE?                | +CMOD:<cmod>     | 0 (Default)<br>");
  Serial.println("Connection mode set     | AT+CMODE=<cmode>         | Ok               | 0 (Connect to a specified address) >> 1 (Connect to any available address)");
  Serial.println("Find Address            | AT+ADDR?                 | +ADDR:<addr>, OK | None");
  Serial.println("Connect to Address      | AT+BIND=<addr>           | OK               | Replace the : with ,");
  Serial.println("Reset Device            | AT+RESET                 | OK               | None");
  Serial.println("Restore Factory Default | AT+ORGL                  | OK               | None");
  Serial.println("");
  Serial.println("Enter commands:");

  // HC-05 default speed in AT command mode
  BTSerial.begin(38400);
}

void loop()
{
  // Read from HC05 and send to Arduino
  if (BTSerial.available())
    Serial.write(BTSerial.read());

  // Read from serial monitor and send to HC05
  if (Serial.available())
    BTSerial.write(Serial.read());
}