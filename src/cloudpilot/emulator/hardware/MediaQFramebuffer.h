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
    bool DecodeFrame(Frame& frame, uint32 rowBytes, uint32 bpp, bool fullRefresh);

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
    bool flipX = static_cast<T*>(this)->FlipX();
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
        rowBytes == (swapXY ? ((height * bpp) / (8 * scaleY)) : ((width * bpp) / (8 * scaleX)));

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

    // We combine those four flags in to a nibble and instantiate a template in a switch block
    // in order to generate optimized code paths for the various combinations.
    const uint8 variant = (flipX ? 0x08 : 0x00) | (flipY ? 0x04 : 0x00) | (swapXY ? 0x02 : 0x00) |
                          (trivialPitch ? 0x01 : 0x00);

#define MQ_RENDER_VARIANT(x)                                                             \
    case x:                                                                              \
        return DecodeFrame<(x >> 3) & 0x01, (x >> 2) & 0x01, (x >> 1) & 0x01, x & 0x01>( \
            frame, rowBytes, bpp, fullRefresh);

    switch (variant) {
        MQ_RENDER_VARIANT(0x00);
        MQ_RENDER_VARIANT(0x01);
        MQ_RENDER_VARIANT(0x02);
        MQ_RENDER_VARIANT(0x03);
        MQ_RENDER_VARIANT(0x04);
        MQ_RENDER_VARIANT(0x05);
        MQ_RENDER_VARIANT(0x06);
        MQ_RENDER_VARIANT(0x07);
        MQ_RENDER_VARIANT(0x08);
        MQ_RENDER_VARIANT(0x09);
        MQ_RENDER_VARIANT(0x0a);
        MQ_RENDER_VARIANT(0x0b);
        MQ_RENDER_VARIANT(0x0c);
        MQ_RENDER_VARIANT(0x0d);
        MQ_RENDER_VARIANT(0x0e);
        MQ_RENDER_VARIANT(0x0f);
        default:
            EmAssert(false);
            return false;
    }
#undef MQ_RENDER_VARIANT
}

