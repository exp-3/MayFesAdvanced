#ifndef ALPHABETS_H
#define ALPHABETS_H

#include "Display.hpp"

class Alphabets {
public:
  Alphabets(char c);
  void draw(int x, int y, int off, int len);
  int width;
  int height;

private:
  int id;
  struct Alphabet {
    int width;
    int height;
    bool fontmap[4][4];
  };

  static const Alphabet alphabets[26];
  Display *display;
};


#endif /* end of include guard: ALPHABETS_H */
