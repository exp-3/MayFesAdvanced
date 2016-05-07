#ifndef CHARACTOR_H
#define CHARACTOR_H

class Charactor {
public:
  Charactor(char c);
  void draw(int x, int y);

private:
  const static bool shapes[26][4][4];
  int id;
};


#endif /* end of include guard: CHARACTOR_H */
