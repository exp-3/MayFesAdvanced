#include "mbed.h"
#include "Display.hpp"

Serial pc(USBTX, USBRX);

I2C i2c(dp5, dp27);

int device_addr_read  = 0xa7;
int device_addr_write = 0xa6;
char data_format[]    = {0x31, 0x00};
char power_ctl[]      = {0x2d, 0x08};
char axis_buff[6];
char data_reg = 0x32;

int main() {
  pc.baud(115200);
  pc.printf("Hello World.\n\r");

  int count = 0;

  Display *display = Display::getInstance();
  display->set(0, 0);
  display->set(1, 1);
  display->set(2, 2);
  display->swapBuffer();

  while(1) {
    pc.printf("count: %d\n\r", count);
    count++;
    pc.printf("x: %d\n\r", x);
    pc.printf("y: %d\n\r", y);
    pc.printf("z: %d\n\r", z);
    pc.printf("\n\r");
    wait(0.01);
  }
}
