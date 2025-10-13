/**
 * Blink
 *
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */
#include "Arduino.h"

// Set LED_BUILTIN if it is not defined by Arduino framework
// #define LED_BUILTIN 13
#define LOGIC_PIN 2

void setup() {
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LOGIC_PIN, OUTPUT);

  Serial.begin(115200);
}

void loop() {
  // turn the LED OFF
  Serial.println("HIGH");
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(LOGIC_PIN, HIGH);

  // wait for a second
  delay(1000);

  // turn the LED ON by making the voltage LOW
  Serial.println("LOW");
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(LOGIC_PIN, LOW);

  // wait for a second
  delay(1000);
}
