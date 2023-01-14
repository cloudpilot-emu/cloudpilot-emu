#ifndef _MEDIA_Q_FRAMEBUFFER_H_
#define _MEDIA_Q_FRAMEBUFFER_H_

#include <cstdint>

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

   private:
    template <bool flipX, bool flipY, bool swapXY, bool trivialPitch>
    bool DecodeFrame(Frame& frame, uint32 rowBytes, uint32 bpp);

    template <bool flipX, bool flipY, bool swapXY>
    inline void UpdatePixel(uint32*& destBuffer, Frame& frame, uint32 x, uint32 y, uint32 value);
};

///////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
///////////////////////////////////////////////////////////////////////////////

template <class T>
bool MediaQFramebuffer<T>::CopyLCDFrame(Frame& frame, bool fullRefresh) {
    const int16 pitch = static_cast<T*>(this)->GetPitch();
    const bool flipY = pitch < 0;
    const bool flipX = static_cast<T*>(this)->FlipX();
    const bool swapXY = static_cast<T*>(this)->SwapXY();
    const uint8 scaleX = static_cast<T*>(this)->GetXDoubling() ? 2 : 1;
    const uint8 scaleY = static_cast<T*>(this)->GetYDoubling() ? 2 : 1;
    const uint32 bpp = static_cast<T*>(this)->GetBPP();
    const uint32 rowBytes = pitch < 0 ? -pitch : pitch;
    const uint32 height =
        swapXY ? std::min(static_cast<T*>(this)->GetHeight(), (rowBytes * 8 * scaleY) / bpp)
               : static_cast<T*>(this)->GetHeight();
    const uint32 width =
        swapXY ? static_cast<T*>(this)->GetWidth()
               : std::min(static_cast<T*>(this)->GetWidth(), (rowBytes * scaleX * 8) / bpp);
    const bool trivialPitch =
        rowBytes == swapXY ? ((height * bpp) / (8 * scaleY)) : ((width * bpp) / (8 * scaleX));

    EmAssert(gSession);
    const ScreenDimensions screenDimensions(gSession->GetDevice().GetScreenDimensions());

    if (width != screenDimensions.Width() || height != screenDimensions.Height()) {
        return false;
    }

    frame.scaleX = scaleX;
    frame.scaleY = scaleY;
    frame.bpp = 24;
    frame.lineWidth = width / frame.scaleX;
    frame.lines = height / frame.scaleY;
    frame.margin = 0;
    frame.bytesPerLine = frame.lineWidth * 4;
    frame.hasChanges = true;

    if (4 * frame.lineWidth * frame.lines > frame.GetBufferSize()) return false;

    frame.UpdateDirtyLines(gSystemState, static_cast<T*>(this)->GetFrameBuffer(), rowBytes,
                           fullRefresh || flipX || flipY || swapXY);
    if (!frame.hasChanges) return true;

    const uint8 variant = (flipX ? 0x08 : 0x00) | (flipY ? 0x04 : 0x00) | (swapXY ? 0x02 : 0x00) |
                          (trivialPitch ? 0x01 : 0x00);

    switch (variant) {
        case 0x00:
            return DecodeFrame<false, false, false, false>(frame, rowBytes, bpp);

        case 0x01:
            return DecodeFrame<false, false, false, true>(frame, rowBytes, bpp);

        case 0x02:
            return DecodeFrame<false, false, true, false>(frame, rowBytes, bpp);

        case 0x03:
            return DecodeFrame<false, false, true, true>(frame, rowBytes, bpp);

        case 0x04:
            return DecodeFrame<false, true, false, false>(frame, rowBytes, bpp);

        case 0x05:
            return DecodeFrame<false, true, false, true>(frame, rowBytes, bpp);

        case 0x06:
            return DecodeFrame<false, true, true, false>(frame, rowBytes, bpp);

        case 0x07:
            return DecodeFrame<false, true, true, true>(frame, rowBytes, bpp);

        case 0x08:
            return DecodeFrame<true, false, false, false>(frame, rowBytes, bpp);

        case 0x09:
            return DecodeFrame<true, false, false, true>(frame, rowBytes, bpp);

        case 0x0a:
            return DecodeFrame<true, false, true, false>(frame, rowBytes, bpp);

        case 0x0b:
            return DecodeFrame<true, false, true, true>(frame, rowBytes, bpp);

        case 0x0c:
            return DecodeFrame<true, true, false, false>(frame, rowBytes, bpp);

        case 0x0d:
            return DecodeFrame<true, true, false, true>(frame, rowBytes, bpp);

        case 0x0e:
            return DecodeFrame<true, true, true, false>(frame, rowBytes, bpp);

        case 0x0f:
            return DecodeFrame<true, true, true, true>(frame, rowBytes, bpp);

        default:
            EmAssert(false);
            return false;
    }
}

