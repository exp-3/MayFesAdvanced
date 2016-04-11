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

void Display::set(int row, int col) { buffers[backBuffer][row][col] = true; }

void Display::reset(int row, int col) { buffers[backBuffer][row][col] = false; }

void Display::flush() {
  if(surfaceBuffer == 0) {
    surfaceBuffer = 1;
    backBuffer    = 0;
  } else {
    surfaceBuffer = 0;
    backBuffer    = 1;
  }
  clearBuffer(buffers[backBuffer]);
}

void Display::clear() { clearBuffer(buffers[backBuffer]); }

bool Display::getBuffer(int row, int col) {
  return buffers[surfaceBuffer][row][col];
}

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
  // sig  = new DigitalOut(dp2);
  // sclk = new DigitalOut(dp6);
  rclk = new DigitalOut(dp4);

  // spi = new SPI(dp2, NC, dp6);
  // spi->format(8, 0);

  surfaceBuffer = 0;
  backBuffer    = 1;

  clearBuffer(buffers[surfaceBuffer]);
  clearBuffer(buffers[backBuffer]);

  ticker.attach_us(this, &Display::shiftCol, 100);
}

Display::~Display() { ticker.detach(); }

void Display::clearBuffer(bool buffer[height][width]) {
  for(int i = 0; i < height; i++) {
    for(int j = 0; j < width; j++) {
      buffer[i][j] = false;
    }
  }
}

void Display::shiftCol() {
  static int currentCol = 0;
  currentCol &= 0b1111;
  sendShiftRegisterCode(generateShiftRegisterCode(currentCol));
  currentCol++;
}

int Display::generateShiftRegisterCode(int col) {
  static int rowMap[] = {7, 5, 4, 6, 0, 3, 1, 2};
  static int colMap[] = {10, 15, 14, 8, 13, 9, 11, 12, 2, 7, 6, 0, 5, 1, 3, 4};
  int colCode         = (1 << colMap[col]);
  int rowCode = 0;
  for(int i = 0; i < height; i++) {
    if(buffers[surfaceBuffer][i][col]) {
      rowCode |= (1 << rowMap[i]);
    }
  }
  return ((rowCode << width) | colCode);
}

void Display::sendShiftRegisterCode(int code) {
  for(int i = 0; i < height + width; i++) {
    *sig  = (code & (1 << ((height + width - 1) - i)));
    *sclk = 0;
    *sclk = 1;
  }
  //spi->write(code);
  *rclk = 0;
  *rclk = 1;
}
