/**
 * Bus sign class
 * for interfacing with Hull Bus Station signs
 * modified from Paul Richards work May 2020
 */
#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>

class Panel : public Adafruit_GFX {
public:
  Panel(int LoadPin, int NumChips);
  ~Panel();

  bool begin(); // initialises CLK,LOAD,DATA signal states
  void display(void); // called to send pixels to the display

  void drawPixel(int16_t x, int16_t y, uint16_t color) override;
  void setClockMode(int m);
  void displayTest(bool state); // all leds turned on if state is true, off if false
  void setBinaryMode();
  void setScan(int totaldigits);
  void setIntensity(int level, int chip); // sets intensity of one or all chips (-1 default)
  void setIndividualIntensity(int chips[]); // provide an array of intensities, one per chip

  void displayOn(int b); // 1=on, 0=off. Entire matrix is turned on/off
  uint8_t getPixel(int x, int y);

private:
  void load();
  void write16(int d);
  void writeDigit(int digit, uint8_t d); // used by display()
  void sendCmd(int cmdData);

  int _loadPin = 0;
  int _clkPin = 0;
  int _dataPin = 0;

  // hardware related
  int _numChips = 0; // assigned in constructor to allow for variable length panels
  int _numColumns = 0;

  uint8_t *pixels[8]; // each chip has 8 rows and 6 segments
};
