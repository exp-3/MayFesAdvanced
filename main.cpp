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
  display->set(0, 0);
  display->set(1, 1);
  display->set(2, 2);
  display->flush();

  while(1) {
    pc.printf("count: %d\n\r", count);
    count++;
    pc.printf("%d\n\r", (int)display->getBuffer(0, 0));
    pc.printf("%d\n\r", (int)display->getBuffer(0, 1));
    pc.printf("%d\n\r", (int)display->getBuffer(1, 1));
    pc.printf("\n\r");
    wait(0.01);
  }
}
