#ifndef _PEN_EVENT_H_
#define _PEN_EVENT_H_

#include "EmCommon.h"

class PenEvent {
   public:
    PenEvent() = default;

    uint32 getX() const { return x; };
    uint32 getY() const { return y; };
    bool isPenDown() const { return penDown; }

    static PenEvent down(uint32 x, uint32 y) { return PenEvent(x, y, true); };
    static PenEvent up() { return PenEvent(0, 0, false); }

   private:
    PenEvent(uint32 x, uint32 y, bool down) : x(x), y(y), penDown(down){};

   private:
    uint32 x{0}, y{0};
    bool penDown{false};
};

#endif  // _PEN_EVENT_H_
