/*
 * Scrolling text
 * @alifeee
 */

#include "Arduino.h"
#include <CH_AS1100.h>
#include <pins.h>

Panel topRow = Panel(LOAD_PIN_TOP);
Panel bottomRow = Panel(LOAD_PIN_BOTTOM);

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  topRow.begin();
  bottomRow.begin();

  topRow.fillScreen(0);
  topRow.println("asdf");
  topRow.display();

  bottomRow.fillScreen(1);
  bottomRow.display();
}

void loop() {
  topRow.display();
  delay(1000);
}
