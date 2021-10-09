#ifndef _SCREEN_DIMENSIONS_H_
#define _SCREEN_DIMENSIONS_H_

#include "EmCommon.h"

class ScreenDimensions {
   public:
    enum Kind { screen160x160 = 0x00, screen320x320 = 0x01, screen240x320 = 0x02 };

    ScreenDimensions() = default;
    ScreenDimensions(const ScreenDimensions&) = default;
    ScreenDimensions(ScreenDimensions&&) = default;

    ScreenDimensions& operator=(const ScreenDimensions&) = default;
    ScreenDimensions& operator=(ScreenDimensions&&) = default;

    ScreenDimensions(Kind kind);

    uint32 Width() const;

    uint32 Height() const;

    uint32 SilkscreenHeight() const;

   private:
    uint32 width{0}, height{0}, silkscreenHeight{0};
};

#endif  // _SCREEN_DIMENSIONS_H_
