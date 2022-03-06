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

   protected:
    template <typename S, int scale>
    bool CopyLCDFrameWithScale(Frame& frame, bool fullRefresh);
};

///////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
///////////////////////////////////////////////////////////////////////////////

template <class T>
bool MediaQFramebuffer<T>::CopyLCDFrame(Frame& frame, bool fullRefresh) {
    class Scaler1x {
       public:
        Scaler1x(uint32* buffer, uint32) : buffer(buffer) {}

        inline void draw(uint32 color) { *(buffer++) = color; }

       private:
        uint32* buffer;
    };

    class Scaler2x {
       public:
        Scaler2x(uint32* buffer, uint32 width) : buf1(buffer), buf2(buffer + width), width(width) {}

        inline void draw(uint32 color) {
            *(buf1++) = *(buf1++) = *(buf2++) = *(buf2++) = color;

            x += 2;
            if (x >= width) {
                x = 0;
                buf1 += width;
                buf2 += width;
            }
        }

       private:
        uint32* buf1;
        uint32* buf2;
        uint32 width;
        uint32 x{0};
    };

    if (static_cast<T*>(this)->GetXDoubling()) {
        return static_cast<T*>(this)->GetYDoubling()
                   ? CopyLCDFrameWithScale<Scaler2x, 2>(frame, fullRefresh)
                   : false;
    }

    return CopyLCDFrameWithScale<Scaler1x, 1>(frame, fullRefresh);
}

template <class T>
template <typename S, int scale>
bool MediaQFramebuffer<T>::CopyLCDFrameWithScale(Frame& frame, bool fullRefresh) {
    int32 bpp = static_cast<T*>(this)->GetBPP();
    int32 height = static_cast<T*>(this)->GetHeight();
    int32 rowBytes = static_cast<T*>(this)->GetRowBytes();

    if (rowBytes <= 0) return false;

    int32 width = std::min(static_cast<T*>(this)->GetWidth(),
                           static_cast<uint32>(rowBytes * 8 * scale / bpp));
    emuptr baseAddr = static_cast<T*>(this)->GetFrameBuffer();

    EmAssert(gSession);
    const ScreenDimensions screenDimensions(gSession->GetDevice().GetScreenDimensions());

    if (width != static_cast<int32>(screenDimensions.Width()) ||
        height != static_cast<int32>(screenDimensions.Height())) {
        return false;
    }

    frame.bpp = 24;
    frame.lineWidth = width;
    frame.lines = height;
    frame.margin = 0;
    frame.bytesPerLine = width * 4;
    frame.hasChanges = true;

    if (4 * width * height > static_cast<ssize_t>(frame.GetBufferSize())) return false;

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

        if constexpr (scale == 2) {
            frame.firstDirtyLine =
                2 * min((max(gSystemState.GetScreenLowWatermark(), baseAddr) - baseAddr) / rowBytes,
                        frame.lines / 2 - 1);

            frame.lastDirtyLine =
                2 * min((gSystemState.GetScreenHighWatermark() - baseAddr) / rowBytes,
                        frame.lines / 2 - 1) +
                1;
        } else {
            frame.firstDirtyLine =
                min((max(gSystemState.GetScreenLowWatermark(), baseAddr) - baseAddr) / rowBytes,
                    frame.lines - 1);

            frame.lastDirtyLine =
                min((gSystemState.GetScreenHighWatermark() - baseAddr) / rowBytes, frame.lines - 1);
        }
    }

    S scaler(
        reinterpret_cast<uint32*>(frame.GetBuffer() + frame.firstDirtyLine * frame.bytesPerLine),
        width);

    switch (bpp) {
        case 1: {
            static_cast<T*>(this)->PrvUpdatePalette();
            Nibbler<1, true> nibbler;
            nibbler.reset(static_cast<T*>(this)->framebuffer.GetRealAddress(
                              baseAddr + frame.firstDirtyLine / scale * rowBytes),
                          0);

            for (uint32 y = frame.firstDirtyLine / scale; y <= frame.lastDirtyLine / scale; y++)
                for (int32 x = 0; x < width / scale; x++)
                    scaler.draw(static_cast<T*>(this)->palette[nibbler.nibble()]);

            break;
        }

        case 2: {
            static_cast<T*>(this)->PrvUpdatePalette();
            Nibbler<2, true> nibbler;
            nibbler.reset(static_cast<T*>(this)->framebuffer.GetRealAddress(
                              baseAddr + frame.firstDirtyLine / scale * rowBytes),
                          0);

            for (uint32 y = frame.firstDirtyLine / scale; y <= frame.lastDirtyLine / scale; y++)
                for (int32 x = 0; x < width / scale; x++)
                    scaler.draw(static_cast<T*>(this)->palette[nibbler.nibble()]);

            break;
        }

        case 4: {
            static_cast<T*>(this)->PrvUpdatePalette();
            Nibbler<4, true> nibbler;
            nibbler.reset(static_cast<T*>(this)->framebuffer.GetRealAddress(
                              baseAddr + frame.firstDirtyLine / scale * rowBytes),
                          0);

            for (uint32 y = frame.firstDirtyLine / scale; y <= frame.lastDirtyLine / scale; y++)
                for (int32 x = 0; x < width / scale; x++)
                    scaler.draw(static_cast<T*>(this)->palette[nibbler.nibble()]);

            break;
        }

        case 8: {
            static_cast<T*>(this)->PrvUpdatePalette();
            uint8* buffer = static_cast<T*>(this)->framebuffer.GetRealAddress(
                baseAddr + frame.firstDirtyLine / scale * rowBytes);

            for (uint32 y = frame.firstDirtyLine / scale; y <= frame.lastDirtyLine / scale; y++)
                for (int32 x = 0; x < width / scale; x++)
                    scaler.draw(static_cast<T*>(this)->palette[*(uint8*)((long)(buffer++) ^ 1)]);

            break;
        }

        default: {
            uint8* buffer = static_cast<T*>(this)->framebuffer.GetRealAddress(
                baseAddr + frame.firstDirtyLine / scale * rowBytes);

            for (uint32 y = frame.firstDirtyLine / scale; y <= frame.lastDirtyLine / scale; y++)
                for (int32 x = 0; x < width / scale; x++) {
                    uint8 p1 = *(uint8*)((long)(buffer++) ^ 1);  // GGGBBBBB
                    uint8 p2 = *(uint8*)((long)(buffer++) ^ 1);  // RRRRRGGG

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

                    scaler.draw(0xff000000 | ((((p << 3) & 0xF8) | ((p >> 0) & 0x07)) << 16) |
                                ((((p >> 3) & 0xFC) | ((p >> 5) & 0x03)) << 8) |
                                (((p >> 8) & 0xF8) | ((p >> 11) & 0x07)));
                }

            break;
        }
    }

    return true;
}

#endif