template <class T>
template <bool flipX, bool flipY, bool swapXY, bool trivialPitch>
bool MediaQFramebuffer<T>::DecodeFrame(Frame& frame, uint32 rowBytes, uint32 bpp,
                                       bool fullRefresh) {
    // The internal geometry of the image is rotated by 90° if swapXY is trze.
    const uint32 lines = swapXY ? frame.lineWidth : frame.lines;
    const uint32 lineWidth = swapXY ? frame.lines : frame.lineWidth;

    // A nontrivial pitch leads to padding that needs to be taken into account during scan.
    const uint32 pitchDelta = rowBytes - lineWidth * bpp / 8;

    emuptr baseAddr = static_cast<T*>(this)->GetFrameBuffer();

    // We always render from memory in strictly ascending order and apply possible flips
    // when writing the destination pixels.

    // Flip y axis? -> the MQ decrements y during scan, so shift offset to the address
    // of the first line
    if constexpr (flipY) baseAddr -= (lines - 1) * rowBytes;

    // Flip x axis? -> the MQ decrements x during scan, so shift offset to the address if the
    // first pixel. Padding from a nontrivial pitch comes *at the beginning*, so compensate
    // for that.
    if constexpr (flipX) baseAddr -= rowBytes - (bpp == 16 ? 2 : 1) - pitchDelta;

    const emuptr framebufferBase = static_cast<T*>(this)->GetFrameBufferBase();
    const uint32 framebufferSize = static_cast<T*>(this)->GetFrameBufferSize();

    // Make sure that we don't overflow the framebuffer.
    if (baseAddr < framebufferBase ||
        baseAddr + lines * rowBytes - pitchDelta >= framebufferBase + framebufferSize) {
        return false;
    }

    // Caclulate the dirty region.
    frame.UpdateDirtyLines(gSystemState, baseAddr, rowBytes, fullRefresh, swapXY);
    if (!frame.hasChanges) return true;

    const uint32 firstLine = frame.firstDirtyLine;
    const uint32 lastLine = frame.lastDirtyLine;

    // The frontend code cannot deal with a vertically oriented dirty region, so always do a
    // full update in the frontend if swapXY is true.
    if constexpr (swapXY) frame.ResetDirtyRegion();

    // If flipY is true, the actual image is flipped relative to our scanning direction, so
    // flip the dirty region for the frontend.
    if constexpr (!swapXY && flipY) frame.FlipDirtyRegion();

    uint32* destBuffer = reinterpret_cast<uint32*>(frame.GetBuffer());

    // If the image is not transformed we can skip recalculating the offset and just increment
    // the pointer. In this case we need to fast-forward the pointer to the start of relevant
    // region.
    if constexpr (!swapXY && flipX == flipY) destBuffer += frame.firstDirtyLine * frame.lineWidth;

    // We can do the same trick in case of a point reflection by *decrementing* the pointer instead
    // -> forward the pointer to the *end* of the relevant region.
    if constexpr (!swapXY && flipX && flipY)
        destBuffer += (frame.lastDirtyLine - frame.firstDirtyLine + 1) * frame.lineWidth - 1;

    switch (bpp) {
        case 1: {
            static_cast<T*>(this)->PrvUpdatePalette();
            Nibbler<1, true> nibbler;
            nibbler.reset(
                static_cast<T*>(this)->framebuffer.GetRealAddress(baseAddr + firstLine * rowBytes),
                0);

            for (uint32 y = firstLine; y <= lastLine; y++) {
                for (uint32 x = 0; x < lineWidth; x++)
                    UpdatePixel<flipX, flipY, swapXY>(
                        destBuffer, frame, x, y, static_cast<T*>(this)->palette[nibbler.nibble()]);

                if constexpr (!trivialPitch) nibbler.skipBytes(pitchDelta);
            }

            break;
        }

        case 2: {
            static_cast<T*>(this)->PrvUpdatePalette();
            Nibbler<2, true> nibbler;
            nibbler.reset(
                static_cast<T*>(this)->framebuffer.GetRealAddress(baseAddr + firstLine * rowBytes),
                0);

            for (uint32 y = firstLine; y <= lastLine; y++) {
                for (uint32 x = 0; x < lineWidth; x++)
                    UpdatePixel<flipX, flipY, swapXY>(
                        destBuffer, frame, x, y, static_cast<T*>(this)->palette[nibbler.nibble()]);

                if constexpr (!trivialPitch) nibbler.skipBytes(pitchDelta);
            }

            break;
        }

        case 4: {
            static_cast<T*>(this)->PrvUpdatePalette();
            Nibbler<4, true> nibbler;
            nibbler.reset(
                static_cast<T*>(this)->framebuffer.GetRealAddress(baseAddr + firstLine * rowBytes),
                0);

            for (uint32 y = firstLine; y <= lastLine; y++) {
                for (uint32 x = 0; x < lineWidth; x++)
                    UpdatePixel<flipX, flipY, swapXY>(
                        destBuffer, frame, x, y, static_cast<T*>(this)->palette[nibbler.nibble()]);

                if constexpr (!trivialPitch) nibbler.skipBytes(pitchDelta);
            }

            break;
        }

        case 8: {
            static_cast<T*>(this)->PrvUpdatePalette();

            uint8* srcBuffer =
                static_cast<T*>(this)->framebuffer.GetRealAddress(baseAddr + firstLine * rowBytes);

            for (uint32 y = firstLine; y <= lastLine; y++) {
                for (uint32 x = 0; x < lineWidth; x++)
                    // Pixels are arranged in LE words in the framebuffer, so byteswap
                    UpdatePixel<flipX, flipY, swapXY>(
                        destBuffer, frame, x, y,
                        static_cast<T*>(this)->palette[*(uint8*)((long)(srcBuffer++) ^ 1)]);

                if constexpr (!trivialPitch) srcBuffer += pitchDelta;
            }

            break;
        }

        default: {
            uint8* srcBuffer =
                static_cast<T*>(this)->framebuffer.GetRealAddress(baseAddr + firstLine * rowBytes);

            for (uint32 y = firstLine; y <= lastLine; y++) {
                for (uint32 x = 0; x < lineWidth; x++) {
                    // Pixel data is LE, so byteswap
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
    // No transformation? -> stream pixels to the destination in ascending direction
    if constexpr (!flipX && !flipY && !swapXY) {
        *(destBuffer++) = value;
    }

    // Point reflection? -> stream pixels to the destination in ascending direction
    else if constexpr (flipX && flipY & !swapXY) {
        *(destBuffer--) = value;
    }

    // For more complex transformations we recalculate the offset for every pixel.
    // Swapped axes? -> Swap x and y
    else if constexpr (swapXY) {
        if constexpr (swapXY && flipY) y = frame.lineWidth - 1 - y;
        if constexpr (swapXY && flipX) x = frame.lines - 1 - x;

        *(destBuffer + x * frame.lineWidth + y) = value;
    }

    // No swap? -> just calculate the address from x and y
    else {
        if constexpr (flipX) x = frame.lineWidth - 1 - x;
        if constexpr (flipY) y = frame.lines - 1 - y;

        *(destBuffer + y * frame.lineWidth + x) = value;
    }
}

#endif
