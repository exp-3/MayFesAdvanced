#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include "mbed.h"

class Accelerometer {
 public:
  static Accelerometer *getInstance();
  int getX();
  int getY();
  int getZ();

 private:
  Accelerometer();

  void writeData(char reg, const char *data, int length);
  void readData(char reg, char *data, int length);

  void updateValue();

  static Accelerometer *mInstance;
  static const char DEVICE_ADDR_READ  = 0xa7;
  static const char DEVICE_ADDR_WRITE = 0xa6;
  static const char DATA_FORMAT_REG   = 0x31;
  static const char POWER_CTL_REG     = 0x2d;
  static const char DATA_REG          = 0x32;
  Ticker ticker;

  I2C *i2c;
  int x, y, z;
};

#endif /* end of include guard: ACCELEROMETER_H */
