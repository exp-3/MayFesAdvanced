#include "mbed.h"

DigitalOut sig(dp2);
DigitalOut sclk(dp6);
DigitalOut rclk(dp4);

void sendByte(uint8_t data) {
  for(int i = 0; i < 8; i++) {
    sig  = (data & (1 << i));
    sclk = 1;
    sclk = 0;
  }
  rclk = 1;
  rclk = 0;
}

int main() {
  for(int i = 0; i < 3; i++) {
    sendByte(0b00000000);
  }
  sendByte(0b01000000);
  sendByte(0b00000010);
  sendByte(0b00000000);
  while(1) {
    wait(1);
  }
}
