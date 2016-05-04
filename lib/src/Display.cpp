#include "mbed.h"
#include "Display.hpp"

// 8888888b.  888     888 888888b.   888      8888888 .d8888b.
// 888   Y88b 888     888 888  "88b  888        888  d88P  Y88b
// 888    888 888     888 888  .88P  888        888  888    888
// 888   d88P 888     888 8888888K.  888        888  888
// 8888888P"  888     888 888  "Y88b 888        888  888
// 888        888     888 888    888 888        888  888    888
// 888        Y88b. .d88P 888   d88P 888        888  Y88b  d88P
// 888         "Y88888P"  8888888P"  88888888 8888888 "Y8888P"

Display *Display::getInstance() {
  if(mInstance == NULL) {
    mInstance = new Display();
  }
  return mInstance;
}

void Display::set(int row, int col) { buffer[row][col] = true; }

void Display::reset(int row, int col) { buffer[row][col] = false; }

void Display::flush() {
  convertBufferToShiftRegisterCodes();
  clearBuffer();
}

void Display::clear() { clearBuffer(); }


// 8888888b.  8888888b.  8888888 888     888     d8888 88888888888 8888888888
// 888   Y88b 888   Y88b   888   888     888    d88888     888     888
// 888    888 888    888   888   888     888   d88P888     888     888
// 888   d88P 888   d88P   888   Y88b   d88P  d88P 888     888     8888888
// 8888888P"  8888888P"    888    Y88b d88P  d88P  888     888     888
// 888        888 T88b     888     Y88o88P  d88P   888     888     888
// 888        888  T88b    888      Y888P  d8888888888     888     888
// 888        888   T88b 8888888     Y8P  d88P     888     888     8888888888

Display *Display::mInstance = NULL;

Display::Display() {
  rclk = new DigitalOut(dp4);

  spi = new SPI(dp2, NC, dp6);
  spi->format(12, 0);

  clearBuffer();
  flush();

  ticker.attach_us(this, &Display::shiftCol, 625);
}

Display::~Display() { ticker.detach(); }

void Display::clearBuffer() {
  for(int i = 0; i < height; i++) {
    for(int j = 0; j < width; j++) {
      buffer[i][j] = false;
    }
  }
}

void Display::shiftCol() {
  static int currentCol = 0;
  currentCol &= 0b1111;
  sendShiftRegisterCode(shiftRegisterCodes[currentCol]);
  currentCol++;
}

void Display::convertBufferToShiftRegisterCodes() {
  for(int col = 0; col < width; col++) {
    shiftRegisterCodes[col] = generateShiftRegisterCode(col);
  }
}

int Display::generateShiftRegisterCode(int col) {
  static int rowMap[] = {7, 5, 4, 6, 0, 3, 1, 2};
  static int colMap[] = {10, 15, 14, 8, 13, 9, 11, 12, 2, 7, 6, 0, 5, 1, 3, 4};
  int colCode         = (1 << colMap[col]);
  int rowCode = 0;
  for(int i = 0; i < height; i++) {
    if(buffer[i][col]) {
      rowCode |= (1 << rowMap[i]);
    }
  }
  return ((rowCode << width) | colCode);
}

void Display::sendShiftRegisterCode(const int code) {
  spi->write(code >> 12);
  spi->write(code);
  *rclk = 0;
  *rclk = 1;
}
