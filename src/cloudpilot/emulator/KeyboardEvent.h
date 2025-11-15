#ifndef _KEYBOARD_EVENT_H_
#define _KEYBOARD_EVENT_H_

#include "EmCommon.h"

class KeyboardEvent {
   public:
    KeyboardEvent(uint16 key) : key(key) {}

    uint16 GetKey() const { return key; }

   private:
    uint16 key;
};

#endif  // _KEYBOARD_EVENT_H_
