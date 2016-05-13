#ifndef BREAKOUT_HPP
#define BREAKOUT_HPP

#include "Game.hpp"
#include "Display.hpp"
#include "Accelerometer.hpp"

class Ball {
public:
  int x, y;
  int vx, vy;
};

class Bar {
public:
  int x;
};

class BreakOut : Game {
public:
  static Game *getInstance();
  void init();
  void update();
  bool isGameOver();
  bool isCleared();

private:
  BreakOut();
  ~BreakOut();

  static BreakOut *mInstance;

  static const int height = 3;
  static const int width = 5;
  bool blocks[height][width];

  Display *display;
  Accelerometer *accel;
  Ball ball;
  Bar bar;

  bool gameOverFlag;
  bool clearFlag;
};

#endif /* end of include guard: BREAKOUT_HPP */
