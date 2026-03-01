/** \mainpage Hull Bus Terminal Signs
 *
 * \section intro_sec Introduction
 *
 * Documentation for the code to use the Hull bus terminal signs.
 *
 * The most useful page to look at is the [Panel class](./classPanel.html).
 *
 * For hardware setup instructions see [README.md](https://github.com/ConnectedHumber/Bus-Terminal-Signs/tree/master/README.md) and [Setup.md](https://github.com/ConnectedHumber/Bus-Terminal-Signs/tree/master/Setup.md).
 *
 * For example code see [examples](https://github.com/ConnectedHumber/Bus-Terminal-Signs/tree/master/Code/Examples).
 *
 * \section contribs Contributors
 *
 * Paul Richards ee@sosgez.co.uk
 *
 * alifeee - alifeee.web@outlook.com
 */

#include <AS1100.h>
#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>

#define ROWS_PER_CHIP 8
#define COLS_PER_CHIP 6
#define LOAD_PULSE_WIDTH 2 // 3 microseconds, duration of MARK
#define NEXT_PULSE_DELAY 1 // 1 microseconds, duration of SPACE following MARK

AS1100::AS1100(int loadPin) : GFXcanvas1(192, 9), _loadPin(loadPin) {
  pinMode(_loadPin, OUTPUT);
  digitalWrite(_loadPin, HIGH);
}

void AS1100::begin() {
  SPI.begin();

  sendCmd(0x0E00 + (2 & 3)); // set clock to internal
  sendCmd(0x0E00 + (0 & 3)); // set clock to external
  sendCmd(0x0900); // set binary mode
  setScan(ROWS_PER_CHIP);
  setIntensity(2, -1); // start low.
  sendCmd(0x0C01); // 0x0C01 - display on, 0x0C00 - display off
  display();
}

void AS1100::load() {
  // load pulse causes data to be loaded and displayed if display is on
  digitalWrite(_loadPin, LOW); // transfer from shift register to display drivers buffer
  delayMicroseconds(LOAD_PULSE_WIDTH);
  digitalWrite(_loadPin, HIGH);
  delayMicroseconds(NEXT_PULSE_DELAY);
}

void AS1100::writeDigit(int digit, uint8_t d) {
  // d=d & 0xFF;	// mask off trash
  int dd = (digit + 1) << 8;
  write16(0x0000 | dd | d);
}

void AS1100::write16(int d) {
  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE3));
  SPI.transfer16(d);
  SPI.endTransaction();
}

void AS1100::display() {
  for (int digit = 0; digit < 8; digit++) {
    for (int chip = 0; chip < 32; chip++) {
      uint8_t value = 0;

      if (digit > 1) {
        value |= getPixel(192 - chip * 6 - (digit - 1), 8);
      }

      for (int i = 0; i < 6; i++) {
        value <<= 1;
        value |= getPixel(192 - chip * 6 - (i + 1), 8 - (digit + 1));
      }

      writeDigit(digit, value);
    }

    load();
  }
}

/**
 * @brief Sets the intensity of one AS1100 chip (character) or ALL chips if chip=-1.
 * Note: you cannot set the intensity of individual LEDs.
 *
 * if chipNum is -1 (default) sets ALL chips to same intensity level
 * level can be 0..16
 * levels outside this range are ignored.
 *
 * The range of level is 0-32. `begin()` sets the initial intensity to 5.
 */
void AS1100::setIntensity(int level, int chipNum = -1) {
  // panel only supports 16 intensity levels
  // if chipNum>-1 only affects that chip
  if (level < 0 && level > 15)
    return; // ignore
  if (chipNum == -1) // set all chips
  {
    for (int c = 0; c < 32; c++)
      write16(0x0A00 + level); // the chip whose brightness needs setting
    load();
    return;
  }

  // we are setting one chip use no-op to push past chips not to be changed

  if (chipNum > 0) {
    for (int c = 0; c < chipNum - 1; c++)
      write16(0); // NO OP
    write16(0x0A00 + level); // the chip whose brightness needs setting
    load();
  }
}

/**
 * @brief Sets all the chips' intensities with one function.
 *
 * Chips is an array of intensity levels in chip order
 * i.e chips[0] is the first chip.
 *
 * This allows you to set the intensity of each character in one go. You provide an array of intensity values.
 *
 * Note that the code does not check if the length of chips is correct
 */

void AS1100::setIndividualIntensity(int chips[]) {
  // chip[s is an array of intensity values, one per chip
  for (int i = 32 - 1; i >= 0; i--)
    write16(0x0A00 + (chips[i] & 15));
  load();
}

/**
 * @brief sends the same command to all chips followed by load()
 */
void AS1100::sendCmd(int data) {
  // workhorse for many register updates below
  // only used when all registers need to contain the same value
  for (int chip = 0; chip < 32; chip++)
    write16(data);
  load();
}

/**
 * Set the scan mode of the chips - ie,. telling them how many
 * digits lines to use
 *
 * Used to configure the AS1100 number of digit lines (8). This is used by begin() so you don't need to call it.
 */

void AS1100::setScan(int totaldigits) {
  sendCmd(0x0B00 + ((totaldigits - 1) & 7)); // 7 means 8 blocks of 8
}
