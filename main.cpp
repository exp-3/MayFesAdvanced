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

class Ball {
 public:
  int x, y;
  int vx, vy;
};

class Bar {
 public:
  int x;
};

int main() {
  pc.baud(115200);
  pc.printf("Hello World.\n\r");

  int count = 0;

  Display *display     = Display::getInstance();
  Accelerometer *accel = Accelerometer::getInstance();

  int height = 3;
  int width  = 5;
  bool blocks[height][width];

  for(int i = 0; i < height; i++) {
    for(int j = 0; j < width; j++) {
      blocks[i][j] = true;
    }
  }

  Ball ball;
  ball.x  = 0;
  ball.y  = 4;
  ball.vx = 1;
  ball.vy = 1;

  Bar bar;
  bar.x = 6;

  while(1) {
    // ballの衝突判定
    if(ball.y == 6 && ball.x >= bar.x && ball.x < bar.x + 4) {
      ball.vy = -ball.vy;
    }

    for(int i = 0; i < height; i++) {
      for(int j = 0; j < width; j++) {
        if(blocks[i][j]) {
          if((ball.y == i + 1 && ball.x >= 3 * j + 1 && ball.x < 3 * j + 3 &&
              ball.vy < 0) ||
             (ball.y == i - 1 && ball.x >= 3 * j + 1 && ball.x < 3 * j + 3 &&
              ball.vy > 0)) {
            blocks[i][j] = false;
            ball.vy      = -ball.vy;
          }
          if(ball.y == i && ((ball.x == 3 * j && ball.vx > 0) ||
                             (ball.x == 3 * j + 3 && ball.vx < 0))) {
            blocks[i][j] = false;
            ball.vx      = -ball.vx;
          }
        }
      }
    }

    // barの更新
    bar.x += accel->getY() >> 5;
    if(bar.x < 0) {
      bar.x = 0;
    } else if(bar.x > 12) {
      bar.x = 12;
    }

    // ballの更新
    ball.x += ball.vx;
    if(ball.x <= 0) {
      ball.x  = 0;
      ball.vx = -ball.vx;
    } else if(ball.x >= 15) {
      ball.x  = 15;
      ball.vx = -ball.vx;
    }

    ball.y += ball.vy;
    if(ball.y <= 0) {
      ball.y  = 0;
      ball.vy = -ball.vy;
    } else if(ball.y >= 8) {
      break;
    }

    //描画開始
    display->clear();

    // blockの描画
    for(int i = 0; i < height; i++) {
      for(int j = 0; j < width; j++) {
        if(blocks[i][j]) {
          for(int k = 0; k < 2; k++) {
            display->set(i, 3 * j + k + 1);
          }
        }
      }
    }

    // barの描画
    for(int i = 0; i < 4; i++) {
      display->set(7, bar.x + i);
    }

    // ballの描画
    display->set(ball.y, ball.x);

    display->flush();

    count++;
    wait(0.1);
  }
}
