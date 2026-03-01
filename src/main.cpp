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
  pinMode(LED_BUILTIN, OUTPUT);
  topRow.begin();
  bottomRow.begin();

  topRow.fillDisplay(0);
  topRow.display();

  bottomRow.fillDisplay(1);
  bottomRow.display();
}

void loop() {
//  digitalWrite(LED_BUILTIN, LOW);
//  topRow.display();
//  digitalWrite(LED_BUILTIN, HIGH);
//  bottomRow.display();
}
