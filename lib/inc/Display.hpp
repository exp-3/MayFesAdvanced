#ifndef DISPLAY_H
#define DISPLAY_H

#include "mbed.h"

class Display {
 public:
  static Display *getInstance();
  static const int height = 8;
  static const int width = 16;
  void set(int row, int col);
  void reset(int row, int col);
  void flush();
  void clear();
  bool getBuffer(int row, int col);

 private:
  Display();
  ~Display();

  void clearBuffer(bool buffer[height][width]);
  void shiftCol();
  int generateShiftRegisterCode(int col);
  void sendShiftRegisterCode(int code);

  static Display *mInstance;
  DigitalOut *sig;
  DigitalOut *sclk;
  DigitalOut *rclk;
  SPI *spi;
  Ticker ticker;

  int surfaceBuffer;
  int backBuffer;
  bool buffers[2][height][width];
};

#endif /* end of include guard: DISPLAY_H */
