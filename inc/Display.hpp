#ifndef DISPLAY_H

class Display {
 public:
  Display();
  const int width  = 16;
  const int height = 8;
  void set(int x, int y);
  void swapBuffer();

 private:
};

#endif
