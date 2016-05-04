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

 private:
  Display();
  ~Display();

  void clearBuffer();
  void shiftCol();
  void convertBufferToShiftRegisterCodes();
  int generateShiftRegisterCode(int col);
  void sendShiftRegisterCode(int code);

  static Display *mInstance;
  DigitalOut *rclk;
  SPI *spi;
  Ticker ticker;

  bool buffer[height][width];
  int shiftRegisterCodes[width];
};

#endif /* end of include guard: DISPLAY_H */
