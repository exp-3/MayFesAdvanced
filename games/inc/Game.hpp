#ifndef GAME_HPP
#define GAME_HPP

class Game {
public:
  virtual void update() = 0;
  virtual bool isGameOver() = 0;
  virtual bool isCleared() = 0;

protected:
  Game();
  ~Game();
};

#endif /* end of include guard: GAME_HPP */
