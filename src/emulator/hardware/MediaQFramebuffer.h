#ifndef _MEDIA_Q_FRAMEBUFFER_H_
#define _MEDIA_Q_FRAMEBUFFER_H_

#include "EmDevice.h"
#include "EmHAL.h"
#include "EmRegsFrameBuffer.h"
#include "EmSession.h"
#include "EmSystemState.h"
#include "Frame.h"
#include "Nibbler.h"
#include "ScreenDimensions.h"

template <class T>
class MediaQFramebuffer : public EmHALHandler {
   protected:
    MediaQFramebuffer() = default;

   public:
    virtual bool CopyLCDFrame(Frame& frame, bool fullRefresh) override;
};

///////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
///////////////////////////////////////////////////////////////////////////////

template <class T>
bool MediaQFramebuffer<T>::CopyLCDFrame(Frame& frame, bool fullRefresh) {
    const int32 bpp = static_cast<T*>(this)->GetBPP();
    const int32 height = static_cast<T*>(this)->GetHeight();
    const int32 rowBytes = static_cast<T*>(this)->GetRowBytes();

    if (rowBytes <= 0) return false;

    int32 width = std::min(
        static_cast<T*>(this)->GetWidth(),
        static_cast<uint32>(rowBytes * 8 * (static_cast<T*>(this)->GetXDoubling() ? 2 : 1) / bpp));
    emuptr baseAddr = static_cast<T*>(this)->GetFrameBuffer();

    EmAssert(gSession);
    const ScreenDimensions screenDimensions(gSession->GetDevice().GetScreenDimensions());

    if (width != static_cast<int32>(screenDimensions.Width()) ||
        height != static_cast<int32>(screenDimensions.Height())) {
        return false;
    }

    frame.scaleX = static_cast<T*>(this)->GetXDoubling() ? 2 : 1;
    frame.scaleY = static_cast<T*>(this)->GetYDoubling() ? 2 : 1;
    frame.bpp = 24;
    frame.lineWidth = width / frame.scaleX;
    frame.lines = height / frame.scaleY;
    frame.margin = 0;
    frame.bytesPerLine = frame.lineWidth * 4;
    frame.hasChanges = true;

    if (4 * frame.lineWidth * frame.lines > frame.GetBufferSize()) return false;

    if (!gSystemState.IsScreenDirty() && !fullRefresh) {
        frame.hasChanges = false;
        return true;
    }

    if (gSystemState.ScreenRequiresFullRefresh() || fullRefresh) {
        frame.firstDirtyLine = 0;
        frame.lastDirtyLine = frame.lines - 1;
    } else {
        if (gSystemState.GetScreenHighWatermark() < baseAddr) {
            frame.hasChanges = false;
            return true;
        }

        frame.firstDirtyLine =
            min((max(gSystemState.GetScreenLowWatermark(), baseAddr) - baseAddr) / rowBytes,
                frame.lines - 1);

        frame.lastDirtyLine =
            min((gSystemState.GetScreenHighWatermark() - baseAddr) / rowBytes, frame.lines - 1);
    }

    uint32* destBuffer =
        reinterpret_cast<uint32*>(frame.GetBuffer() + frame.firstDirtyLine * frame.bytesPerLine);

    switch (bpp) {
        case 1: {
            static_cast<T*>(this)->PrvUpdatePalette();
            Nibbler<1, true> nibbler;
            nibbler.reset(static_cast<T*>(this)->framebuffer.GetRealAddress(
                              baseAddr + frame.firstDirtyLine * rowBytes),
                          0);

            for (uint32 y = frame.firstDirtyLine; y <= frame.lastDirtyLine; y++)
                for (uint32 x = 0; x < frame.lineWidth; x++)
                    *(destBuffer++) = static_cast<T*>(this)->palette[nibbler.nibble()];

            break;
        }

        case 2: {
            static_cast<T*>(this)->PrvUpdatePalette();
            Nibbler<2, true> nibbler;
            nibbler.reset(static_cast<T*>(this)->framebuffer.GetRealAddress(
                              baseAddr + frame.firstDirtyLine * rowBytes),
                          0);

            for (uint32 y = frame.firstDirtyLine; y <= frame.lastDirtyLine; y++)
                for (uint32 x = 0; x < frame.lineWidth; x++)
                    *(destBuffer++) = static_cast<T*>(this)->palette[nibbler.nibble()];

            break;
        }

        case 4: {
            static_cast<T*>(this)->PrvUpdatePalette();
            Nibbler<4, true> nibbler;
            nibbler.reset(static_cast<T*>(this)->framebuffer.GetRealAddress(
                              baseAddr + frame.firstDirtyLine * rowBytes),
                          0);

            for (uint32 y = frame.firstDirtyLine; y <= frame.lastDirtyLine; y++)
                for (uint32 x = 0; x < frame.lineWidth; x++)
                    *(destBuffer++) = static_cast<T*>(this)->palette[nibbler.nibble()];

            break;
        }

        case 8: {
            static_cast<T*>(this)->PrvUpdatePalette();

            uint8* srcBuffer = static_cast<T*>(this)->framebuffer.GetRealAddress(
                baseAddr + frame.firstDirtyLine * rowBytes);

            for (uint32 y = frame.firstDirtyLine; y <= frame.lastDirtyLine; y++)
                for (uint32 x = 0; x < frame.lineWidth; x++)
                    *(destBuffer++) =
                        static_cast<T*>(this)->palette[*(uint8*)((long)(srcBuffer++) ^ 1)];

            break;
        }

        default: {
            uint8* srcBuffer = static_cast<T*>(this)->framebuffer.GetRealAddress(
                baseAddr + frame.firstDirtyLine * rowBytes);

            for (uint32 y = frame.firstDirtyLine; y <= frame.lastDirtyLine; y++)
                for (uint32 x = 0; x < frame.lineWidth; x++) {
                    uint8 p1 = *(uint8*)((long)(srcBuffer++) ^ 1);  // GGGBBBBB
                    uint8 p2 = *(uint8*)((long)(srcBuffer++) ^ 1);  // RRRRRGGG

                    // Merge the two together so that we get RRRRRGGG GGGBBBBB

                    uint16 p;

                    p = (p1 << 8) | p2;

                    // Shift the bits around, forming RRRRRrrr, GGGGGGgg, and
                    // BBBBBbbb values, where the lower-case bits are copies of
                    // the least significant bits in the upper-case bits.
                    //
                    // Note that all of this could also be done with three 64K
                    // lookup tables.  If speed is an issue, we might want to
                    // investigate that.

                    *(destBuffer++) = 0xff000000 | ((((p << 3) & 0xF8) | ((p >> 0) & 0x07)) << 16) |
                                      ((((p >> 3) & 0xFC) | ((p >> 5) & 0x03)) << 8) |
                                      (((p >> 8) & 0xF8) | ((p >> 11) & 0x07));
                }

            break;
        }
    }

    return true;
}

#endif
