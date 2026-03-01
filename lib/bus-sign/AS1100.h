/**
 * Bus sign class
 * for interfacing with Hull Bus Station signs
 * modified from Paul Richards work May 2020
 */
#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>

class AS1100 : public GFXcanvas1 {
public:
  AS1100(int LoadPin);

  void begin();
  void display();

  void setScan(int totaldigits);
  void setIntensity(int level, int chip); // sets intensity of one or all chips (-1 default)
  void setIndividualIntensity(int chips[]); // provide an array of intensities, one per chip

private:
  void load();
  void write16(int d);
  void writeDigit(int digit, uint8_t d); // used by display()
  void sendCmd(int cmdData);

  int _loadPin = 0;
  int _clkPin = 0;
  int _dataPin = 0;
};
