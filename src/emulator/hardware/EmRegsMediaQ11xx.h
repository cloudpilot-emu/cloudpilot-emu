/* -*- mode: C++; tab-width: 4 -*- */
/* ===================================================================== *\
        Copyright (c) 2001 Palm, Inc. or its subsidiaries.
        All rights reserved.

        This file is part of the Palm OS Emulator.

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.
\* ===================================================================== */

#ifndef EmRegsMediaQ11xx_h
#define EmRegsMediaQ11xx_h

#include <vector>  // vector

#include "EmHAL.h"          // EmHALHandler
#include "EmPalmStructs.h"  // EmProxyHwrMediaQ11xType
#include "EmRegs.h"         // EmRegs
#include "EmStructs.h"

#define T_BASE 0x1F000000L   // 68KVZ for Palm platform
#define MMIO_OFFSET 0x40000  // starts at 256K address
#define MMIO_BASE (T_BASE + MMIO_OFFSET)

// This struct is a mirror of the MediaQ graphics enginer registers.  As
// the registers  are updated, this struct is updated so that we have
// easy access to the values in the registers without having to unpack
// them all the time.

class EmRegsFrameBuffer;

struct GEState {
    //	*	means that I've completely accounted for that field
    //		and don't have to think about it any more.
    //
    //	-	means that I'm not going to support that field and
    //		so don't ever have to think about it.
    //
    //	?	means that I don't know what this field is for.

    // ==================== GE00R ====================

    uint8 rasterOperation;     // [7:0]*
    uint8 commandType;         // [10:8]*
    uint8 xDirection;          // [11]*
    uint8 yDirection;          // [12]*
    uint8 systemMemory;        // [13]*
    uint8 monoSource;          // [14]*
    uint8 monoPattern;         // [15]*
    uint8 colorTransEnable;    // [16]*
    uint8 destTransPolarity;   // [17]*
    uint8 monoTransEnable;     // [18]*
    uint8 monoTransPolarity;   // [19]*
    uint8 memToScreen;         // [20]*
    uint8 solidSourceColor;    // [23]*
    uint8 srcEqualDestStride;  // [24]*
    uint8 rop2Select;          // [25]?
    uint8 clipEnable;          // [26]*
    uint8 autoExecute;         // [27]*
    uint8 solidPattern;        // [30]*
    uint8 colorTransCmpSrc;    // [31]*

    // ==================== GE01R ====================

    // BitBLT

    uint16 width;        // [11:0]*
    uint16 height;       // [27:16]*
    uint8 xyConversion;  // [31]*

    // Line

    int32 gamma;          // [16:0]?
    uint16 majorLength;   // [28:17]*
    uint8 yIsMajor;       // [29]*
    uint8 drawLastPixel;  // [30]*
    uint8 useXY;          // [31]*

    // ==================== GE02R ====================

    // BitBLT

    uint16 xDest;              // [11:0]*
    uint8 monoPatternXOffset;  // [15:13]*
    uint16 yDest;              // [27:16]*
    uint8 monoPatternYOffset;  // [31:29]*

    // Line

    uint16 xStart;      // [11:0]*
    uint16 deltaMajor;  // [28:12]*
    uint16 quadrant;    // [31:29]*

    // ==================== GE03R ====================

    // BitBLT

    uint16 xSrc;  // [11:0]*
    uint16 ySrc;  // [27:16]*

    // Line

    uint16 yStart;      // [11:0]*
    uint16 deltaMinor;  // [28:12]*

    // ==================== GE04R ====================

    uint16 destTransColor;  // [15:0]*

    // ==================== GE05R ====================

    uint16 clipLeft;  // [10:0]*
    uint16 clipTop;   // [25:16]*

    // ==================== GE06R ====================

    uint16 clipRight;   // [10:0]*
    uint16 clipBottom;  // [25:16]*

    // ==================== GE07R ====================

    uint16 fgColorMonoSrc;  // [15:0]*

    // ==================== GE08R ====================

    uint16 bgColorMonoSrc;  // [15:0]*

    // ==================== GE09R ====================

    // Lined Mode

    uint16 srcLineStride;  // [9:0]*
    uint8 srcBitOffset;    // [27:25]*
    uint8 srcByteOffset;   // [30:28]*

    // Packed Mode

    uint8 srcLeadingBits;    // [2:0]*
    uint8 srcLeadingBytes;   // [5:3]*
    uint16 srcNumBytes;      // [15:6]*
    uint8 srcTrailingBits;   // [27:25]*
    uint8 srcTrailingBytes;  // [31:28]*

    // ==================== GE0AR ====================

    uint16 destLineStride;  // [9:0]*
    uint8 monoSrcBitSwap;   // [28]*
    uint8 rotate90;         // [29]*
    uint8 colorDepth;       // [31:30]*

    // ==================== GE0BR ====================

    uint32 baseAddr;        // [19:0]*
    uint8 testModeEnable;   // [29]-
    uint8 testModeControl;  // [31:30]-

    // ==================== GE0CR ====================

    uint16 cmdLineStart;     // [9:0]-
    uint16 cmdLineEnd;       // [21:12]-
    uint8 cmdLineControl;    // [24]-
    uint8 gc1SwitchControl;  // [27:26]-

    // ==================== GE0FR ====================

    // Test register

    // ==================== GE10R ====================

    uint32 monoPattern1;  // [31:0]*

    // ==================== GE11R ====================

    uint32 monoPattern2;  // [31:0]*

    // ==================== GE12R ====================

    uint16 fgColorMonoPat;  // [15:0]*

    // ==================== GE13R ====================

    uint16 bgColorMonoPat;  // [15:0]*
};

