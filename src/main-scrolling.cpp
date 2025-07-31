/*
 * Scrolling text
 * @alifeee
 */

#include "Arduino.h"
#include <CH_AS1100.h>
#include <pins.h>

// panel length
#define NUM_CHIPS 32
Panel topRow = Panel(LOAD_PIN, NUM_CHIPS);
Panel bottomRow = Panel(LOAD_PIN_2, NUM_CHIPS);

void showText(Panel &p, char *msg) {
  p.setCursor(0, 0);
  p.setTextColor(1);
  p.setTextSize(1); // pixel size multiplier
  p.println(msg);
  p.display();
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  topRow.begin();
  bottomRow.begin();

  showText(topRow, (char *)"this is a text");
  showText(bottomRow, (char *)"@alifeee");
}

void loop() {
  digitalWrite(LED_BUILTIN, LOW);
  topRow.scrollRows(-1, true); // wrap scroll left to right
  topRow.display();
  digitalWrite(LED_BUILTIN, HIGH);
  bottomRow.scrollRows(1, true); // wrap scroll right to left
  bottomRow.display();
}
