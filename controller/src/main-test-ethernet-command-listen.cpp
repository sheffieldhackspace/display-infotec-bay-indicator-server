/**
 * Ethernet test - commands
 *
 * echo back what is sent to the server
 * messages should be null-byte terminated
 * send things with e.g.,
 *   echo -e 'I am on a serial port!\0' | netcat 192.168.0.31 80
 */

#include <Arduino.h>
#define W5500_ETHERNET_SHIELD
#include <Ethernet.h>
#include <SPI.h>

byte mac[] = {0x60, 0xF2, 0xEC, 0x18, 0x05, 0xFE};

EthernetServer server(80);

void setup() {
  Serial.begin(9600);

  // disable SD SPI
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);

  // start ethernet
  Serial.println(F("Starting ethernet"));
  while (!Ethernet.begin(mac)) {
    Serial.print(F("."));
    delay(200);
  }

  Serial.print(F("Connected to… "));
  Serial.print(Ethernet.localIP());
  Serial.print(F(" on "));
  Serial.println(Ethernet.gatewayIP());

  // start listening for clients
  Serial.println(F("starting server"));
  server.begin();
  delay(1000);
  Serial.println(F("server started"));
}

void loop() {
  EthernetClient client = server.available();
  if (client) {
    Serial.println("Got a client");
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\0') {
          break;
        }
        Serial.print(c);
      }
    }
    Serial.println("");
    Serial.println("finished");
    delay(1);
    client.stop();
  }
}