template <class T>
template <bool flipX, bool flipY, bool swapXY, bool trivialPitch>
bool MediaQFramebuffer<T>::DecodeFrame(Frame& frame, uint32 rowBytes, uint32 bpp) {
    emuptr baseAddr = static_cast<T*>(this)->GetFrameBuffer();

    if constexpr (flipY) {
        baseAddr -= (frame.lines - 1) * rowBytes;
    }

    if constexpr (flipX) {
        baseAddr -= rowBytes - (bpp == 16 ? 2 : 1);
    }

    if (baseAddr < static_cast<T*>(this)->GetFramebufferBase()) return false;

    uint32 pitchDelta = rowBytes - frame.lineWidth * bpp / 8;

    uint32* destBuffer =
        reinterpret_cast<uint32*>(frame.GetBuffer() + frame.firstDirtyLine * frame.bytesPerLine);
    if constexpr (flipX && flipY) destBuffer += frame.lines * frame.lineWidth - 1;

    switch (bpp) {
        case 1: {
            static_cast<T*>(this)->PrvUpdatePalette();
            Nibbler<1, true> nibbler;
            nibbler.reset(static_cast<T*>(this)->framebuffer.GetRealAddress(
                              baseAddr + frame.firstDirtyLine * rowBytes),
                          0);

            for (uint32 y = frame.firstDirtyLine; y <= frame.lastDirtyLine; y++) {
                for (uint32 x = 0; x < frame.lineWidth; x++)
                    UpdatePixel<flipX, flipY, swapXY>(
                        destBuffer, frame, x, y, static_cast<T*>(this)->palette[nibbler.nibble()]);

                if constexpr (!trivialPitch) nibbler.skipBytes(pitchDelta);
            }

            break;
        }

        case 2: {
            static_cast<T*>(this)->PrvUpdatePalette();
            Nibbler<2, true> nibbler;
            nibbler.reset(static_cast<T*>(this)->framebuffer.GetRealAddress(
                              baseAddr + frame.firstDirtyLine * rowBytes),
                          0);

            for (uint32 y = frame.firstDirtyLine; y <= frame.lastDirtyLine; y++) {
                for (uint32 x = 0; x < frame.lineWidth; x++)
                    UpdatePixel<flipX, flipY, swapXY>(
                        destBuffer, frame, x, y, static_cast<T*>(this)->palette[nibbler.nibble()]);

                if constexpr (!trivialPitch) nibbler.skipBytes(pitchDelta);
            }

            break;
        }

        case 4: {
            static_cast<T*>(this)->PrvUpdatePalette();
            Nibbler<4, true> nibbler;
            nibbler.reset(static_cast<T*>(this)->framebuffer.GetRealAddress(
                              baseAddr + frame.firstDirtyLine * rowBytes),
                          0);

            for (uint32 y = frame.firstDirtyLine; y <= frame.lastDirtyLine; y++) {
                for (uint32 x = 0; x < frame.lineWidth; x++)
                    UpdatePixel<flipX, flipY, swapXY>(
                        destBuffer, frame, x, y, static_cast<T*>(this)->palette[nibbler.nibble()]);

                if constexpr (!trivialPitch) nibbler.skipBytes(pitchDelta);
            }

            break;
        }

        case 8: {
            static_cast<T*>(this)->PrvUpdatePalette();

            uint8* srcBuffer = static_cast<T*>(this)->framebuffer.GetRealAddress(
                baseAddr + frame.firstDirtyLine * rowBytes);

            for (uint32 y = frame.firstDirtyLine; y <= frame.lastDirtyLine; y++) {
                for (uint32 x = 0; x < frame.lineWidth; x++)
                    UpdatePixel<flipX, flipY, swapXY>(
                        destBuffer, frame, x, y,
                        static_cast<T*>(this)->palette[*(uint8*)((long)(srcBuffer++) ^ 1)]);

                if constexpr (!trivialPitch) srcBuffer += pitchDelta;
            }

            break;
        }

        default: {
            uint8* srcBuffer = static_cast<T*>(this)->framebuffer.GetRealAddress(
                baseAddr + frame.firstDirtyLine * rowBytes);

            for (uint32 y = frame.firstDirtyLine; y <= frame.lastDirtyLine; y++) {
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

                    UpdatePixel<flipX, flipY, swapXY>(
                        destBuffer, frame, x, y,
                        0xff000000 | ((((p << 3) & 0xF8) | ((p >> 0) & 0x07)) << 16) |
                            ((((p >> 3) & 0xFC) | ((p >> 5) & 0x03)) << 8) |
                            (((p >> 8) & 0xF8) | ((p >> 11) & 0x07)));
                }

                if constexpr (!trivialPitch) srcBuffer += pitchDelta;
            }
            break;
        }
    }

    return true;
}

template <class T>
template <bool flipX, bool flipY, bool swapXY>
void MediaQFramebuffer<T>::UpdatePixel(uint32*& destBuffer, Frame& frame, uint32 x, uint32 y,
                                       uint32 value) {
    if constexpr (!flipX && !flipY && !swapXY) *(destBuffer++) = value;
    if constexpr (flipX && flipY & !swapXY) *(destBuffer--) = value;
}

#endif
