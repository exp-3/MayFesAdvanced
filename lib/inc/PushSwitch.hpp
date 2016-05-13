#ifndef PUSHSWITCH_H
#define PUSHSWITCH_H

#include "mbed.h"

class PushSwitch {
public:
  static PushSwitch *getInstance();
  bool isPressed();
  void reset();

private:
  PushSwitch();
  ~PushSwitch();

  void setPressedFlag();

  static PushSwitch *mInstance;
  InterruptIn *event;

  bool pressedFlag;
};


#endif /* end of include guard: PUSHSWITCH_H */
