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

// #define SEND_PIXELS_OLD	// for debugging

#include <CH_AS1100.h>
#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>

#define ROWS_PER_CHIP 8 // there are 9 rows but only 8 used
#define COLS_PER_CHIP 6
#define LOAD_PULSE_WIDTH 2 // 3 microseconds, duration of MARK

#define NEXT_PULSE_DELAY 1 // 1 microseconds, duration of SPACE following MARK

Panel::Panel(int loadPin) : GFXcanvas1(192, 9), _loadPin(loadPin) {
  pinMode(_loadPin, OUTPUT);
  digitalWrite(_loadPin, HIGH);
}

boolean Panel::begin() {
  SPI.begin();
  setClockMode(2); // reset the clock to internal
  setClockMode(0);
  setBinaryMode();
  setScan(ROWS_PER_CHIP);
  setIntensity(2, -1); // start low.
  displayOn(1);
  display();
  return true;
}

void Panel::load() {
  // load pulse causes data to be loaded and displayed if display is on
  digitalWrite(_loadPin, LOW); // transfer from shift register to display drivers buffer
  delayMicroseconds(LOAD_PULSE_WIDTH);
  digitalWrite(_loadPin, HIGH);
  delayMicroseconds(NEXT_PULSE_DELAY);
}

void Panel::writeDigit(int digit, uint8_t d) {
  // d=d & 0xFF;	// mask off trash
  int dd = (digit + 1) << 8;
  write16(0x0000 | dd | d);
}

void Panel::write16(int d) {
  // first 4 bits are don't care so we send zeros
  // caller must call load() if this is the last write16
  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE3));
  SPI.transfer16(d);
  SPI.endTransaction();
}

void Panel::display() {
  for (int digit = 0; digit < 8; digit++) {
    for (int chip = 0; chip < 32; chip++) {
      uint8_t value = 0;

      for (int i = 0; i < 6; i++) {
        value <<= 1;
        value |= getPixel(192 - chip * 6 - i, 6 - digit);
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
void Panel::setIntensity(int level, int chipNum = -1) {
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

void Panel::setIndividualIntensity(int chips[]) {
  // chip[s is an array of intensity values, one per chip
  for (int i = 32 - 1; i >= 0; i--)
    write16(0x0A00 + (chips[i] & 15));
  load();
}

/**
 * @brief sends the same command to all chips followed by load()
 */
void Panel::sendCmd(int data) {
  // workhorse for many register updates below
  // only used when all registers need to contain the same value
  for (int chip = 0; chip < 32; chip++)
    write16(data);
  load();
}

/**
 * The AS1100 chips can be driven by an external clock (See AS1100 data sheet). This is called by begin().
 *
 * This funcion is also used to reset the display. See datasheet, section 8.10.
 */
void Panel::setClockMode(int m) {
  sendCmd(0x0E00 + (m & 3));
}

/**
 * @brief Turns the panel on/off. This needs to be called to see anything.
 * Can be called to flash the display since LED state is not affected
 *
 * state 0 means off, non-zero means on
 *
 * The panel can still be programmed whilst the display is off it just won't appear till you turn the display on again.
 */

void Panel::displayOn(int state) {
  if (state != 0)
    sendCmd(0x0C01);
  else
    sendCmd(0x0C00);
}

/**
 * @brief LED test. Regardless of programming, this turns on/off all LEDs
 *
 * on if state=true and off if state=false.
 *
 * It's useful to check that you don't have any burned out LEDs.
 */
void Panel::displayTest(bool state) {
  // panel is turned on if state is non-zero, otherwise off
  if (state)
    sendCmd(0x0F01);
  else
    sendCmd(0x0F00);
}

/**
 * This puts the chips in binary mode which allows us to
 * control which leds are on/off using the digit and segment lines
 *
 * This allows us to address each pixel separately.
 *
 * The AS1100 is basically intended for 7 segment displays, but with this we can control a dot-matrix of pixels too.
 *
 * See datasheet section 9.3
 */
void Panel::setBinaryMode() {
  sendCmd(0x0900);
}

/**
 * Set the scan mode of the chips - ie,. telling them how many
 * digits lines to use
 *
 * Used to configure the AS1100 number of digit lines (8). This is used by begin() so you don't need to call it.
 */

void Panel::setScan(int totaldigits) {
  sendCmd(0x0B00 + ((totaldigits - 1) & 7)); // 7 means 8 blocks of 8
}
