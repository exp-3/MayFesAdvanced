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

Display::Display() {
  sig  = new DigitalOut(dp2);
  sclk = new DigitalOut(dp6);
  rclk = new DigitalOut(dp4);

  surfaceBuffer = 0;
  backBuffer    = 1;

  clearBuffer(buffers[surfaceBuffer]);
  clearBuffer(buffers[backBuffer]);

  ticker.attach_us(this, &Display::shiftCol, 80);
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
  *rclk = 0;
  *rclk = 1;
}
