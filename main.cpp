#include "mbed.h"
#include "Display.hpp"
#include "Accelerometer.hpp"

Serial pc(USBTX, USBRX);

int main() {
  pc.baud(115200);
  pc.printf("Hello World.\n\r");

  int count = 0;

  Display *display = Display::getInstance();
  Accelerometer *accel = Accelerometer::getInstance();
  display->clear();
  for(int i = 0; i < 8; i++) {
    for(int j = 0; j < 16; j++) {
      display->set(i, j);
    }
  }
  display->flush();

  while(1) {
    pc.printf("count: %d\n\r", count);
    count++;
    pc.printf("%d\n\r", (int)display->getBuffer(0, 0));
    pc.printf("%d\n\r", (int)display->getBuffer(0, 1));
    pc.printf("%d\n\r", (int)display->getBuffer(0, 2));
    pc.printf("\n\r");
    wait(0.01);
  }
}
