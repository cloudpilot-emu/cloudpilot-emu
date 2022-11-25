#include "ScreenDimensions.h"

ScreenDimensions::ScreenDimensions(Kind kind) {
    switch (kind) {
        case screen160x160:
            width = height = 160;
            silkscreenHeight = 60;
            break;

        case screen320x320:
            width = height = 320;
            silkscreenHeight = 120;
            break;

        case screen240x320:
            width = 240;
            height = 320;
            silkscreenHeight = 0;
            break;

        case screen320x480:
            width = 320;
            height = 480;
            silkscreenHeight = 0;
            break;
    }
}

uint32 ScreenDimensions::Width() const { return width; }

uint32 ScreenDimensions::Height() const { return height; }

uint32 ScreenDimensions::SilkscreenHeight() const { return silkscreenHeight; }
