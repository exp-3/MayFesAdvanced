class Game {
public:
  virtual Game* getInstance() = 0;
  virtual void update() = 0;

protected:
  Game();
  ~Game();
};
