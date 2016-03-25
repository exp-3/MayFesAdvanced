#include "mbed.h"
#include "Display.hpp"

Serial pc(USBTX, USBRX);

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
    pc.printf("\n\r");
    wait(0.01);
  }
}
