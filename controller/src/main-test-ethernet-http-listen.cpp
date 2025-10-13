/**
 * Ethernet test - fetch
 *
 * reply to HTTP requests with a simple webpage
 * adapted from
 * https://docs.arduino.cc/tutorials/ethernet-shield-rev2/barometric-pressure-web-server/
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
  Serial.println(F("Starting ethernet..."));
  if (!Ethernet.begin(mac))
    Serial.println(F("failed"));
  else {
    Serial.print(F("Connected to… "));
    Serial.print(Ethernet.localIP());
    Serial.print(F(" on "));
    Serial.println(Ethernet.gatewayIP());
  }

  // start listening for clients
  Serial.println(F("starting server"));
  server.begin();
  delay(1000);
}

void loop() {
  EthernetClient client = server.available();
  if (client) {
    Serial.println("Got a client, sending response…");
    // an http request ends with a blank line
    char lastChar = '\n';
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && (lastChar == '\n' || lastChar == '\r')) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          // print the current readings, in HTML format:
          client.print("Temperature: ");
          client.print(" degrees C");
          client.println("<br />");
          client.print(" Pa");
          client.println("<br />");
          break;
        }
      }
    }
    delay(1);
    client.stop();
    Serial.println("  finished!");
  }
}
