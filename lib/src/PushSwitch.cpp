#include "mbed.h"

#include "PushSwitch.hpp"

// public
PushSwitch *PushSwitch::getInstance() {
  if(mInstance == NULL) {
    mInstance = new PushSwitch();
  }
  return mInstance;
}

bool PushSwitch::isPressed() {
  return pressedFlag;
}

void PushSwitch::reset() {
  pressedFlag = false;
}

// private
PushSwitch *PushSwitch::mInstance = NULL;

PushSwitch::PushSwitch() {
  event = new InterruptIn(dp17);
  reset();

  event->fall(this, &PushSwitch::setPressedFlag);
}

PushSwitch::~PushSwitch() { delete event; }

void PushSwitch::setPressedFlag() {
  pressedFlag = true;
}