class EmRegsMediaQ11xx : public EmRegs, public EmHALHandler {
   public:
    EmRegsMediaQ11xx(EmRegsFrameBuffer& framebuffer, emuptr baseRegsAddr, emuptr baseVideoAddr);
    virtual ~EmRegsMediaQ11xx(void);

    // EmRegs overrides
    virtual void Initialize(void);
    virtual void Reset(Bool hardwareReset);
    virtual void Save(Savestate&);
    virtual void Save(SavestateProbe&);
    virtual void Load(SavestateLoader&);
    virtual void Dispose(void);

    virtual void SetSubBankHandlers(void);
    virtual uint8* GetRealAddress(emuptr address);
    virtual emuptr GetAddressStart(void);
    virtual uint32 GetAddressRange(void);

    // EmHAL overrides
    virtual Bool GetLCDScreenOn(void);
    virtual Bool GetLCDBacklightOn(void);
    virtual Bool GetLCDHasFrame(void);
    virtual void GetLCDBeginEnd(emuptr& begin, emuptr& end);
    virtual bool CopyLCDFrame(Frame& frame);
    virtual uint16 GetLCD2bitMapping();

   protected:
    uint32 MQRead(emuptr address, int size);
    void MQWrite(emuptr address, int size, uint32 value);

   private:
    uint32 CC01Read(emuptr address, int size);

    void DC00Write(emuptr address, int size, uint32 value);
    void GE00Write(emuptr address, int size, uint32 value);
    void GE02Write(emuptr address, int size, uint32 value);
    void SourceFifoWrite(emuptr address, int size, uint32 value);
    void invalidateWrite(emuptr address, int size, uint32 value);

   private:
    template <typename T>
    void DoSave(T& savestate);

    template <typename T>
    void DoSaveLoad(T& helper, uint32 version);

    void AddressError(emuptr address, long size, Bool forRead);
    void PrvUpdatePalette();
    void PrvUpdateByteLanes(void);

    uint32 PrvGetBPP(void);
    uint32 PrvGetWidth(void);
    uint32 PrvGetHeight(void);
    uint32 PrvGetRowBytes(void);
    uint32 PrvGetVideoOffset(void);
    uint32 PrvGetVideoBase(void);
    uint32 PrvGetFrameBuffer(void);
    Bool PrvGetXDoubling(void);
    Bool PrvGetYDoubling(void);

    void PrvGetGEState(int regNum);
    void PrvLogGEState();

    void PrvDoCommand(void);
    void PrvDoBitBLT(void);
    void PrvDoLine(void);
    void PrvIllegalCommand(void);

    int PrvSrcFifoFilledSlots(void);
    void PrvGetSrcFifoSlot(uint32&, uint32&);

    // Drawing related functions
    uint16 PrvAdjustPixel(uint16 pen, uint16 dest, uint8 rOpCode);
    uint16 PrvAdjustPixel(uint16 pattern, uint16 src, uint16 dest, uint8 rOpCode);

    void PrvSetPixel(uint16 pixel, uint16 x, uint16 y);
    uint16 PrvGetPixel(uint16 x, uint16 y);
    emuptr PrvGetPixelLocation(uint16 x, uint16 y);

    void PrvIncBlitterInit(void);
    void PrvIncBlitterRun(void);

    void PrvPatternPipeInit(void);
    uint16 PrvPatternPipeNextPixel(void);
    void PrvPatternPipeNextX(void);
    void PrvPatternPipeNextY(void);

    void PrvSrcPipeInit(void);
    uint16 PrvSrcPipeNextPixel(Bool& stalled);
    void PrvSrcPipeNextX(void);
    void PrvSrcPipeNextY(void);
    void PrvSrcPipeFill(Bool& stalled);

    void PrvDestPipeInit(void);
    uint16 PrvDestPipeNextPixel(void);
    void PrvDestPipeNextX(void);
    void PrvDestPipeNextY(void);

    Bool PrvNextXY(void);
    Bool PrvTransparent(uint16 source, uint16 dest, uint16 pattern);
    Bool PrvClipped(void);
    uint16 PrvLeadingPixels(void);
    uint16 PrvTrailingPixels(void);

    Bool PrvUsesPattern(void);
    Bool PrvUsesSource(void);
    void PrvExpandMono8(uint8 bits, uint16* results, uint16 fgColor, uint16 bgColor);
    void PrvExpandMono32(uint32 bits, uint16* results, uint16 fgColor, uint16 bgColor);

   private:
    emuptr fBaseRegsAddr;
    emuptr fBaseVideoAddr;
    EmProxyHwrMediaQ11xxType fRegs;
    GEState fState;

    int fBytelanes[4];
    emuptr fLastAddress;
    int fLastSize;

    vector<uint32> fSourceFifo;

    // Values used while incrementally blitting.

    Bool fBlitInProgress;

    uint16 fCurXOffset;
    uint16 fCurYOffset;

    // Some values cached for speed.  These are calculated once
    // for each blit operation and saved, as they don't vary once
    // the blit has started.  However, there's no evidence that
    // any such caching is needed -- no timing experiments have
    // bee performed.

    Bool fUsesPattern;
    Bool fUsesSource;

    uint16 fLeadingSourcePixels;
    uint16 fTrailingSourcePixels;

    // Pattern pipe data

    uint16 fPatternPipe[64];
    uint16 fXPattern;
    uint16 fYPattern;

    // Source pipe data

    uint16 fSourcePipe[64];
    uint16 fSourcePipeIndex;
    uint16 fSourcePipeMax;
    uint16 fSourcePipeSkip;
    uint16 fXSrc;
    uint16 fYSrc;

    // Dest pipe data

    uint16 fXDest;
    uint16 fYDest;

    uint32 palette[256];
    EmRegsFrameBuffer& framebuffer;
};

#endif  // EmRegsMediaQ11xx_h
