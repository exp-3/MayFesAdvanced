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
  I2C *i2c;
  char DEVICE_ADDR_READ  = 0xa7;
  char DEVICE_ADDR_WRITE = 0xa6;
  char DATA_FORMAT_REG   = 0x31;
  char POWER_CTL_REG     = 0x2d;
  char DATA_REG          = 0x32;

  int x, y, z;
};

#endif /* end of include guard: ACCELEROMETER_H */
