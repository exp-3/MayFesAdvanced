#include "mbed.h"
#include "Display.hpp"
#include "Accelerometer.hpp"

Serial pc(USBTX, USBRX);

void printValue(int value) {
  if(value < 0) {
    value = -value;
    for(int i = 0; i < (value >> 4); i++) {
      pc.printf("-");
    }
  } else {
    for(int i = 0; i < (value >> 4); i++) {
      pc.printf("+");
    }
  }
  pc.printf("\n\r");
}

int main() {
  pc.baud(115200);
  pc.printf("Hello World.\n\r");

  int count = 0;

  Display *display     = Display::getInstance();
  Accelerometer *accel = Accelerometer::getInstance();
  display->clear();
  for(int i = 0; i < 8; i++) {
    for(int j = 0; j < 16; j++) {
      display->set(i, j);
    }
  }
  display->flush();

  while(1) {
    pc.printf("\033[2J");
    pc.printf("\033[0;0H");
    pc.printf("count: %d\n\r", count);
    count++;
    pc.printf("x: ");
    printValue(accel->getX());
    pc.printf("y: ");
    printValue(accel->getY());
    pc.printf("z: ");
    printValue(accel->getZ());
    wait(0.01);
  }
}
