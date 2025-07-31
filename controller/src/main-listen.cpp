/*
 * Listen to SoftwareSerial/UART on pins 3/4
 * @alifeee
 */

#include "Arduino.h"
#include <SoftwareSerial.h>

SoftwareSerial link(3, 4); // Rx, Tx

String message;

void setup() {
  link.begin(9600);   // setup software serial
  Serial.begin(9600); // setup serial monitor
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.print("reset...");

  pinMode(3, INPUT);
  pinMode(4, OUTPUT);
}

void loop() {
  char character;

  while (link.available()) {
    character = link.read();
    Serial.println("got character");
    Serial.println(character);
    // message.concat(character);
    // if (character == '\0') {
    //   Serial.println("got message");
    //   Serial.println(message);
    //   message = "";
    // }
  }
}
