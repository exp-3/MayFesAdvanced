#include "mbed.h"
#include "Accelerometer.hpp"

Accelerometer *Accelerometer::mInstance = NULL;

Accelerometer *Accelerometer::getInstance() {
  if(mInstance == NULL) {
    mInstance = new Accelerometer();
  }
  return mInstance;
}

int Accelerometer::getX() { return x; }

int Accelerometer::getY() { return y; }

int Accelerometer::getZ() { return z; }

Accelerometer::Accelerometer() {
  i2c = new I2C(dp5, dp27);
  i2c->frequency(100000);
  char byte = 0x00;
  writeData(DATA_FORMAT_REG, &byte, 1);
  byte = 0x08;
  writeData(POWER_CTL_REG, &byte, 1);
}

void Accelerometer::updateValue() {
  int length = 6;
  char axis_buff[length];
  readData(DATA_REG, axis_buff, length);
  x = (((int)axis_buff[1]) << 8) | axis_buff[0];
  y = (((int)axis_buff[3]) << 8) | axis_buff[2];
  z = (((int)axis_buff[5]) << 8) | axis_buff[4];
}

void Accelerometer::writeData(char reg, const char *data, int length) {
  char temp[length + 1];
  temp[0] = reg;
  for(int i = 0; i < length; i++) {
    temp[i + 1] = data[i];
  }
  i2c->write(DEVICE_ADDR_WRITE, temp, length + 1);
}

void Accelerometer::readData(char reg, char *data, int length) {
  writeData(reg, NULL, 0);
  i2c->read(DEVICE_ADDR_READ, data, length);
}
