/*
 * Scrolling text
 * @alifeee
 */

#include "Arduino.h"
#include <CH_AS1100.h>
#include <pins.h>

// panel length
#define NUM_CHIPS 32
Panel topRow = Panel(LOAD_PIN_TOP, NUM_CHIPS);
Panel bottomRow = Panel(LOAD_PIN_BOTTOM, NUM_CHIPS);

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  topRow.begin();
  bottomRow.begin();

  topRow.fillScreen(1);
  topRow.display();

  bottomRow.fillScreen(1);
  bottomRow.display();
}

void loop() {
  topRow.display();
  delay(1000);
}
