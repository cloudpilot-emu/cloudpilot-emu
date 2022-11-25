#ifndef _KEYBOARD_EVENT_H_
#define _KEYBOARD_EVENT_H_

#include "EmCommon.h"

class KeyboardEvent {
   public:
    KeyboardEvent(uint16 key, bool ctrl = false) : key(key), ctrl(ctrl) {}

    uint16 GetKey() const { return key; }

    bool hasCtrl() const { return ctrl; }

   private:
    uint16 key;
    bool ctrl;
};

#endif  // _KEYBOARD_EVENT_H_
