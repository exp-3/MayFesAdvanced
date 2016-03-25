#ifndef DISPLAY_H

#include "mbed.h"

class Display {
 public:
  static Display *getInstance();
  static const int height = 8;
  static const int width = 16;
  void set(int row, int col);
  void clear(int row, int col);
  void swapBuffer();

 private:
  Display();
  ~Display();

  void resetBuffer(bool buffer[height][width]);
  void shiftRow();
  int generateShiftRegisterCode(int row);
  void sendShiftRegisterCode(int code);

  static Display *mInstance;
  DigitalOut *sig;
  DigitalOut *sclk;
  DigitalOut *rclk;
  Ticker ticker;

  int surfaceBuffer;
  int backBuffer;
  bool buffers[2][height][width];
};

#endif
