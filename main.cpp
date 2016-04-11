#include "mbed.h"
#include "Display.hpp"
#include "Accelerometer.hpp"
#include "Game.hpp"
#include "BreakOut.hpp"

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

  Game *game = BreakOut::getInstance();

  while(1) {
    if(game->isGameOver()) {
      break;
    }
    //描画開始
    display->clear();

    game->update();

    display->flush();

    count++;
    wait(0.1);
  }
}
