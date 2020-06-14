#ifndef _KEYBOARD_EVENT_H_
#define _KEYBOARD_EVENT_H_

class KeyboardEvent {
   public:
    KeyboardEvent(char key, bool ctrl = false) : key(key), ctrl(ctrl) {}

    char GetKey() const { return key; }

    bool hasCtrl() const { return ctrl; }

   private:
    char key;
    bool ctrl;
};

#endif  // _KEYBOARD_EVENT_H_
