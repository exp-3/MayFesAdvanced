#include "mbed.h"
#include "Display.hpp"

Display *Display::mInstance = NULL;

Display *Display::getInstance() {
  if(mInstance == NULL) {
    mInstance = new Display();
  }
  return mInstance;
}

void Display::set(int row, int col) { buffers[backBuffer][row][col] = true; }

void Display::clear(int row, int col) { buffers[backBuffer][row][col] = false; }

void Display::swapBuffer() {
  if(surfaceBuffer == 0) {
    surfaceBuffer = 1;
    backBuffer    = 0;
  } else {
    surfaceBuffer = 0;
    backBuffer    = 1;
  }
  resetBuffer(buffers[backBuffer]);
}

Display::Display() {
  sig  = new DigitalOut(dp2);
  sclk = new DigitalOut(dp6);
  rclk = new DigitalOut(dp4);

  surfaceBuffer = 0;
  backBuffer    = 1;

  ticker.attach(this, &Display::shiftRow, 0.01);
}

Display::~Display() { ticker.detach(); }

void Display::resetBuffer(bool buffer[height][width]) {
  for(int i = 0; i < height; i++) {
    for(int j = 0; j < width; j++) {
      buffer[i][j] = false;
    }
  }
}

void Display::shiftRow() {
  static int currentRow = 0;
  currentRow &= 0x7;
  sendShiftRegisterCode(generateShiftRegisterCode(currentRow));
  currentRow++;
}

int Display::generateShiftRegisterCode(int row) {
  static int rowMap[] = {7, 5, 4, 6, 0, 3, 1, 2};
  static int colMap[] = {10, 15, 14, 8, 13, 9, 11, 12, 2, 7, 6, 0, 5, 1, 3, 4};
  int rowCode         = (1 << rowMap[row]);
  int colCode = 0;
  for(int i = 0; i < width; i++) {
    if(buffers[surfaceBuffer][row][i]) {
      colCode |= (1 << colMap[i]);
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
  *rclk = 0;
  *rclk = 1;
}