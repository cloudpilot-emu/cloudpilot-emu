/* -*- mode: C++; tab-width: 4 -*- */
/* ===================================================================== *\
        Copyright (c) 2000-2001 Palm, Inc. or its subsidiaries.
        All rights reserved.

        This file is part of the Palm OS Emulator.

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.
\* ===================================================================== */

#include "EmRegsSZ.h"

#include <algorithm>
#include <cmath>

#include "Byteswapping.h"  // Canonical
#include "ChunkHelper.h"
#include "EmCommon.h"
#include "EmHAL.h"     // EmHAL
#include "EmMemory.h"  // gMemAccessFlags, EmMem_memcpy
#include "EmRegsSZPrv.h"
#include "EmSPISlave.h"  // DoExchange
#include "EmSession.h"   // gSession
#include "EmSystemState.h"
#include "Frame.h"
#include "Logging.h"  // LogAppendMsg
#include "MetaMemory.h"
#include "Miscellaneous.h"  // GetHostTime
#include "Nibbler.h"
#include "Savestate.h"
#include "SavestateLoader.h"
#include "SavestateProbe.h"
#include "SavestateStructures.h"
#include "StackDump.h"
#include "UAE.h"  // regs, SPCFLAG_INT

static const uint16 ROPMask = 0x2000;    // Make to get the Read-Only-Protect bit.
static const uint16 UPSIZMask = 0x1800;  // Mask to get the unprotected memory size from csESelect.
static const uint16 UPSIZShift = 11;
static const uint16 SIZMask = 0x000E;  // Mask to get the memory size from csASelect.
static const uint16 ENMask = 0x0001;   // Mask to get the enable bit from csASelect.

static const int SIZShift = 1;
// static const int ENShift = 0;

static const uint16 EUPS2Mask = 0x0020;
static const uint16 EUPS2Shift = 5;
static const uint16 EUPENMask = 0x4000;

// static const int EUPS2Shift = 5;

static const int kBaseAddressShift = 16;  // Shift to get base address from CSGBx register value

// #define LOGGING 0
#ifdef LOGGING
    #define PRINTF logging::printf
#else
    #define PRINTF(...) ;
#endif

namespace {
    constexpr uint32 SAVESTATE_VERSION = 2;

    double TimerTicksPerSecond(uint16 tmrControl, uint16 tmrPrescaler, int32 systemClockFrequency) {
        uint8 clksource = (tmrControl >> 1) & 0x7;
        double prescaler = ((tmrPrescaler & 0xff) + 1);

        switch (clksource) {
            case 0x1:
                return (double)systemClockFrequency / prescaler;

            case 0x2:
                return (double)systemClockFrequency / prescaler / 16.;

            default:
                return (clksource & 0x4) ? 32768. / prescaler : 0;
        }
    }

    void cycleThunk(void* context, uint64 cycles, bool sleeping) {
        ((EmRegsSZ*)context)->Cycle(cycles, sleeping);
    }

    template <class T>
    void markScreenWith(T marker, HwrM68SZ328Type& f68SZ328Regs) {
        const uint16 screenHeight = READ_REGISTER(lcdScreenSize) & 0x01ff;
        const uint16 virtualPageWidth = (READ_REGISTER(lcdPageWidth) & 0x03ff) * 2;

        const emuptr firstLineAddr = READ_REGISTER(lcdStartAddr) & ~1;
        const emuptr lastLineAddr = firstLineAddr + (screenHeight + 1) * virtualPageWidth;

        if (EmMemGetBankPtr(firstLineAddr) == nullptr) return;

        marker(firstLineAddr, lastLineAddr);
    }

    inline uint32 convertColor_12bit(uint16 encoded) {
        uint8 r = (encoded >> 8) & 0x0f;
        uint8 g = (encoded >> 4) & 0x0f;
        uint8 b = (encoded >> 0) & 0x0f;

        r |= (r << 4);
        g |= (g << 4);
        b |= (b << 4);

        return 0xff000000 | (b << 16) | (g << 8) | r;
    }

    inline uint32 convertColor_16bit(uint16 encoded) {
        uint8 r = (encoded >> 11) & 0x1f;
        uint8 g = (encoded >> 5) & 0x3f;
        uint8 b = (encoded >> 0) & 0x1f;

        r = (r << 3) | (r >> 2);
        g = (g << 2) | (g >> 4);
        b = (b << 3) | (b >> 2);

        return 0xff000000 | (b << 16) | (g << 8) | r;
    }
}  // namespace

// Values used to initialize the DragonBallSZ registers.

static const HwrM68SZ328Type kInitial68SZ328RegisterValues = {
    0x0000,  // UInt16	dmaControl;					// $00000: DMA Control
             // Register
    0x0000,  // UInt16	dmaInterruptStatus;			// $00002: DMA Interrupt Status
             // Register
    0x01FF,  // UInt16	dmaInterruptMask;			// $00004: DMA Interrupt Mask
             // Register
    0x0000,  // UInt16	dmaBurstTimeOutStatus;		// $00006: DMA Burst Time-Out Status
             // Register
    0x0000,  // UInt16	dmaRequestTimeOutStatus;	// $00008: DMA Request Time-Out Status
             // Register

    {0},  // UInt8
          // ___filler00[0x00E-0x00A];

    0x0000,  // UInt16	dmaBurstTimeOutControl;		// $0000E: DMA Burst Time-Out Control
             // Register

    {0},  // UInt8
          // ___filler01[0x040-0x010];

    0x00000000,  // UInt32	mem0SourceAddress;			// $00040: Memory Channel 0
                 // Source Address Register
    0x00000000,  // UInt32	mem0DestAddress;			// $00044: Memory Channel 0
                 // Destination Address Register
    0x00000000,  // UInt32	mem0Count;					// $00048: Memory
                 // Channel 0 Count Register
    0x0000,      // UInt16	mem0Control;				// $0004C: Memory Channel 0 Control
                 // Register
    0x0000,      // UInt16	mem0BurstLength;			// $0004E: Memory Channel 0 Burst
                 // Length Register
    0x0000,      // UInt16	mem0BusUtilizationControl;	// $00050: Memory Channel 0 Bus Utilization
                 // Control Register
    0x0000,      // UInt16	mem0BlockLength;			// $00052: Memory Channel 0 Block
                 // Length Register
    0x0000,      // UInt16	mem0BlockSeparationDist;	// $00054: Memory Channel 0 Block Separation
                 // Distance Register
    0x0000,      // UInt16	mem0DMARequestTimeOut;		// $00056: Memory Channel 0 DMA Request Time
                 // Out Register

    {0},  // UInt8
          // ___filler02[0x080-0x058];

    0x00000000,  // UInt32	mem1SourceAddress;			// $00080: Memory Channel 1
                 // Source Address Register
    0x00000000,  // UInt32	mem1DestAddress;			// $00084: Memory Channel 1
                 // Destination Address Register
    0x00000000,  // UInt32	mem1Count;					// $00088: Memory
                 // Channel 1 Count Register
    0x0000,      // UInt16	mem1Control;				// $0008C: Memory Channel 1 Control
                 // Register
    0x0000,      // UInt16	mem1BurstLength;			// $0008E: Memory Channel 1 Burst
                 // Length Register
    0x0000,      // UInt16	mem1BusUtilizationControl;	// $00090: Memory Channel 1 Bus Utilization
                 // Control Register
    0x0000,      // UInt16	mem1BlockLength;			// $00092: Memory Channel 1 Block
                 // Length Register
    0x0000,      // UInt16	mem1BlockSeparationDist;	// $00094: Memory Channel 1 Block Separation
                 // Distance Register
    0x0000,      // UInt16	mem1DMARequestTimeOut;		// $00096: Memory Channel 1 DMA Request Time
                 // Out Register

    {0},  // UInt8
          // ___filler03[0x0C0-0x098];

    0x00000000,  // UInt32	io2MemoryAddress;			// $000C0: I/O Channel 2
                 // Memory Address Register
    0x00000000,  // UInt32	io2PeripheralAddress;		// $000C4: I/O Channel 2 Peripheral
                 // Address Register
    0x00000000,  // UInt32	io2CountRegister;			// $000C8: I/O Channel 2
                 // Count Register
    0x0000,      // UInt16	io2Control;					// $000CC: I/O Channel 2
                 // Control Register
    0x0000,      // UInt16	io2RequestSourceSelect;		// $000CE: I/O Channel 2 Request Source
                 // Select Register
    0x0000,      // UInt16	io2BurstLength;				// $000D0: I/O Channel 2 Burst
                 // Length Register
    0x0000,      // UInt16	io2DMARequestTimeOut;		// $000D2: I/O Channel 2 DMA Request
                 // Time-Out Register

    {0},  // UInt8
          // ___filler04[0x100-0x0D4];

    0x00000000,  // UInt32	io3MemoryAddress;			// $00100: I/O Channel 3
                 // Memory Address Register
    0x00000000,  // UInt32	io3PeripheralAddress;		// $00104: I/O Channel 3 Peripheral
                 // Address Register
    0x00000000,  // UInt32	io3CountRegister;			// $00108: I/O Channel 3
                 // Count Register
    0x0000,      // UInt16	io3Control;					// $0010C: I/O Channel 3
                 // Control Register
    0x0000,      // UInt16	io3RequestSourceSelect;		// $0010E: I/O Channel 3 Request Source
                 // Select Register
    0x0000,      // UInt16	io3BurstLength;				// $00110: I/O Channel 3 Burst
                 // Length Register
    0x0000,      // UInt16	io3DMARequestTimeOut;		// $00112: I/O Channel 3 DMA Request
                 // Time-Out Register

    {0},  // UInt8
          // ___filler05[0x140-0x114];

    0x00000000,  // UInt32	io4MemoryAddress;			// $00140: I/O Channel 4
                 // Memory Address Register
    0x00000000,  // UInt32	io4PeripheralAddress;		// $00144: I/O Channel 4 Peripheral
                 // Address Register
    0x00000000,  // UInt32	io4CountRegister;			// $00148: I/O Channel 4
                 // Count Register
    0x0000,      // UInt16	io4Control;					// $0014C: I/O Channel 4
                 // Control Register
    0x0000,      // UInt16	io4RequestSourceSelect;		// $0014E: I/O Channel 4 Request Source
                 // Select Register
    0x0000,      // UInt16	io4BurstLength;				// $00150: I/O Channel 4 Burst
                 // Length Register
    0x0000,      // UInt16	io4DMARequestTimeOut;		// $00152: I/O Channel 4 DMA Request
                 // Time-Out Register

    {0},  // UInt8
          // ___filler06[0x180-0x154];

    0x00000000,  // UInt32	io5MemoryAddress;			// $00180: I/O Channel 5
                 // Memory Address Register
    0x00000000,  // UInt32	io5PeripheralAddress;		// $00184: I/O Channel 5 Peripheral
                 // Address Register
    0x00000000,  // UInt32	io5CountRegister;			// $00188: I/O Channel 5
                 // Count Register
    0x0000,      // UInt16	io5Control;					// $0018C: I/O Channel 5
                 // Control Register
    0x0000,      // UInt16	io5RequestSourceSelect;		// $0018E: I/O Channel 5 Request Source
                 // Select Register
    0x0000,      // UInt16	io5BurstLength;				// $00190: I/O Channel 5 Burst
                 // Length Register
    0x0000,      // UInt16	io5DMARequestTimeOut;		// $00192: I/O Channel 5 DMA Request
                 // Time-Out Register

    {0},  // UInt8
          // ___filler07[0x200-0x194];

    0x0000,  // UInt16	adcPenSampleFifo;			// $00200: ADC Pen Sample FIFO

    {0},  // UInt8
          // ___filler08[0x210-0x202];

    0x00000000,  // UInt32	adcControl;					// $00210: ADC
                 // Control Register
    0x0000,      // UInt16	adcPenADSampleRateControl;	// $00214: ADC Pen A/D Sample Rate Control
                 // Register

    {0},  // UInt8
          // ___filler09[0x218-0x216];

    0x0000,  // UInt16	adcInterruptControl;		// $00218: ADC Interrupt Control Register
    0x0000,  // UInt16	adcInterruptErrorStatus;	// $0021A: ADC Interrupt/Error Status
             // Register

    {0},  // UInt8
          // ___filler10[0x22C-0x21C];

    0x0000,  // UInt16	adcClockDivide;				// $0022C: ADC Clock Divide Register

    {0},  // UInt8
          // ___filler11[0x230-0x22E];

    0x00000000,  // UInt32	adcCompareControl;			// $00230: ADC Compare
                 // Control Register

    {0},  // UInt8
          // ___filler12[0x300-0x234];

    0x0000,  // UInt16	mmcClockControl;			// $00300: MMC Clock Control
             // Register

    {0},  // UInt8
          // ___filler13[0x304-0x302];

    0x0000,  // UInt16	mmcStatus;					// $00304: MMC Status
             // Register

    {0},  // UInt8
          // ___filler14[0x308-0x306];

    0x0036,  // UInt16	mmcClockRate;				// $00308: MMC Clock Rate Register

    {0},  // UInt8
          // ___filler15[0x310-0x30A];

    0x0000,  // UInt16	mmcCommandDataControl;		// $00310: MMC Command and Data Control
             // Register

    {0},  // UInt8
          // ___filler16[0x314-0x312];

    0x0040,  // UInt16	mmcResponseTimeOut;			// $00314: MMC Response Time Out
             // Register

    {0},  // UInt8
          // ___filler17[0x318-0x316];

    0xFFFF,  // UInt16	mmcReadTimeOut;				// $00318: MMC Read Time Out
             // Register

    {0},  // UInt8
          // ___filler18[0x31C-0x31A];

    0x0000,  // UInt16	mmcBlockLength;				// $0031C: MMC Block Length Register

    {0},  // UInt8
          // ___filler19[0x320-0x31E];

    0x0000,  // UInt16	mmcNumberOfBlocks;			// $00320: MMC Number of Blocks
             // Register

    {0},  // UInt8
          // ___filler20[0x324-0x322];

    0x0380,  // UInt16	mmcRevisionNumber;			// $00324: MMC Revision Number
             // Register

    0x0000,  // UInt16	mmcIOPort;					// $00326: MMC IO Port
             // Register
    0x000F,  // UInt16	mmcInterruptMask;			// $00328: MMC Interrupt Mask
             // Register

    {0},  // UInt8
          // ___filler21[0x32C-0x32A];

    0x0000,  // UInt16	mmcCommandNumber;			// $0032C: MMC Command Number
             // Register

    {0},  // UInt8
          // ___filler22[0x330-0x32E];

    0x0000,  // UInt16	mmcHigherArgument;			// $00330: MMC HIGHER Argument
             // Register
    0x0000,  // UInt16	mmcLowerArgument;			// $00332: MMC LOWER Argument
             // Register
    0x0000,  // UInt16	mmcResponseFifo;			// $00334: MMC Response FIFO
             // Register;

    {0},  // UInt8
          // ___filler23[0x338-0x336];

    0x0000,  // UInt16	mmcBufferAccess;			// $00338: MMC Buffer Access
             // Register

    {0},  // UInt8
          // ___filler24[0x33C-0x33A];

    0x0000,  // UInt16	mmcBufferPartFull;			// $0033C: MMC Buffer Part Full
             // Register

    {0},  // UInt8
          // ___filler25[0x400-0x33E];

    0x00000000,  // UInt32	usbFrameNumberMatch;		// $00400: USB Frame Number and
                 // Match
    0x00001010,  // UInt32	usbSpecificationRelease;	// $00404: USB Specification/Release
                 // Number
    0x00000000,  // UInt32	usbStatus;					// $00408: USB
                 // Status
    0x00000010,  // UInt32	usbControl;					// $0040C: USB
                 // Control
    0x80000000,  // UInt32	usbConfigurationStatus;		// $00410: USB Configuration Status
    0x00000000,  // UInt32	usbEndpointBufferData;		// $00414: USB Endpoint Buffer Data
    0x00000000,  // UInt32	usbGeneralInterruptStatus;	// $00418: USB General Interrupt
                 // Status Register
    0x800000FF,  // UInt32	usbInterruptMask;			// $0041C: USB Interrupt
                 // Mask

    {0},  // UInt8
          // ___filler26[0x424-0x420];

    0x00000000,  // UInt32	usbEnable;					// $00424: USB
                 // Enable
    0x00000000,  // UInt32	usbInterruptStatus;			// $00428: USB Interrupt
                 // Status Register

    {0},  // UInt8
          // ___filler27[0x430-0x42C];

    0x00000000,  // UInt32	usbEP0StatusControl;		// $00430: USB Endpoint 0
                 // Status/Control
    0x00000080,  // UInt32	usbEP0InterruptStatus;		// $00434: USB Endpoint 0 Interrupt
                 // Status
    0x000001FF,  // UInt32	usbEP0InterruptMask;		// $00438: USB Endpoint 0 Interrupt
                 // Mask
    0x0000,      // UInt16	usbEP0FifoData;				// $0043C: USB Endpoint 0 FIFO Data

    {0},  // UInt8
          // ___filler28[0x440-0x43E];

    0x00010000,  // UInt32	usbEP0FifoStatus;			// $00440: USB Endpoint 0
                 // FIFO Status
    0x01000000,  // UInt32	usbEP0FifoControl;			// $00444: USB Endpoint 0
                 // FIFO Control
    0x00000000,  // UInt32	usbEP0LastReadFramePointer;	// $00448: USB Endpoint 0 FIFO Last
                 // Read Frame Pointer
    0x00000000,  // UInt32	usbEP0LastWriteFramePointer;	// $0044C: USB Endpoint 0 FIFO Last
                 // Write Frame Pointer
    0x00000000,  // UInt32	usbEP0FifoAlarm;			// $00450: USB Endpoint 0
                 // FIFO Alarm
    0x00000000,  // UInt32	usbEP0FifoReadPointer;		// $00454: USB Endpoint 0 FIFO Read
                 // Pointer
    0x00000000,  // UInt32	usbEP0FifoWritePointer;		// $00458: USB Endpoint 0 FIFO Write
                 // Pointer

    {0},  // UInt8
          // ___filler29[0x460-0x45C];

    0x00000000,  // UInt32	usbEP1StatusControl;		// $00460: USB Endpoint 1
                 // Status/Control
    0x00000080,  // UInt32	usbEP1InterruptStatus;		// $00464: USB Endpoint 1 Interrupt
                 // Status
    0x000001FF,  // UInt32	usbEP1InterruptMask;		// $00468: USB Endpoint 1 Interrupt
                 // Mask
    0x0000,      // UInt16	usbEP1FifoData;				// $0046C: USB Endpoint 1 FIFO Data

    {0},  // UInt8
          // ___filler30[0x470-0x46E];

    0x00010000,  // UInt32	usbEP1FifoStatus;			// $00470: USB Endpoint 1
                 // FIFO Status
    0x01000000,  // UInt32	usbEP1FifoControl;			// $00474: USB Endpoint 1
                 // FIFO Control
    0x00000000,  // UInt32	usbEP1LastReadFramePointer;	// $00478: USB Endpoint 1 FIFO Last
                 // Read Frame Pointer
    0x00000000,  // UInt32	usbEP1LastWriteFramePointer;	// $0047C: USB Endpoint 1 FIFO Last
                 // Write Frame Pointer
    0x00000000,  // UInt32	usbEP1FifoAlarm;			// $00480: USB Endpoint 1
                 // FIFO Alarm
    0x00000000,  // UInt32	usbEP1FifoReadPointer;		// $00484: USB Endpoint 1 FIFO Read
                 // Pointer
    0x00000000,  // UInt32	usbEP1FifoWritePointer;		// $00488: USB Endpoint 1 FIFO Write
                 // Pointer

    {0},  // UInt8
          // ___filler31[0x490-0x48C];

    0x00000000,  // UInt32	usbEP2StatusControl;		// $00490: USB Endpoint 2
                 // Status/Control
    0x00000080,  // UInt32	usbEP2InterruptStatus;		// $00494: USB Endpoint 2 Interrupt
                 // Status
    0x000001FF,  // UInt32	usbEP2InterruptMask;		// $00498: USB Endpoint 2 Interrupt
                 // Mask
    0x0000,      // UInt16	usbEP2FifoData;				// $0049C: USB Endpoint 2 FIFO Data

    {0},  // UInt8
          // ___filler32[0x4A0-0x49E];

    0x00010000,  // UInt32	usbEP2FifoStatus;			// $004A0: USB Endpoint 2
                 // FIFO Status
    0x01000000,  // UInt32	usbEP2FifoControl;			// $004A4: USB Endpoint 2
                 // FIFO Control
    0x00000000,  // UInt32	usbEP2LastReadFramePointer;	// $004A8: USB Endpoint 2 FIFO Last
                 // Read Frame Pointer
    0x00000000,  // UInt32	usbEP2LastWriteFramePointer;	// $004AC: USB Endpoint 2 FIFO Last
                 // Write Frame Pointer
    0x00000000,  // UInt32	usbEP2FifoAlarm;			// $004B0: USB Endpoint 2
                 // FIFO Alarm
    0x00000000,  // UInt32	usbEP2FifoReadPointer;		// $004B4: USB Endpoint 2 FIFO Read
                 // Pointer
    0x00000000,  // UInt32	usbEP2FifoWritePointer;		// $004B8: USB Endpoint 2 FIFO Write
                 // Pointer

    {0},  // UInt8
          // ___filler33[0x4C0-0x4BC];

    0x00000000,  // UInt32	usbEP3StatusControl;		// $004C0: USB Endpoint 3
                 // Status/Control
    0x00000080,  // UInt32	usbEP3InterruptStatus;		// $004C4: USB Endpoint 3 Interrupt
                 // Status
    0x000001FF,  // UInt32	usbEP3InterruptMask;		// $004C8: USB Endpoint 3 Interrupt
                 // Mask
    0x0000,      // UInt16	usbEP3FifoData;				// $004CC: USB Endpoint 3 FIFO Data

    {0},  // UInt8
          // ___filler34[0x4D0-0x4CE];

    0x00010000,  // UInt32	usbEP3FifoStatus;			// $004D0: USB Endpoint 3
                 // FIFO Status
    0x01000000,  // UInt32	usbEP3FifoControl;			// $004D4: USB Endpoint 3
                 // FIFO Control
    0x00000000,  // UInt32	usbEP3LastReadFramePointer;	// $004D8: USB Endpoint 3 FIFO Last
                 // Read Frame Pointer
    0x00000000,  // UInt32	usbEP3LastWriteFramePointer;	// $004DC: USB Endpoint 3 FIFO Last
                 // Write Frame Pointer
    0x00000000,  // UInt32	usbEP3FifoAlarm;			// $004E0: USB Endpoint 3
                 // FIFO Alarm
    0x00000000,  // UInt32	usbEP3FifoReadPointer;		// $004E4: USB Endpoint 3 FIFO Read
                 // Pointer
    0x00000000,  // UInt32	usbEP3FifoWritePointer;		// $004E8: USB Endpoint 3 FIFO Write
                 // Pointer

    {0},  // UInt8
          // ___filler35[0x4F0-0x4EC];

    0x00000000,  // UInt32	usbEP4StatusControl;		// $004F0: USB Endpoint 4
                 // Status/Control
    0x00000080,  // UInt32	usbEP4InterruptStatus;		// $004F4: USB Endpoint 4 Interrupt
                 // Status
    0x000001FF,  // UInt32	usbEP4InterruptMask;		// $004F8: USB Endpoint 4 Interrupt
                 // Mask
    0x0000,      // UInt16	usbEP4FifoData;				// $004FC: USB Endpoint 4 FIFO Data

    {0},  // UInt8
          // ___filler36[0x500-0x4FE];

    0x00010000,  // UInt32	usbEP4FifoStatus;			// $00500: USB Endpoint 4
                 // FIFO Status
    0x01000000,  // UInt32	usbEP4FifoControl;			// $00504: USB Endpoint 4
                 // FIFO Control
    0x00000000,  // UInt32	usbEP4LastReadFramePointer;	// $00508: USB Endpoint 4 FIFO Last
                 // Read Frame Pointer
    0x00000000,  // UInt32	usbEP4LastWriteFramePointer;	// $0050C: USB Endpoint 4 FIFO Last
                 // Write Frame Pointer
    0x00000000,  // UInt32	usbEP4FifoAlarm;			// $00510: USB Endpoint 4
                 // FIFO Alarm
    0x00000000,  // UInt32	usbEP4FifoReadPointer;		// $00514: USB Endpoint 4 FIFO Read
                 // Pointer
    0x00000000,  // UInt32	usbEP4FifoWritePointer;		// $00518: USB Endpoint 4 FIFO Write
                 // Pointer

    {0},  // UInt8
          // ___filler37[0x800-0x51C];

    0x00000000,  // UInt32	lcdStartAddr;				// $00800: LCD Screen
                 // Starting Address Register
    0x0000,      // UInt16	lcdScreenSize;				// $00804: LCD Screen Size Register
    0x0000,      // UInt16	lcdPageWidth;				// $00806: LCD Virtual Page Width
                 // Register
    0x0000,      // UInt16	lcdCursorXPos;				// $00808: LCD Cursor X Position
    0x0000,      // UInt16	lcdCursorYPos;				// $0080A: LCD Cursor Y Position
    0x0101,      // UInt16	lcdCursorSize;				// $0080C: LCD Cursor Size
    0x00FF,      // UInt16	lcdBlinkControl;			// $0080E: LCD Blink Control
                 // Register
    0x0000,      // UInt16	lcdColorCursorMapping;		// $00810: LCD Color Cursor Mapping Register
    0x0000,      // UInt16	lcdPanelControl0;			// $00812: LCD Panel Configuration
                 // Register 0
    0x0000,      // UInt16	lcdPanelControl1;			// $00814: LCD Panel Configuration
                 // Register 1
    0x0000,      // UInt16	lcdHorizontalConfig0;		// $00816: LCD Horizontal Configuration
                 // Register 0
    0x0400,      // UInt16	lcdHorizontalConfig1;		// $00818: LCD Horizontal Configuration
                 // Register 1
    0x0000,      // UInt16	lcdVerticalConfig0;			// $0081A: LCD Vertical
                 // Configuration Register 0
    0x0401,      // UInt16	lcdVerticalConfig1;			// $0081C: LCD Vertical
                 // Configuration Register 1
    0x0000,      // UInt16	lcdPanningOffset;			// $0081E: LCD Panning Offset
                 // Register
    0x0073,      // UInt16	lcdGrayPalette;				// $00820: LCD Gray Palette Mapping
                 // Register
    0x0000,      // UInt16	lcdPWMContrastControl;		// $00822: LCD PWM Contrast Control Register
    0x0404,      // UInt16	lcdDMAControl;				// $00824: LCD DMA Control Register
    0x0000,      // UInt16	lcdRefreshModeControl;		// $00826: LCD Refresh Mode Control Register
    0x0000,      // UInt16	lcdInterruptConfiguration;	// $00828: LCD Interrupt Configuration
    0x0000,      // UInt16	lcdInterruptStatus;			// $0082A: LCD Interrupt Status
    {0},
    {0},  // UInt32[256] lcdCLUT

    {0},  // UInt8
          // ___filler38[0x1F000-0x00C2C];

    0x1C,  // UInt8	scr;						// $10000: System Control
           // Register

    {0},  // UInt8
          // ___filler39[0x003-0x001];

    0x00,    // UInt8	pcr;						// $10003: Peripheral
             // Control Register
    0x56,    // UInt8	chipID;						// $10004: Chip ID Register
    0x01,    // UInt8	maskID;						// $10005: Mask ID Register
    0x0000,  // UInt16	swID;						// $10006: Software ID
             // Register
    0x1FFF,  // UInt16	ioDriveControl;				// $10008: I/O Drive Control
             // Register

    {0},  // UInt8
          // ___filler40[0x100-0x00A];

    0x0000,  // UInt16	csAGroupBase;				// $10100: Chip Select Group A Base
             // Register
    0x0000,  // UInt16	csBGroupBase;				// $10102: Chip Select Group B Base
             // Register
    0x0000,  // UInt16	csCGroupBase;				// $10104: Chip Select Group C Base
             // Register
    0x0000,  // UInt16	csDGroupBase;				// $10106: Chip Select Group D Base
             // Register

    {0},  // UInt8
          // ___filler41[0x10c-0x108];

    0x0000,  // UInt16	csControl2;					// $1010C: Chip Select
             // Control Register 2

    {0},  // UInt8
          // ___filler42[0x110-0x10e];

    0x00B0,  // UInt16	csASelect;					// $10110: Group A Chip
             // Select Register
    0x0000,  // UInt16	csBSelect;					// $10112: Group B Chip
             // Select Register
    0x0000,  // UInt16	csCSelect;					// $10114: Group C Chip
             // Select Register
    0x0200,  // UInt16	csDSelect;					// $10116: Group D Chip
             // Select Register

    0x0060,  // UInt16	emuCS;						// $10118: EMU Chip Select
             // Register

    {0},  // UInt8
          // ___filler43[0x150-0x11A];

    0x9C00,  // UInt16	csControl3;					// $10150: Chip Select
             // Control Register 3

    {0},  // UInt8
          // ___filler43a[0x180-0x152];

    0x0000,  // UInt16	csEGroupBase;				// $10180: Chip Select Group E Base
             // Register
    0x0000,  // UInt16	csFGroupBase;				// $10182: Chip Select Group F Base
             // Register
    0x0000,  // UInt16	csGGroupBase;				// $10184: Chip Select Group G Base
             // Register

    {0},  // UInt8
          // ___filler44[0x18A-0x186];

    0x0000,  // UInt16	csControl1;					// $1018A: Chip Select
             // Control Register 1

    {0},  // UInt8
          // ___filler45[0x190-0x18C];

    0x0000,  // UInt16	csESelect;					// $10190: Group E Chip
             // Select Register
    0x0000,  // UInt16	csFSelect;					// $10192: Group F Chip
             // Select Register
    0x0000,  // UInt16	csGSelect;					// $10194: Group G Chip
             // Select Register

    {0},  // UInt8
          // ___filler46[0x200-0x196];

    0x2414,  // UInt16	pllControl;					// $10200: PLL Control
             // Register
    0x3CE8,  // UInt16	pllFreqSel0;				// $10202: MCU PLL Frequency Select
             // Register 0
    0x0900,  // UInt16	pllFreqSel1;				// $10204: MCU PLL Frequency Select
             // Register 1

    0,  // UInt8									___filler47;

    0x1F,  // UInt8	pwrControl;					// $10207: CPU Power Control
           // Register

    0x2C01,  // UInt16	usbpllFreqSel0;				// $10208: USBPLL Frequency Select
             // Register 0
    0x0001,  // UInt16	usbpllFreqSel1;				// $1020A: USBPLL Frequency Select
             // Register 1

    0x8A03,  // UInt16	clockSrcCtl;				// $1020C: Clock Source Control
             // Register

    {0},  // UInt8
          // ___filler48[0x300-0x20E];

    0x00,  // UInt8	intVector;					// $10300: Interrupt Vector
           // Register

    0x00,  // UInt8									___filler49;

    0x0000,  // UInt16	intControl;					// $10302: Interrupt Control
             // Register
    0x00FF,  // UInt16	intMaskHi;					// $10304: Interrupt Mask
             // Register (high word)
    0xFFFF,  // UInt16	intMaskLo;					// $10306: Interrupt Mask
             // Register (low word)

    {0},  // UInt8
          // ___filler50[0x30C-0x308];

    0x0000,  // UInt16	intStatusHi;				// $1030C: Interrupt Status Register
             // (high word)
    0x0000,  // UInt16	intStatusLo;				// $1030E: Interrupt Status Register
             // (low word)
    0x0000,  // UInt16	intPendingHi;				// $10310: Interrupt Pending
             // Register (high word)
    0x0000,  // UInt16	intPendingLo;				// $10312: Interrupt Pending
             // Register (low word)
    0x6533,  // UInt16	intLevelControl;			// $10314: Interrupt Level Control
             // Register
    0x6533,  // UInt16	intLevelControl2;			// $10316: Interrupt Level Control
             // Register 2
    0x4666,  // UInt16	intLevelControl3;			// $10318: Interrupt Level Control
             // Register 3
    0x4444,  // UInt16	intLevelControl4;			// $1031A: Interrupt Level Control
             // Register 4
    0x4444,  // UInt16	intLevelControl5;			// $1031C: Interrupt Level Control
             // Register 5
    0x4444,  // UInt16	intLevelControl6;			// $1031E: Interrupt Level Control
             // Register 6
    0x0546,  // UInt16	intLevelControl7;			// $10320: Interrupt Level Control
             // Register 7

    {0},  // UInt8
          // ___filler51[0x400-0x322];

    0x00,  // UInt8	portADir;					// $10400: Port A Direction
           // Register
    0xFF,  // UInt8	portAData;					// $10401: Port A Data
           // Register
    0xFF,  // UInt8	portAPullupEn;				// $10402: Port A Pullup Enable

    {0},  // UInt8
          // ___filler52[0x408-0x403];

    0x00,  // UInt8	portBDir;					// $10408: Port B Direction
           // Register
    0xFF,  // UInt8	portBData;					// $10409: Port B Data
           // Register
    0xFF,  // UInt8	portBPullupEn;				// $1040A: Port B Pullup Enable
    0xFF,  // UInt8	portBSelect;				// $1040B: Port B Select Register

    {0},  // UInt8
          // ___filler53[0x410-0x40C];

    0x00,  // UInt8	portCDir;					// $10410: Port C Direction
           // Register
    0x00,  // UInt8	portCData;					// $10411: Port C Data
           // Register
    0xFF,  // UInt8	portCPulldnEn;				// $10412: Port C Pulldown Enable
    0xFF,  // UInt8	portCSelect;				// $10413: Port C Select Register

    {0},  // UInt8
          // ___filler54[0x418-0x414];

    0x00,  // UInt8	portDDir;					// $10418: Port D Direction
           // Register
    0xFF,  // UInt8	portDData;					// $10419: Port D Data
           // Register
    0xFF,  // UInt8	portDPullupEn;				// $1041A: Port D Pull-up Enable
    0xFF,  // UInt8	portDSelect;				// $1041B: Port D Select Register
    0x00,  // UInt8	portDIntMask;				// $1041C: Port D Interrupt Mask
           // Register
    0x00,  // UInt8	portDIntStatus;				// $1041D: Port D Interrupt Status
           // Register
    0x00,  // UInt8	portDIntEdge;				// $1041E: Port D Interrupt Edge
           // Register
    0x00,  // UInt8	portDIntPolarity;			// $1041F: Port D Interrupt Polarity
           // Register

    0x00,  // UInt8	portEDir;					// $10420: Port E Direction
           // Register
    0xFF,  // UInt8	portEData;					// $10421: Port E Data
           // Register
    0xFF,  // UInt8	portEPullupEn;				// $10422: Port E Pull-up Enable
    0xFF,  // UInt8	portESelect;				// $10423: Port E Select Register
    0x00,  // UInt8	portEIntMask;				// $10424: Port E Interrupt Mask
           // Register
    0x00,  // UInt8	portEIntStatus;				// $10425: Port E Interrupt Status
           // Register
    0x00,  // UInt8	portEIntEdge;				// $10426: Port E Interrupt Edge
           // Register
    0x00,  // UInt8	portEIntPolarity;			// $10427: Port E Interrupt Polarity
           // Register

    0x00,  // UInt8	portFDir;					// $10428: Port F Direction
           // Register
    0xFF,  // UInt8	portFData;					// $10429: Port F Data
           // Register
    0xFF,  // UInt8	portFPullupdnEn;			// $1042A: Port F Pull-up/down
           // Enable
    0x87,  // UInt8	portFSelect;				// $1042B: Port F Select Register
    0x00,  // UInt8	portFIntMask;				// $1042C: Port F Interrupt Mask
           // Register
    0x00,  // UInt8	portFIntStatus;				// $1042D: Port F Interrupt Status
           // Register
    0x00,  // UInt8	portFIntEdge;				// $1042E: Port F Interrupt Edge
           // Register
    0x00,  // UInt8	portFIntPolarity;			// $1042F: Port F Interrupt Polarity
           // Register

    0x00,  // UInt8	portGDir;					// $10430: Port G Direction
           // Register
    0x3F,  // UInt8	portGData;					// $10431: Port G Data
           // Register
    0x3D,  // UInt8	portGPullupEn;				// $10432: Port G Pull-up Enable
    0x08,  // UInt8	portGSelect;				// $10433: Port G Select Register
    0x00,  // UInt8	portGIntMask;				// $10434: Port G Interrupt Mask
           // Register
    0x00,  // UInt8	portGIntStatus;				// $10435: Port G Interrupt Status
           // Register
    0x00,  // UInt8	portGIntEdge;				// $10436: Port G Interrupt Edge
           // Register
    0x00,  // UInt8	portGIntPolarity;			// $10437: Port G Interrupt Polarity
           // Register

    0x00,  // UInt8	portJDir;					// $10438: Port J Direction
           // Register
    0xFF,  // UInt8	portJData;					// $10439: Port J Data
           // Register
    0xFF,  // UInt8	portJPullupEn;				// $1043A: Port J Pull-up Enable
    0xEF,  // UInt8	portJSelect;				// $1043B: Port J Select Register
    0x00,  // UInt8	portJIntMask;				// $1043C: Port J Interrupt Mask
           // Register
    0x00,  // UInt8	portJIntStatus;				// $1043D: Port J Interrupt Status
           // Register
    0x00,  // UInt8	portJIntEdge;				// $1043E: Port J Interrupt Edge
           // Register
    0x00,  // UInt8	portJIntPolarity;			// $1043F: Port J Interrupt Polarity
           // Register

    0x00,  // UInt8	portKDir;					// $10440: Port K Direction
           // Register
    0x0F,  // UInt8	portKData;					// $10441: Port K Data
           // Register
    0xFF,  // UInt8	portKPullupdnEn;			// $10442: Port K Pull-up/down
           // Enable
    0x00,  // UInt8	portKSelect;				// $10443: Port K Select Register
    0x00,  // UInt8	portKIntMask;				// $10444: Port K Interrupt Mask
           // Register
    0x00,  // UInt8	portKIntStatus;				// $10445: Port K Interrupt Status
           // Register
    0x00,  // UInt8	portKIntEdge;				// $10446: Port K Interrupt Edge
           // Register
    0x00,  // UInt8	portKIntPolarity;			// $10447: Port K Interrupt Polarity
           // Register

    0x00,  // UInt8	portMDir;					// $10448: Port M Direction
           // Register
    0x20,  // UInt8	portMData;					// $10449: Port M Data
           // Register
    0x3F,  // UInt8	portMPullupdnEn;			// $1044A: Port M Pull-up/down
           // Enable
    0x3F,  // UInt8	portMSelect;				// $1044B: Port M Select Register
    0x00,  // UInt8	portMIntMask;				// $1044C: Port M Interrupt Mask
           // Register
    0x00,  // UInt8	portMIntStatus;				// $1044D: Port M Interrupt Status
           // Register
    0x00,  // UInt8	portMIntEdge;				// $1044E: Port M Interrupt Edge
           // Register
    0x00,  // UInt8	portMIntPolarity;			// $1044F: Port M Interrupt Polarity
           // Register

    0x00,  // UInt8	portNDir;					// $10450: Port N Direction
           // Register
    0x0F,  // UInt8	portNData;					// $10451: Port N Data
           // Register
    0xFF,  // UInt8	portNPullupdnEn;			// $10452: Port N Pull-up/down
           // Enable
    0xFF,  // UInt8	portNSelect;				// $10453: Port N Select Register
    0x00,  // UInt8	portNIntMask;				// $10454: Port N Interrupt Mask
           // Register
    0x00,  // UInt8	portNIntStatus;				// $10455: Port N Interrupt Status
           // Register
    0x00,  // UInt8	portNIntEdge;				// $10456: Port N Interrupt Edge
           // Register
    0x00,  // UInt8	portNIntPolarity;			// $10457: Port N Interrupt Polarity
           // Register

    0x00,  // UInt8	portPDir;					// $10458: Port P Direction
           // Register
    0x0F,  // UInt8	portPData;					// $10459: Port P Data
           // Register
    0xFF,  // UInt8	portPPullupdnEn;			// $1045A: Port P Pull-up/down
           // Enable
    0xFF,  // UInt8	portPSelect;				// $1045B: Port P Select Register
    0x00,  // UInt8	portPIntMask;				// $1045C: Port P Interrupt Mask
           // Register
    0x00,  // UInt8	portPIntStatus;				// $1045D: Port P Interrupt Status
           // Register
    0x00,  // UInt8	portPIntEdge;				// $1045E: Port P Interrupt Edge
           // Register
    0x00,  // UInt8	portPIntPolarity;			// $1045F: Port P Interrupt Polarity
           // Register

    0x00,  // UInt8	portRDir;					// $10460: Port R Direction
           // Register
    0x0F,  // UInt8	portRData;					// $10461: Port R Data
           // Register
    0xFF,  // UInt8	portRPullupdnEn;			// $10462: Port R Pull-up/down
           // Enable
    0xFF,  // UInt8	portRSelect;				// $10463: Port R Select Register
    0x00,  // UInt8	portRIntMask;				// $10464: Port R Interrupt Mask
           // Register
    0x00,  // UInt8	portRIntStatus;				// $10465: Port R Interrupt Status
           // Register
    0x00,  // UInt8	portRIntEdge;				// $10466: Port R Interrupt Edge
           // Register
    0x00,  // UInt8	portRIntPolarity;			// $10467: Port R Interrupt Polarity
           // Register

    {0},  // UInt8
          // ___filler55[0x500-0x468];

    0x0020,  // UInt16	pwmControl;					// $10500: PWM 1 Control
             // Register  (PWM 1 is the same as the single PWM in EZ)
    0x00,    // UInt8	pwmSampleHi;    			// $10502: PWM 1 Sample - high byte
    0x00,    // UInt8	pwmSampleLo;				// $10503: PWM 1 Sample - low byte
    0xFE,    // UInt8	pwmPeriod;					// $10504: PWM 1 Period
    0x00,    // UInt8	pwmCounter;					// $10505: PWM 1 Counter

    {0},  // UInt8
          // ___filler56[0x510-0x506];

    0x0000,  // UInt16	pwm2Control;				// $10510: PWM 2 Control Register
    0x0000,  // UInt16	pwm2Period;					// $10512: PWM 2 Period
    0x0000,  // UInt16	pwm2Width;					// $10514: PWM 2 Width
    0x0000,  // UInt16	pwm2Counter;				// $10516: PWM 2 Counter

    {0},  // UInt8
          // ___filler57[0x600-0x518];

    0x0000,  // UInt16	tmr1Control;				// $10600: Timer 1 Control Register
    0x0000,  // UInt16	tmr1Prescaler;				// $10602: Timer 1 Prescaler
             // Register
    0xFFFF,  // UInt16	tmr1Compare;				// $10604: Timer 1 Compare Register
    0x0000,  // UInt16	tmr1Capture;				// $10606: Timer 1 Capture Register
    0x0000,  // UInt16	tmr1Counter;				// $10608: Timer 1 Counter Register
    0x0000,  // UInt16	tmr1Status;					// $1060A: Timer 1 Status
             // Register

    {0},  // UInt8
          // ___filler58[0x610-0x60C];

    0x0000,  // UInt16	tmr2Control;				// $10610: Timer 2 Control Register
    0x0000,  // UInt16	tmr2Prescaler;				// $10612: Timer 2 Prescaler
             // Register
    0xFFFF,  // UInt16	tmr2Compare;				// $10614: Timer 2 Compare Register
    0x0000,  // UInt16	tmr2Capture;				// $10616: Timer 2 Capture Register
    0x0000,  // UInt16	tmr2Counter;				// $10618: Timer 2 Counter Register
    0x0000,  // UInt16	tmr2Status;					// $1061A: Timer 2 Status
             // Register

    {0},  // UInt8
          // ___filler59[0x700-0x61C];

    0x0000,  // UInt16	spiRxD;						// $10700: SPI Unit 1
             // Receive Data Register
    0x0000,  // UInt16	spiTxD;						// $10702: SPI Unit 1
             // Transmit Data Register
    0x0000,  // UInt16	spiCont1;					// $10704: SPI Unit 1
             // Control/Status Register
    0x0000,  // UInt16	spiIntCS;					// $10706: SPI Unit 1
             // Interrupt control/Status Register
    0x0000,  // UInt16	spiTest;					// $10708: SPI Unit 1 Test
             // Register
    0x0000,  // UInt16	spiSpc;						// $1070A: SPI Unit 1 Sample
             // period counter register

    {0},  // UInt8
          // ___filler60[0x800-0x70C];

    0x00,  // UInt8	i2cAddress;					// $10800: I2C Address
           // Register

    {0},  // UInt8
          // ___filler61[0x804-0x801];

    0x00,  // UInt8	i2cFreqDiv;					// $10804: I2C Frequency
           // Divider Register

    {0},  // UInt8
          // ___filler62[0x808-0x805];

    0x00,  // UInt8	i2cControl;					// $10808: I2C Control
           // Register

    {0},  // UInt8
          // ___filler63[0x80C-0x809];

    0x81,  // UInt8	i2cStatus;					// $1080C: I2C Status
           // Register

    {0},  // UInt8
          // ___filler64[0x810-0x80D];

    0x00,  // UInt8	i2cDataIO;					// $10810: I2C Data I/O
           // Register

    {0},  // UInt8
          // ___filler65[0x900-0x811];

    0x0000,  // UInt16	uControl;					// $10900: Uart 1
             // Status/Control Register (Uart 1 is the same as the single Uart in EZ)
    0x0002,  // UInt16	uBaud;						// $10902: Uart 1 Baud
             // Control Register
    0x0000,  // UInt16	uReceive;					// $10904: Uart 1 Receive
             // Register
    0xE800,  // UInt16	uTransmit;					// $10906: Uart 1 Transmit
             // Register
    0x0000,  // UInt16	uMisc;						// $10908: Uart 1
             // Miscellaneous Register
    0x0000,  // UInt16	uNonIntPresc;				// $1090A: Uart 1 Non-Integer
             // Prescaler
    0x0102,  // UInt16	uFIFOHMark;					// $1090C: Uart 1 Half Mark
             // FIFO Register

    {0},  // UInt8
          // ___filler66[0x910-0x90E];

    0x0000,  // UInt16	u2Control;					// $10910: Uart 2
             // Status/Control Register
    0x0002,  // UInt16	u2Baud;						// $10912: Uart 2 Baud
             // Control Register
    0x0000,  // UInt16	u2Receive;					// $10914: Uart 2 Receiver
             // Register
    0xE800,  // UInt16	u2Transmit;					// $10916: Uart 2
             // Transmitter Register
    0x0000,  // UInt16	u2Misc;						// $10918: Uart 2
             // Miscellaneous Register
    0x0000,  // UInt16	u2NonIntPresc;				// $1091A: Uart 2 Non-Integer
             // Prescaler Register
    0x0102,  // UInt16	u2FIFOHMark;				// $1091C: Uart 2 Half Mark FIFO
             // Register

    {0},  // UInt8
          // ___filler67[0xB00-0x91E];

    0x00000000,  // UInt32	rtcHourMinSec;				// $10B00: RTC Hours,
                 // Minutes, Seconds Register
    0x00000000,  // UInt32	rtcAlarm;					// $10B04: RTC Alarm
                 // Register

    {0},  // UInt8
          // ___filler68[0xB0A-0xB08];

    0x0001,  // UInt16   rtcWatchDog;				// $10B0A: RTC Watchdog Timer
    0x0080,  // UInt16	rtcControl;					// $10B0C: RTC Control
             // Register
    0x0000,  // UInt16	rtcIntStatus;				// $10B0E: RTC Interrupt Status
             // Register
    0x0000,  // UInt16	rtcIntEnable;				// $10B10: RTC Interrupt Enable
             // Register
    0x003F,  // UInt16	stopWatch;					// $10B12: Stopwatch Minutes

    {0},  // UInt8
          // ___filler69[0xB1A-0xB14];

    0x0000,  // UInt16	rtcDay;						// $10B1A: RTC Day
    0x0000,  // UInt16   rtcDayAlarm;				// $10B1C: RTC Day Alarm

    {0},  // UInt8
          // ___filler70[0xC00-0xB1E];

    0x01000300,  // UInt32	sdramControlE;				// $10C00: SDRAM Control
                 // Register for Chip Select E
    0x01000000,  // UInt32   sdramControlF;				// $10C04: SDRAM Control
                 // Register for Chip Select F
    0x00000000,  // UInt32	edoControlE;				// $10C08: EDO Control
                 // Register for Chip Select E
    0x00000000,  // UInt32	edoControlF;				// $10C0C: EDO Control
                 // Register for Chip Select F
    0x0000,      // UInt16	secondaryControl;			// $10C10: Secondary Control
                 // Register

    {0},  // UInt8
          // ___filler71[0xD00-0xC12];

    0x00000000,  // UInt32	emuAddrCompare;				// $10D00: Emulation Address
                 // Compare Register
    0x00000000,  // UInt32	emuAddrMask;				// $10D04: Emulation Address
                 // Mask Register
    0x0000,      // UInt16	emuControlCompare;			// $10D08: Emulation Control Compare
                 // Register
    0x0000,      // UInt16	emuControlMask;				// $10D0A: Emulation Control Mask
                 // Register
    0x0000,      // UInt16	emuControl;					// $10DOC: Emulation Control
                 // Register
    0x0000       // UInt16	emuStatus;					// $10D0E: Emulation Status
                 // Register
};

// ---------------------------------------------------------------------------
//		� EmRegsSZ::EmRegsSZ
// ---------------------------------------------------------------------------

EmRegsSZ::EmRegsSZ(void)
    : EmRegs(),
      f68SZ328Regs(),
      fHotSyncButtonDown(0),
      fKeyBits(0),
      fLastTmr1Status(0),
      fLastTmr2Status(0),
      fPortDDataCount(0),
      esram(new EmRegsESRAM()) {
    fUART[0] = NULL;
    fUART[1] = NULL;

    for (int counter = 'R'; counter >= 'D'; counter--) {
        fPortXEdge[counter - 'D'] = 0;
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::~EmRegsSZ
// ---------------------------------------------------------------------------

EmRegsSZ::~EmRegsSZ(void) {}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::Initialize
// ---------------------------------------------------------------------------

void EmRegsSZ::Initialize(void) {
    EmRegs::Initialize();

    lastRtcAlarmCheck = -1;
    pwmActive = false;
    afterLoad = false;
    clutDirty = true;

    padcFifoReadIndex = 0;

    EmHAL::AddCycleConsumer(cycleThunk, this);

    systemCycles = gSession->GetSystemCycles();
    tmr1LastProcessedSystemCycles = systemCycles;
    tmr2LastProcessedSystemCycles = systemCycles;

    fUART[0] = new EmUARTDragonball(EmUARTDragonball::kUART_DragonballVZ, 0);
    fUART[1] = new EmUARTDragonball(EmUARTDragonball::kUART_DragonballVZ, 1);

    onDayRolloverHandle = EmHAL::onDayRollover.AddHandler([this]() { HandleDayRollover(); });

    UpdateTimers();
    powerOffCached = GetAsleep();

    onMarkScreenCleanHandle = gSystemState.onMarkScreenClean.AddHandler([this]() { MarkScreen(); });
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::Reset
// ---------------------------------------------------------------------------

void EmRegsSZ::Reset(Bool hardwareReset) {
    EmRegs::Reset(hardwareReset);
    UnmarkScreen();

    if (hardwareReset) {
        tmr1LastProcessedSystemCycles = systemCycles;
        tmr2LastProcessedSystemCycles = systemCycles;

        f68SZ328Regs = kInitial68SZ328RegisterValues;

        // Byteswap all the words in the DragonballSZ registers (if necessary).

        Canonical(f68SZ328Regs);
        ByteswapWords(&f68SZ328Regs, sizeof(f68SZ328Regs));

        fKeyBits = 0;
        fLastTmr1Status = 0;
        fLastTmr2Status = 0;
        for (int counter = 'R'; counter >= 'D'; counter--) {
            fPortXEdge[counter - 'D'] = 0;
        }
        fPortDDataCount = 0;

        // React to the new data in the UART registers.

        Bool sendTxData = false;
        EmRegsSZ::UARTStateChanged(sendTxData, 0);
        EmRegsSZ::UARTStateChanged(sendTxData, 1);

        pwmActive = false;
        powerOffCached = GetAsleep();
        padcFifoReadIndex = 0;
        clutDirty = true;
    }

    UpdateTimers();
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::Dispose
// ---------------------------------------------------------------------------

void EmRegsSZ::Dispose(void) {
    delete fUART[0];
    fUART[0] = NULL;
    delete fUART[1];
    fUART[1] = NULL;

    EmRegs::Dispose();

    gSystemState.onMarkScreenClean.RemoveHandler(onMarkScreenCleanHandle);
    EmHAL::onDayRollover.RemoveHandler(onDayRolloverHandle);
    EmHAL::RemoveCycleConsumer(cycleThunk, this);
}

EmRegsESRAM* EmRegsSZ::GetESRAM() { return esram; }

void EmRegsSZ::Save(Savestate& savestate) { DoSave(savestate); }

void EmRegsSZ::Save(SavestateProbe& savestate) { DoSave(savestate); }

void EmRegsSZ::Load(SavestateLoader& loader) {
    if (fSPISlaveADC) fSPISlaveADC->Load(loader);

    Chunk* chunk = loader.GetChunk(ChunkType::regsSZ);
    if (!chunk) {
        logging::printf("unable to restore RegsSZ: missing savestate\n");
        loader.NotifyError();

        return;
    }

    const uint32 version = chunk->Get32();
    if (version > SAVESTATE_VERSION) {
        logging::printf("unable to restore RegsSZ: unsupported savestate version\n");
        loader.NotifyError();

        return;
    }

    pwmActive = false;

    LoadChunkHelper helper(*chunk);
    DoSaveLoad(helper, version);

    Bool sendTxData = false;
    EmRegsSZ::UARTStateChanged(sendTxData, 0);
    EmRegsSZ::UARTStateChanged(sendTxData, 1);

    gMemAccessFlags.fProtect_SRAMSet = (READ_REGISTER(csESelect) & ROPMask) != 0;

    markScreen = true;
    afterLoad = true;
    clutDirty = true;

    systemCycles = gSession->GetSystemCycles();
    UpdateTimers();
    powerOffCached = GetAsleep();

    UpdateEsramLocation();
    UpdateFramebufferLocation();

    if (version < 2) padcFifoReadIndex = 0;
}

template <typename T>
void EmRegsSZ::DoSave(T& savestate) {
    if (fSPISlaveADC) fSPISlaveADC->Save(savestate);

    typename T::chunkT* chunk = savestate.GetChunk(ChunkType::regsSZ);
    if (!chunk) return;

    chunk->Put32(SAVESTATE_VERSION);

    SaveChunkHelper helper(*chunk);
    DoSaveLoad(helper, SAVESTATE_VERSION);
}

template <typename T>
void EmRegsSZ::DoSaveLoad(T& helper, uint32 version) {
    ::DoSaveLoad(helper, f68SZ328Regs, version >= 2);

    helper.DoBool(fHotSyncButtonDown)
        .Do16(fKeyBits)
        .Do(typename T::Pack16() << fLastTmr1Status << fLastTmr2Status)
        .Do(typename T::Pack8() << fPortXEdge[0] << fPortXEdge[1] << fPortXEdge[2] << fPortXEdge[3])
        .Do(typename T::Pack8() << fPortXEdge[4] << fPortXEdge[5] << fPortXEdge[6] << fPortXEdge[7])
        .Do(typename T::Pack8() << fPortXEdge[8] << fPortXEdge[9] << fPortXEdge[10]
                                << fPortXEdge[11])
        .Do(typename T::Pack8() << fPortXEdge[12] << fPortXEdge[13] << fPortXEdge[14])
        .Do32(fPortDDataCount)
        .DoDouble(tmr1LastProcessedSystemCycles)
        .DoDouble(tmr2LastProcessedSystemCycles)
        .Do32(lastRtcAlarmCheck)
        .DoBool(pwmActive);

    if (version >= 2) helper.Do8(padcFifoReadIndex);
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::SetSubBankHandlers
// ---------------------------------------------------------------------------

void EmRegsSZ::SetSubBankHandlers(void) {
    // Install base handlers.

    EmRegs::SetSubBankHandlers();

    // Now add standard/specialized handers for the defined registers.

    INSTALL_HANDLER(StdRead, StdWrite, dmaControl);
    INSTALL_HANDLER(StdRead, StdWrite, dmaInterruptStatus);
    INSTALL_HANDLER(StdRead, StdWrite, dmaInterruptMask);
    INSTALL_HANDLER(StdRead, StdWrite, dmaBurstTimeOutStatus);
    INSTALL_HANDLER(StdRead, StdWrite, dmaRequestTimeOutStatus);
    INSTALL_HANDLER(StdRead, StdWrite, dmaBurstTimeOutControl);

    INSTALL_HANDLER(StdRead, StdWrite, mem0SourceAddress);
    INSTALL_HANDLER(StdRead, StdWrite, mem0DestAddress);
    INSTALL_HANDLER(StdRead, StdWrite, mem0Count);
    INSTALL_HANDLER(StdRead, StdWrite, mem0Control);
    INSTALL_HANDLER(StdRead, StdWrite, mem0BurstLength);
    INSTALL_HANDLER(StdRead, StdWrite, mem0BusUtilizationControl);
    INSTALL_HANDLER(StdRead, StdWrite, mem0BlockLength);
    INSTALL_HANDLER(StdRead, StdWrite, mem0BlockSeparationDist);
    INSTALL_HANDLER(StdRead, StdWrite, mem0DMARequestTimeOut);

    INSTALL_HANDLER(StdRead, StdWrite, mem1SourceAddress);
    INSTALL_HANDLER(StdRead, StdWrite, mem1DestAddress);
    INSTALL_HANDLER(StdRead, StdWrite, mem1Count);
    INSTALL_HANDLER(StdRead, StdWrite, mem1Control);
    INSTALL_HANDLER(StdRead, StdWrite, mem1BurstLength);
    INSTALL_HANDLER(StdRead, StdWrite, mem1BusUtilizationControl);
    INSTALL_HANDLER(StdRead, StdWrite, mem1BlockLength);
    INSTALL_HANDLER(StdRead, StdWrite, mem1BlockSeparationDist);
    INSTALL_HANDLER(StdRead, StdWrite, mem1DMARequestTimeOut);

    INSTALL_HANDLER(StdRead, StdWrite, io2MemoryAddress);
    INSTALL_HANDLER(StdRead, StdWrite, io2PeripheralAddress);
    INSTALL_HANDLER(StdRead, StdWrite, io2CountRegister);
    INSTALL_HANDLER(StdRead, StdWrite, io2Control);
    INSTALL_HANDLER(StdRead, StdWrite, io2RequestSourceSelect);
    INSTALL_HANDLER(StdRead, StdWrite, io2BurstLength);
    INSTALL_HANDLER(StdRead, StdWrite, io2DMARequestTimeOut);

    INSTALL_HANDLER(StdRead, StdWrite, io3MemoryAddress);
    INSTALL_HANDLER(StdRead, StdWrite, io3PeripheralAddress);
    INSTALL_HANDLER(StdRead, StdWrite, io3CountRegister);
    INSTALL_HANDLER(StdRead, StdWrite, io3Control);
    INSTALL_HANDLER(StdRead, StdWrite, io3RequestSourceSelect);
    INSTALL_HANDLER(StdRead, StdWrite, io3BurstLength);
    INSTALL_HANDLER(StdRead, StdWrite, io3DMARequestTimeOut);

    INSTALL_HANDLER(StdRead, StdWrite, io4MemoryAddress);
    INSTALL_HANDLER(StdRead, StdWrite, io4PeripheralAddress);
    INSTALL_HANDLER(StdRead, StdWrite, io4CountRegister);
    INSTALL_HANDLER(StdRead, StdWrite, io4Control);
    INSTALL_HANDLER(StdRead, StdWrite, io4RequestSourceSelect);
    INSTALL_HANDLER(StdRead, StdWrite, io4BurstLength);
    INSTALL_HANDLER(StdRead, StdWrite, io4DMARequestTimeOut);

    INSTALL_HANDLER(StdRead, StdWrite, io5MemoryAddress);
    INSTALL_HANDLER(StdRead, StdWrite, io5PeripheralAddress);
    INSTALL_HANDLER(StdRead, StdWrite, io5CountRegister);
    INSTALL_HANDLER(StdRead, StdWrite, io5Control);
    INSTALL_HANDLER(StdRead, StdWrite, io5RequestSourceSelect);
    INSTALL_HANDLER(StdRead, StdWrite, io5BurstLength);
    INSTALL_HANDLER(StdRead, StdWrite, io5DMARequestTimeOut);

    INSTALL_HANDLER(penSampleFifoRead, StdWrite, adcPenSampleFifo);
    INSTALL_HANDLER(StdRead, adcControlWrite, adcControl);
    INSTALL_HANDLER(StdRead, StdWrite, adcPenADSampleRateControl);
    INSTALL_HANDLER(StdRead, StdWrite, adcInterruptControl);
    INSTALL_HANDLER(StdRead, NullWrite, adcInterruptErrorStatus);
    INSTALL_HANDLER(StdRead, StdWrite, adcClockDivide);
    INSTALL_HANDLER(StdRead, StdWrite, adcCompareControl);

    INSTALL_HANDLER(StdRead, StdWrite, mmcClockControl);
    INSTALL_HANDLER(StdRead, StdWrite, mmcStatus);
    INSTALL_HANDLER(StdRead, StdWrite, mmcClockRate);
    INSTALL_HANDLER(StdRead, StdWrite, mmcCommandDataControl);
    INSTALL_HANDLER(StdRead, StdWrite, mmcResponseTimeOut);
    INSTALL_HANDLER(StdRead, StdWrite, mmcReadTimeOut);
    INSTALL_HANDLER(StdRead, StdWrite, mmcBlockLength);
    INSTALL_HANDLER(StdRead, StdWrite, mmcNumberOfBlocks);
    INSTALL_HANDLER(StdRead, StdWrite, mmcRevisionNumber);
    INSTALL_HANDLER(StdRead, StdWrite, mmcIOPort);
    INSTALL_HANDLER(StdRead, StdWrite, mmcInterruptMask);
    INSTALL_HANDLER(StdRead, StdWrite, mmcCommandNumber);
    INSTALL_HANDLER(StdRead, StdWrite, mmcHigherArgument);
    INSTALL_HANDLER(StdRead, StdWrite, mmcLowerArgument);
    INSTALL_HANDLER(StdRead, StdWrite, mmcResponseFifo);
    INSTALL_HANDLER(StdRead, StdWrite, mmcBufferAccess);
    INSTALL_HANDLER(StdRead, StdWrite, mmcBufferPartFull);

    INSTALL_HANDLER(StdRead, NullWrite, usbFrameNumberMatch);
    INSTALL_HANDLER(StdRead, NullWrite, usbSpecificationRelease);
    INSTALL_HANDLER(StdRead, NullWrite, usbStatus);
    INSTALL_HANDLER(StdRead, NullWrite, usbControl);
    INSTALL_HANDLER(StdRead, NullWrite, usbConfigurationStatus);
    INSTALL_HANDLER(StdRead, NullWrite, usbEndpointBufferData);
    INSTALL_HANDLER(StdRead, NullWrite, usbGeneralInterruptStatus);
    INSTALL_HANDLER(StdRead, NullWrite, usbInterruptMask);
    INSTALL_HANDLER(StdRead, NullWrite, usbEnable);
    INSTALL_HANDLER(StdRead, NullWrite, usbInterruptStatus);

    INSTALL_HANDLER(StdRead, NullWrite, usbEP0StatusControl);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP0InterruptMask);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP0FifoData);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP0FifoStatus);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP0FifoControl);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP0LastReadFramePointer);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP0LastWriteFramePointer);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP0FifoAlarm);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP0FifoReadPointer);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP0FifoWritePointer);

    INSTALL_HANDLER(StdRead, NullWrite, usbEP1StatusControl);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP1InterruptMask);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP1FifoData);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP1FifoStatus);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP1FifoControl);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP1LastReadFramePointer);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP1LastWriteFramePointer);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP1FifoAlarm);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP1FifoReadPointer);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP1FifoWritePointer);

    INSTALL_HANDLER(StdRead, NullWrite, usbEP2StatusControl);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP2InterruptMask);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP2FifoData);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP2FifoStatus);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP2FifoControl);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP2LastReadFramePointer);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP2LastWriteFramePointer);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP2FifoAlarm);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP2FifoReadPointer);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP2FifoWritePointer);

    INSTALL_HANDLER(StdRead, NullWrite, usbEP3StatusControl);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP3InterruptMask);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP3FifoData);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP3FifoStatus);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP3FifoControl);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP3LastReadFramePointer);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP3LastWriteFramePointer);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP3FifoAlarm);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP3FifoReadPointer);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP3FifoWritePointer);

    INSTALL_HANDLER(StdRead, NullWrite, usbEP4StatusControl);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP4InterruptMask);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP4FifoData);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP4FifoStatus);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP4FifoControl);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP4LastReadFramePointer);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP4LastWriteFramePointer);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP4FifoAlarm);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP4FifoReadPointer);
    INSTALL_HANDLER(StdRead, NullWrite, usbEP4FifoWritePointer);

    INSTALL_HANDLER(StdRead, StdWrite, scr);

    INSTALL_HANDLER(StdRead, StdWrite, pcr);
    INSTALL_HANDLER(StdRead, NullWrite, chipID);
    INSTALL_HANDLER(StdRead, NullWrite, maskID);
    INSTALL_HANDLER(StdRead, NullWrite, swID);
    INSTALL_HANDLER(StdRead, StdWrite, ioDriveControl);

    INSTALL_HANDLER(StdRead, StdWrite, csAGroupBase);
    INSTALL_HANDLER(StdRead, StdWrite, csBGroupBase);
    INSTALL_HANDLER(StdRead, StdWrite, csCGroupBase);
    INSTALL_HANDLER(StdRead, StdWrite, csDGroupBase);
    INSTALL_HANDLER(StdRead, StdWrite, csEGroupBase);
    INSTALL_HANDLER(StdRead, StdWrite, csFGroupBase);
    INSTALL_HANDLER(StdRead, csgRegWrite, csGGroupBase);

    INSTALL_HANDLER(StdRead, csControl1Write, csControl1);
    INSTALL_HANDLER(StdRead, StdWrite, csControl2);
    INSTALL_HANDLER(StdRead, StdWrite, csControl3);

    INSTALL_HANDLER(StdRead, csASelectWrite, csASelect);
    INSTALL_HANDLER(StdRead, StdWrite, csBSelect);
    INSTALL_HANDLER(StdRead, StdWrite, csCSelect);
    INSTALL_HANDLER(StdRead, StdWrite, csDSelect);
    INSTALL_HANDLER(StdRead, csESelectWrite, csESelect);
    INSTALL_HANDLER(StdRead, StdWrite, csFSelect);
    INSTALL_HANDLER(StdRead, csgRegWrite, csGSelect);

    INSTALL_HANDLER(StdRead, StdWrite, emuCS);

    INSTALL_HANDLER(StdRead, pllRegisterWrite, pllControl);
    INSTALL_HANDLER(StdRead, StdWrite, pllFreqSel0);
    INSTALL_HANDLER(StdRead, StdWrite, pllFreqSel1);
    INSTALL_HANDLER(StdRead, StdWrite, pwrControl);

    INSTALL_HANDLER(StdRead, StdWrite, usbpllFreqSel0);
    INSTALL_HANDLER(StdRead, StdWrite, usbpllFreqSel1);

    INSTALL_HANDLER(StdRead, StdWrite, clockSrcCtl);

    INSTALL_HANDLER(StdRead, StdWrite, intVector);
    INSTALL_HANDLER(StdRead, StdWrite, intControl);
    INSTALL_HANDLER(StdRead, intMaskHiWrite, intMaskHi);
    INSTALL_HANDLER(StdRead, intMaskLoWrite, intMaskLo);
    INSTALL_HANDLER(StdRead, intStatusHiWrite, intStatusHi);
    INSTALL_HANDLER(StdRead, NullWrite, intStatusLo);
    INSTALL_HANDLER(StdRead, NullWrite, intPendingHi);
    INSTALL_HANDLER(StdRead, NullWrite, intPendingLo);
    INSTALL_HANDLER(StdRead, StdWrite, intLevelControl);
    INSTALL_HANDLER(StdRead, StdWrite, intLevelControl2);
    INSTALL_HANDLER(StdRead, StdWrite, intLevelControl3);
    INSTALL_HANDLER(StdRead, StdWrite, intLevelControl4);
    INSTALL_HANDLER(StdRead, StdWrite, intLevelControl5);
    INSTALL_HANDLER(StdRead, StdWrite, intLevelControl6);
    INSTALL_HANDLER(StdRead, StdWrite, intLevelControl7);

    INSTALL_HANDLER(StdRead, StdWrite, portADir);
    INSTALL_HANDLER(portXDataRead, portXDataWrite, portAData);
    INSTALL_HANDLER(StdRead, StdWrite, portAPullupEn);

    INSTALL_HANDLER(StdRead, StdWrite, portBDir);
    INSTALL_HANDLER(portXDataRead, portXDataWrite, portBData);
    INSTALL_HANDLER(StdRead, StdWrite, portBPullupEn);
    INSTALL_HANDLER(StdRead, StdWrite, portBSelect);

    INSTALL_HANDLER(StdRead, StdWrite, portCDir);
    INSTALL_HANDLER(portXDataRead, portXDataWrite, portCData);
    INSTALL_HANDLER(StdRead, StdWrite, portCPulldnEn);
    INSTALL_HANDLER(StdRead, StdWrite, portCSelect);

    INSTALL_HANDLER(StdRead, StdWrite, portDDir);
    INSTALL_HANDLER(portXDataRead, portXDataWrite, portDData);
    INSTALL_HANDLER(StdRead, StdWrite, portDPullupEn);
    INSTALL_HANDLER(StdRead, StdWrite, portDSelect);
    INSTALL_HANDLER(StdRead, portXIntMaskWrite, portDIntMask);
    INSTALL_HANDLER(StdRead, portXIntStatusWrite, portDIntStatus);
    INSTALL_HANDLER(StdRead, StdWrite, portDIntEdge);
    INSTALL_HANDLER(StdRead, StdWrite, portDIntPolarity);

    INSTALL_HANDLER(StdRead, StdWrite, portEDir);
    INSTALL_HANDLER(portXDataRead, portXDataWrite, portEData);
    INSTALL_HANDLER(StdRead, StdWrite, portEPullupEn);
    INSTALL_HANDLER(StdRead, StdWrite, portESelect);
    INSTALL_HANDLER(StdRead, StdWrite, portEIntMask);
    INSTALL_HANDLER(StdRead, portXIntStatusWrite, portEIntStatus);
    INSTALL_HANDLER(StdRead, StdWrite, portEIntEdge);
    INSTALL_HANDLER(StdRead, StdWrite, portEIntPolarity);

    INSTALL_HANDLER(StdRead, StdWrite, portFDir);
    INSTALL_HANDLER(portXDataRead, portXDataWrite, portFData);
    INSTALL_HANDLER(StdRead, StdWrite, portFPullupdnEn);
    INSTALL_HANDLER(StdRead, StdWrite, portFSelect);
    INSTALL_HANDLER(StdRead, StdWrite, portFIntMask);
    INSTALL_HANDLER(StdRead, portXIntStatusWrite, portFIntStatus);
    INSTALL_HANDLER(StdRead, StdWrite, portFIntEdge);
    INSTALL_HANDLER(StdRead, StdWrite, portFIntPolarity);

    INSTALL_HANDLER(StdRead, StdWrite, portGDir);
    INSTALL_HANDLER(portXDataRead, portXDataWrite, portGData);
    INSTALL_HANDLER(StdRead, StdWrite, portGPullupEn);
    INSTALL_HANDLER(StdRead, StdWrite, portGSelect);
    INSTALL_HANDLER(StdRead, StdWrite, portGIntMask);
    INSTALL_HANDLER(StdRead, portXIntStatusWrite, portGIntStatus);
    INSTALL_HANDLER(StdRead, StdWrite, portGIntEdge);
    INSTALL_HANDLER(StdRead, StdWrite, portGIntPolarity);

    INSTALL_HANDLER(StdRead, StdWrite, portJDir);
    INSTALL_HANDLER(portXDataRead, portXDataWrite, portJData);
    INSTALL_HANDLER(StdRead, StdWrite, portJPullupEn);
    INSTALL_HANDLER(StdRead, StdWrite, portJSelect);
    INSTALL_HANDLER(StdRead, StdWrite, portJIntMask);
    INSTALL_HANDLER(StdRead, portXIntStatusWrite, portJIntStatus);
    INSTALL_HANDLER(StdRead, StdWrite, portJIntEdge);
    INSTALL_HANDLER(StdRead, StdWrite, portJIntPolarity);

    INSTALL_HANDLER(StdRead, StdWrite, portKDir);
    INSTALL_HANDLER(portXDataRead, portXDataWrite, portKData);
    INSTALL_HANDLER(StdRead, StdWrite, portKPullupdnEn);
    INSTALL_HANDLER(StdRead, StdWrite, portKSelect);
    INSTALL_HANDLER(StdRead, StdWrite, portKIntMask);
    INSTALL_HANDLER(StdRead, portXIntStatusWrite, portKIntStatus);
    INSTALL_HANDLER(StdRead, StdWrite, portKIntEdge);
    INSTALL_HANDLER(StdRead, StdWrite, portKIntPolarity);

    INSTALL_HANDLER(StdRead, StdWrite, portMDir);
    INSTALL_HANDLER(portXDataRead, portXDataWrite, portMData);
    INSTALL_HANDLER(StdRead, StdWrite, portMPullupdnEn);
    INSTALL_HANDLER(StdRead, StdWrite, portMSelect);
    INSTALL_HANDLER(StdRead, StdWrite, portMIntMask);
    INSTALL_HANDLER(StdRead, portXIntStatusWrite, portMIntStatus);
    INSTALL_HANDLER(StdRead, StdWrite, portMIntEdge);
    INSTALL_HANDLER(StdRead, StdWrite, portMIntPolarity);

    INSTALL_HANDLER(StdRead, StdWrite, portNDir);
    INSTALL_HANDLER(portXDataRead, portXDataWrite, portNData);
    INSTALL_HANDLER(StdRead, StdWrite, portNSelect);
    INSTALL_HANDLER(StdRead, StdWrite, portNIntMask);
    INSTALL_HANDLER(StdRead, portXIntStatusWrite, portNIntStatus);
    INSTALL_HANDLER(StdRead, StdWrite, portNIntEdge);
    INSTALL_HANDLER(StdRead, StdWrite, portNIntPolarity);

    INSTALL_HANDLER(StdRead, StdWrite, portPDir);
    INSTALL_HANDLER(portXDataRead, portXDataWrite, portPData);
    INSTALL_HANDLER(StdRead, StdWrite, portPPullupdnEn);
    INSTALL_HANDLER(StdRead, StdWrite, portPSelect);
    INSTALL_HANDLER(StdRead, StdWrite, portPIntMask);
    INSTALL_HANDLER(StdRead, portXIntStatusWrite, portPIntStatus);
    INSTALL_HANDLER(StdRead, StdWrite, portPIntEdge);
    INSTALL_HANDLER(StdRead, StdWrite, portPIntPolarity);

    INSTALL_HANDLER(StdRead, StdWrite, portRDir);
    INSTALL_HANDLER(portXDataRead, portXDataWrite, portRData);
    INSTALL_HANDLER(StdRead, StdWrite, portRPullupdnEn);
    INSTALL_HANDLER(StdRead, StdWrite, portRSelect);
    INSTALL_HANDLER(StdRead, StdWrite, portRIntMask);
    INSTALL_HANDLER(StdRead, portXIntStatusWrite, portRIntStatus);
    INSTALL_HANDLER(StdRead, StdWrite, portRIntEdge);
    INSTALL_HANDLER(StdRead, StdWrite, portRIntPolarity);

    INSTALL_HANDLER(StdRead, pwmc1Write, pwmControl);
    INSTALL_HANDLER(StdRead, StdWrite, pwmSampleHi);
    INSTALL_HANDLER(StdRead, pwms1Write, pwmSampleLo);
    INSTALL_HANDLER(StdRead, pwmp1Write, pwmPeriod);
    INSTALL_HANDLER(StdRead, NullWrite, pwmCounter);

    INSTALL_HANDLER(StdRead, StdWrite, pwm2Control);
    INSTALL_HANDLER(StdRead, StdWrite, pwm2Period);
    INSTALL_HANDLER(StdRead, StdWrite, pwm2Width);
    INSTALL_HANDLER(StdRead, NullWrite, pwm2Counter);

    INSTALL_HANDLER(StdRead, tmrRegisterWrite, tmr1Control);
    INSTALL_HANDLER(StdRead, tmrRegisterWrite, tmr1Prescaler);
    INSTALL_HANDLER(StdRead, tmrRegisterWrite, tmr1Compare);
    INSTALL_HANDLER(StdRead, tmrRegisterWrite, tmr1Capture);
    INSTALL_HANDLER(tmrRegisterRead, NullWrite, tmr1Counter);
    INSTALL_HANDLER(tmr1StatusRead, tmr1StatusWrite, tmr1Status);

    INSTALL_HANDLER(StdRead, tmrRegisterWrite, tmr2Control);
    INSTALL_HANDLER(StdRead, tmrRegisterWrite, tmr2Prescaler);
    INSTALL_HANDLER(StdRead, tmrRegisterWrite, tmr2Compare);
    INSTALL_HANDLER(StdRead, tmrRegisterWrite, tmr2Capture);
    INSTALL_HANDLER(tmrRegisterRead, NullWrite, tmr2Counter);
    INSTALL_HANDLER(tmr2StatusRead, tmr2StatusWrite, tmr2Status);

    INSTALL_HANDLER(StdRead, StdWrite, spiRxD);
    INSTALL_HANDLER(StdRead, StdWrite, spiTxD);
    INSTALL_HANDLER(StdRead, spiCont1Write, spiCont1);
    INSTALL_HANDLER(StdRead, StdWrite, spiIntCS);
    INSTALL_HANDLER(StdRead, StdWrite, spiTest);
    INSTALL_HANDLER(StdRead, StdWrite, spiSpc);

    INSTALL_HANDLER(StdRead, StdWrite, i2cAddress);
    INSTALL_HANDLER(StdRead, StdWrite, i2cFreqDiv);
    INSTALL_HANDLER(StdRead, StdWrite, i2cControl);
    INSTALL_HANDLER(StdRead, StdWrite, i2cStatus);
    INSTALL_HANDLER(StdRead, StdWrite, i2cDataIO);

    INSTALL_HANDLER(uart1Read, uart1Write, uControl);
    INSTALL_HANDLER(uart1Read, uart1Write, uBaud);
    INSTALL_HANDLER(uart1Read, uart1Write, uReceive);
    INSTALL_HANDLER(uart1Read, uart1Write, uTransmit);
    INSTALL_HANDLER(uart1Read, uart1Write, uMisc);
    INSTALL_HANDLER(StdRead, StdWrite, uNonIntPresc);
    INSTALL_HANDLER(StdRead, StdWrite, uFIFOHMark);

    INSTALL_HANDLER(uart2Read, uart2Write, u2Control);
    INSTALL_HANDLER(uart2Read, uart2Write, u2Baud);
    INSTALL_HANDLER(uart2Read, uart2Write, u2Receive);
    INSTALL_HANDLER(uart2Read, uart2Write, u2Transmit);
    INSTALL_HANDLER(uart2Read, uart2Write, u2Misc);
    INSTALL_HANDLER(StdRead, StdWrite, u2NonIntPresc);
    INSTALL_HANDLER(StdRead, StdWrite, u2FIFOHMark);

    INSTALL_HANDLER(rtcHourMinSecRead, StdWrite, rtcHourMinSec);
    INSTALL_HANDLER(StdRead, StdWrite, rtcAlarm);
    INSTALL_HANDLER(StdRead, StdWrite, rtcWatchDog);
    INSTALL_HANDLER(StdRead, rtcControlWrite, rtcControl);
    INSTALL_HANDLER(StdRead, rtcIntStatusWrite, rtcIntStatus);
    INSTALL_HANDLER(StdRead, rtcIntEnableWrite, rtcIntEnable);
    INSTALL_HANDLER(StdRead, StdWrite, stopWatch);
    INSTALL_HANDLER(StdRead, StdWrite, rtcDay);
    INSTALL_HANDLER(StdRead, StdWrite, rtcDayAlarm);

    INSTALL_HANDLER(StdRead, sdramControlEWrite, sdramControlE);
    INSTALL_HANDLER(StdRead, StdWrite, sdramControlF);

    INSTALL_HANDLER(StdRead, StdWrite, edoControlE);
    INSTALL_HANDLER(StdRead, StdWrite, edoControlF);

    INSTALL_HANDLER(StdRead, StdWrite, secondaryControl);

    INSTALL_HANDLER(StdRead, StdWrite, emuAddrCompare);
    INSTALL_HANDLER(StdRead, StdWrite, emuAddrMask);
    INSTALL_HANDLER(StdRead, StdWrite, emuControlCompare);
    INSTALL_HANDLER(StdRead, StdWrite, emuControlMask);
    INSTALL_HANDLER(StdRead, StdWrite, emuControl);
    INSTALL_HANDLER(StdRead, StdWrite, emuStatus);

    INSTALL_HANDLER(StdRead, lcdStartAddrWrite, lcdStartAddr);
    INSTALL_HANDLER(StdRead, lcdRegisterWrite, lcdScreenSize);
    INSTALL_HANDLER(StdRead, lcdRegisterWrite, lcdPageWidth);
    INSTALL_HANDLER(StdRead, StdWrite, lcdCursorXPos);
    INSTALL_HANDLER(StdRead, StdWrite, lcdCursorYPos);
    INSTALL_HANDLER(StdRead, StdWrite, lcdCursorSize);
    INSTALL_HANDLER(StdRead, StdWrite, lcdBlinkControl);
    INSTALL_HANDLER(StdRead, StdWrite, lcdColorCursorMapping);
    INSTALL_HANDLER(StdRead, StdWrite, lcdPanelControl0);
    INSTALL_HANDLER(StdRead, lcdRegisterWrite, lcdPanelControl1);
    INSTALL_HANDLER(StdRead, StdWrite, lcdHorizontalConfig0);
    INSTALL_HANDLER(StdRead, StdWrite, lcdHorizontalConfig1);
    INSTALL_HANDLER(StdRead, StdWrite, lcdVerticalConfig0);
    INSTALL_HANDLER(StdRead, StdWrite, lcdVerticalConfig1);
    INSTALL_HANDLER(StdRead, lcdRegisterWrite, lcdPanningOffset);
    INSTALL_HANDLER(StdRead, lcdRegisterWrite, lcdGrayPalette);
    INSTALL_HANDLER(StdRead, StdWrite, lcdPWMContrastControl);
    INSTALL_HANDLER(StdRead, StdWrite, lcdDMAControl);
    INSTALL_HANDLER(StdRead, StdWrite, lcdRefreshModeControl);
    INSTALL_HANDLER(StdRead, StdWrite, lcdInterruptConfiguration);
    INSTALL_HANDLER(StdRead, StdWrite, lcdInterruptStatus);
    INSTALL_HANDLER(StdRead, clutWrite, lcdCLUT);
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::GetRealAddress
// ---------------------------------------------------------------------------

uint8* EmRegsSZ::GetRealAddress(emuptr address) {
    uint8* loc = ((uint8*)&f68SZ328Regs) + (address - kMemoryStart);

    return loc;
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::GetAddressStart
// ---------------------------------------------------------------------------

emuptr EmRegsSZ::GetAddressStart(void) { return kMemoryStart; }

// ---------------------------------------------------------------------------
//		� EmRegsSZ::GetAddressRange
// ---------------------------------------------------------------------------

uint32 EmRegsSZ::GetAddressRange(void) { return kMemorySize; }

// ---------------------------------------------------------------------------
//		� EmRegsSZ::Cycle
// ---------------------------------------------------------------------------
// Handles periodic events that need to occur when the processor cycles (like
// updating timer registers).  This function is called in two places from
// Emulator::Execute.  Interestingly, the loop runs 3% FASTER if this function
// is in its own separate function instead of being inline.

inline void EmRegsSZ::Cycle(uint64 systemCycles, Bool sleeping) {
    if (unlikely(afterLoad)) {
        // DispatchPwmChange();
        afterLoad = false;
    }

    if (unlikely(powerOffCached)) return;

    this->systemCycles = systemCycles;
    if (unlikely(systemCycles >= nextTimerEventAfterCycle)) UpdateTimers();
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::CycleSlowly
// ---------------------------------------------------------------------------
// Handles periodic events that need to occur when the processor cycles (like
// updating timer registers).  This function is called in two places from
// Emulator::Execute.  Interestingly, the loop runs 3% FASTER if this function
// is in its own separate function instead of being inline.

void EmRegsSZ::CycleSlowly(Bool sleeping) {
    UNUSED_PARAM(sleeping)

    // See if a hard button is pressed.

    EmAssert(gSession);

    if (gSession->HasButtonEvent()) {
        ButtonEvent event = gSession->NextButtonEvent();
        if (event.GetButton() == ButtonEvent::Button::cradle)
            EmRegsSZ::HotSyncEvent(event.GetType() == ButtonEvent::Type::press);
        else
            EmRegsSZ::DispatchButtonEvent(event);
    }

    // See if there's anything new ("Put the data on the bus")

    EmRegsSZ::UpdateUARTState(false, 0);
    EmRegsSZ::UpdateUARTState(false, 1);

    // Check to see if the RTC alarm is ready to go off.  First see
    // if the RTC is enabled, and that the alarm event isn't already
    // registered (the latter check is just an optimization).

    if ((READ_REGISTER(rtcIntEnable) & hwrSZ328RTCIntEnableAlarm) != 0 &&
        (READ_REGISTER(rtcIntStatus) & hwrSZ328RTCIntStatusAlarm) == 0) {
        uint32 rtcAlarm = READ_REGISTER(rtcAlarm);

        uint32 almHour = (rtcAlarm & hwrSZ328RTCAlarmHoursMask) >> hwrSZ328RTCAlarmHoursOffset;
        uint32 almMin = (rtcAlarm & hwrSZ328RTCAlarmMinutesMask) >> hwrSZ328RTCAlarmMinutesOffset;
        uint32 almSec = (rtcAlarm & hwrSZ328RTCAlarmSecondsMask) >> hwrSZ328RTCAlarmSecondsOffset;
        int32 almInSeconds = (almHour * 60 * 60) + (almMin * 60) + almSec;

        uint32 hour, min, sec;
        Platform::GetTime(hour, min, sec);
        int32 nowInSeconds = hour * 3600 + min * 60 + sec;

        if (lastRtcAlarmCheck < almInSeconds && almInSeconds <= nowInSeconds) {
            WRITE_REGISTER(rtcIntStatus, READ_REGISTER(rtcIntStatus) | hwrSZ328RTCIntStatusAlarm);
            EmRegsSZ::UpdateRTCInterrupts();
        }

        lastRtcAlarmCheck = nowInSeconds;
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::TurnSoundOff
// ---------------------------------------------------------------------------

void EmRegsSZ::TurnSoundOff(void) {
    uint16 pwmControl = READ_REGISTER(pwmControl);
    WRITE_REGISTER(pwmControl, pwmControl & ~hwrSZ328PWMControlEnable);
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::ResetTimer
// ---------------------------------------------------------------------------

void EmRegsSZ::ResetTimer(void) {
    WRITE_REGISTER(tmr1Counter, 0);
    WRITE_REGISTER(tmr2Counter, 0);
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::ResetRTC
// ---------------------------------------------------------------------------

void EmRegsSZ::ResetRTC(void) {}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::GetInterruptLevel
// ---------------------------------------------------------------------------

int32 EmRegsSZ::GetInterruptLevel(void) {
    /*
            From the DragonBall SZ manual, Chapter 15:

                    * EMUIRQ or hardware breakpoint interrupt (level 7)

                    * IRQ6 external interrupt (level 6)
                    * IRQ3 external interrupt (level 3)
                    * IRQ2 external interrupt (level 2)
                    * IRQ1 external interrupt (level 1)
                    * Timer unit 1 (configurable from level 1 to 6)
                    * Timer unit 2 (configurable from level 1 to 6)
                    * Pulse-width modulator unit 1 (configurable from level 1 to 6)
                    * Pulse-width modulator unit 2 (configurable from level 1 to 6)

                    * LCD controller unit (configurable from level 1 to 6)
                    * Configurable serial peripheral interface unit (configurable from level 1 to 6)
                    * I2C (configurable from level 1 to 6)
                    * USB (configurable from level 1 to 6)
                    * DMA unit (configurable from level 1 to 6)
                    * A/D converter unit (configurable from level 1 to 6)
                    * MMC unit (configurable from level 1 to 6)

                    * UART unit 1 (configurable from level 1 to 6)
                    * UART unit 2 (configurable from level 1 to 6)
                    * Software watchdog timer interrupt (configurable from level 1 to 6)
                    * Real-time clock (configurable from level 1 to 6)
                    * Real-time interrupt (configurable from level 1 to 6)

                    * Port D,E,F,G,J,K,M,N,P,R (configurable from level 1 to 6)
    */

    static int8 intLevel[32] = {
        0,  // 0x00 - LCD controller (configurable)
        0,  // 0x01 - hwrSZ328IntLoTimer (configurable)
        0,  // 0x02 - hwrSZ328IntLoUART (configurable)
        0,  // 0x03 - hwrSZ328IntLoWDT (configurable)
        0,  // 0x04 - hwrSZ328IntLoRTC (configurable)
        0,  // 0x05 - hwrSZ328IntLoTimer2 (configurable)
        0,  // 0x06 - Port J (configurable)
        0,  // 0x07 - PWM1 (configurable)

        0,  // 0x08 - Port G (configurable)
        0,  // 0x09 - Port F (configurable)
        0,  // 0x0A - Port E (configurable)
        0,  // 0x0B - Port D (configurable)
        0,  // 0x0C - hwrSZ328IntLoUART2 (configurable)
        0,  // 0x0D - PWM2 (configurable)
        0,  // 0x0E - DMA2 (configurable)
        0,  // 0x0F - DMA (configurable)

        1,  // 0x10 - hwrSZ328IntHiIRQ1
        2,  // 0x11 - hwrSZ328IntHiIRQ2
        3,  // 0x12 - hwrSZ328IntHiIRQ3
        6,  // 0x13 - hwrSZ328IntHiIRQ6
        0,  // 0x14 - Port R (configurable)
        0,  // 0x15 - Configurable serial peripheral interface unit (configurable)
        0,  // 0x16 - RTC Timer (configurable)
        7,  // 0x17 - hwrSZ328IntHiEMU

        0,  // 0x18 - A/D converter (configurable)
        0,  // 0x19 - Port P (configurable)
        0,  // 0x1A - Port N (configurable)
        0,  // 0x1B - Port M (configurable)
        0,  // 0x1C - Port K (configurable)
        0,  // 0x1D - MMC (configurable)
        0,  // 0x1E - I2C (configurable)
        0   // 0x1F - USB (configurable)
    };

    // Load in the configurable interrupt levels. With the SZ's
    // many configurable interrupt levels, this section is now the bulk
    // of the function. Progress is a pain to code.

    uint16 intLevelControl1 = READ_REGISTER(intLevelControl);
    uint16 intLevelControl2 = READ_REGISTER(intLevelControl2);
    uint16 intLevelControl3 = READ_REGISTER(intLevelControl3);
    uint16 intLevelControl4 = READ_REGISTER(intLevelControl4);
    uint16 intLevelControl5 = READ_REGISTER(intLevelControl5);
    uint16 intLevelControl6 = READ_REGISTER(intLevelControl6);
    uint16 intLevelControl7 = READ_REGISTER(intLevelControl7);

    // LCD controller
    intLevel[0x00] = (intLevelControl2 >> 4) & 0x000F;

    // hwrSZ328IntLoTimer
    intLevel[0x01] = (intLevelControl7 >> 0) & 0x000F;

    // hwrSZ328IntLoUART
    intLevel[0x02] = (intLevelControl4 >> 0) & 0x000F;

    // hwrSZ328IntLoWDT
    intLevel[0x03] = (intLevelControl4 >> 4) & 0x000F;

    // hwrSZ328IntLoRTC
    intLevel[0x04] = (intLevelControl4 >> 8) & 0x000F;

    // hwrSZ328IntLoTimer2
    intLevel[0x05] = (intLevelControl1 >> 0) & 0x000F;

    // Port J
    intLevel[0x06] = (intLevelControl6 >> 0) & 0x000F;

    // PWM 1
    intLevel[0x07] = (intLevelControl3 >> 0) & 0x000F;

    // Port G
    intLevel[0x08] = (intLevelControl6 >> 4) & 0x000F;

    // Port F
    intLevel[0x09] = (intLevelControl6 >> 8) & 0x000F;

    // Port E
    intLevel[0x0A] = (intLevelControl6 >> 12) & 0x000F;

    // Port D
    intLevel[0x0B] = (intLevelControl5 >> 0) & 0x000F;

    // hwrSZ328IntLoUART2
    intLevel[0x0C] = (intLevelControl1 >> 8) & 0x000F;

    // PWM2
    intLevel[0x0D] = (intLevelControl1 >> 4) & 0x000F;

    // DMA2
    intLevel[0x0E] = (intLevelControl3 >> 4) & 0x000F;

    // DMA
    intLevel[0x0F] = (intLevelControl3 >> 8) & 0x000F;

    // Port R
    intLevel[0x14] = (intLevelControl7 >> 4) & 0x000F;

    // CSP
    intLevel[0x15] = (intLevelControl1 >> 12) & 0x000F;

    // RTC Timer
    intLevel[0x16] = (intLevelControl3 >> 12) & 0x000F;

    // ADC
    intLevel[0x18] = (intLevelControl2 >> 0) & 0x000F;

    // Port P
    intLevel[0x19] = (intLevelControl5 >> 4) & 0x000F;

    // Port M
    intLevel[0x1A] = (intLevelControl5 >> 12) & 0x000F;

    // Port N
    intLevel[0x1B] = (intLevelControl5 >> 8) & 0x000F;

    // Port K
    intLevel[0x1C] = (intLevelControl4 >> 12) & 0x000F;

    // MMC
    intLevel[0x1D] = (intLevelControl2 >> 8) & 0x000F;

    // I2C
    intLevel[0x1E] = (intLevelControl2 >> 12) & 0x000F;

    // USB
    intLevel[0x1F] = (intLevelControl7 >> 8) & 0x000F;

    // Find the highest interrupt level.

    int8 result = -1;
    int index = 0;

    uint16 intStatusHi = READ_REGISTER(intStatusHi);
    uint16 intStatusLo = READ_REGISTER(intStatusLo);
    uint32 intStatus = (((uint32)intStatusHi) << 16) | intStatusLo;

    while (intStatus) {
        if (intStatus & 1) {
            // Make sure no undefined bits are set.
            EmAssert(intLevel[index] >= 0);

            if (result < intLevel[index]) {
                result = intLevel[index];
            }
        }

        intStatus >>= 1;
        index++;
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::GetInterruptBase
// ---------------------------------------------------------------------------

int32 EmRegsSZ::GetInterruptBase(void) { return READ_REGISTER(intVector) & 0xF8; }

// ---------------------------------------------------------------------------
//		� EmRegsSZ::GetLCDHasFrame
// ---------------------------------------------------------------------------

Bool EmRegsSZ::GetLCDHasFrame(void) { return false; }

// ---------------------------------------------------------------------------
//		� EmRegsSZ::GetDynamicHeapSize
// ---------------------------------------------------------------------------

int32 EmRegsSZ::GetDynamicHeapSize(void) {
    int32 result = 0;

    uint16 csControl = READ_REGISTER(csControl1);
    uint16 csESelect = READ_REGISTER(csESelect);

    uint16 csEUPSIZ = (csESelect & UPSIZMask) >> UPSIZShift;
    uint16 csESIZ = (csESelect & SIZMask) >> SIZShift;

    if ((csControl & EUPENMask) != 0) {
        // Unprotected size = Chip-select Size/2^(7-upsize)

        // Determine upsize.

        // Merge in the extended bit.  UPSIZ contains [1:0].  The bit
        // in EUPS2 extends this to [2:0].

        csEUPSIZ |= ((csControl & EUPS2Mask) >> EUPS2Shift) << 2;

        // Determine Chip-select Size.

        long chip_select_size = (256 * 1024L) << csESIZ;

        // Erratum: 11 instead of 7
        result = chip_select_size / (1 << ((revision == Revision::rev10 ? 11 : 7) - csEUPSIZ));
    } else {
        result = (32 * 1024L) << csEUPSIZ;
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::GetROMSize
// ---------------------------------------------------------------------------

int32 EmRegsSZ::GetROMSize(void) {
    /*
            SIZ	Bits 3-1	Chip-Select Size

            This field determines the memory range of the chip-select.

                    000 = 256K byte
                    001 = 512K byte
                    011 = 1 M byte
                    011 = 2 M byte
                    101 = 4 Mbyte
                    101 = 8 Mbyte
                    110 = 16 Mbyte
                    111 = 32 Mbyte
    */

    uint16 csASelect = READ_REGISTER(csASelect);
    uint32 result = (256 * 1024L) << ((csASelect & SIZMask) >> SIZShift);

    if ((csASelect & ENMask) == 0) {
        result = 32 * 1024L * 1024L;
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::GetROMBaseAddress
// ---------------------------------------------------------------------------

uint32 EmRegsSZ::GetROMBaseAddress(void) {
    /*
            7.3.1 Chip-Select Group Base Address Registers

            The upper 15 bits of each base address register selects the starting
            address for the chip-select address range. The GBAx field is compared
            to the address on the address bus to determine if the group is decoded.
            The chip-select base address must be set according to the size of the
            corresponding chip-select signals of the group. For example, if CSA1
            and CSA0 are each assigned a 2 Mbyte memory space, the CSGBA register
            must be set in a 4 Mbyte space boundary, such as system address 0 � 0,
            0 � 4 Mbyte, 0 � 8 Mbyte, and so on. It cannot be set at 0 � 1 Mbyte,
            0 � 2 Mbyte, 0 � 3 Mbyte, 0 � 5 Mbyte, and so on.

            ...

            GBAx	Bits 15-2	Group A Base Address

            These bits select the high-order bits (31-18) of the starting address
            for the chip-select range.

            The chip-select base address must be set according to the size of the
            corresponding chip-select signals of the group.
    */

    if (!this->ChipSelectsConfigured()) {
        return 0xFFFFFFFF;
    }

    uint16 csAGroupBase = READ_REGISTER(csAGroupBase);
    uint32 result = ((uint32)csAGroupBase) << kBaseAddressShift;

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::ChipSelectsConfigured
// ---------------------------------------------------------------------------

Bool EmRegsSZ::ChipSelectsConfigured(void) { return READ_REGISTER(csASelect) & ENMask; }

// ---------------------------------------------------------------------------
//		� EmRegsSZ::GetSystemClockFrequency431.,
// ---------------------------------------------------------------------------

int32 EmRegsSZ::GetSystemClockFrequency(void) {
    constexpr uint32 clockRef = 32768;
    constexpr uint32 premult = 512;

    const uint32 mpfsr0 = READ_REGISTER(pllFreqSel0);
    const uint32 mpfsr1 = READ_REGISTER(pllFreqSel1);
    const uint32 mmfi = (mpfsr0 >> 11) & 0x0f;
    const uint32 mmfn = mpfsr0 & 0x3ff;
    const uint32 mmfd = (mpfsr1 + 1) & 0x3ff;
    const uint32 mpdf = ((mpfsr1 >> 11) + 1) & 0x0f;

    const uint32 mcupllClock =
        (2 * clockRef * premult * (static_cast<uint64>(mmfi) * mmfd + mmfn)) / (mpdf * mmfd);

    const uint32 cscr = READ_REGISTER(clockSrcCtl);
    const uint32 dmaClockDivider = (cscr & (1 << 10)) ? 1 : 2 << ((cscr >> 8) & 0x03);

    const uint32 cpuClock = mcupllClock / dmaClockDivider;

    return cpuClock;
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::GetAsleep
// ---------------------------------------------------------------------------

Bool EmRegsSZ::GetAsleep(void) {
    return ((READ_REGISTER(pllControl) & hwrSZ328PLLControlMCUDisable) != 0);
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::GetPortInputValue
// ---------------------------------------------------------------------------
// Return the GPIO values for the pins on the port.  These values are used
// if the select pins are high.

uint8 EmRegsSZ::GetPortInputValue(int port) {
    uint8 result = 0;

    if (port == 'D') {
        // Get the INT bits that need to be set.

        result = this->GetKeyBits();
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::GetPortInternalValue
// ---------------------------------------------------------------------------
// Return the dedicated values for the pins on the port.  These values are
// used if the select pins are low.

uint8 EmRegsSZ::GetPortInternalValue(int port) { return 0; }

// ---------------------------------------------------------------------------
//		� EmRegsSZ::PortDataChanged
// ---------------------------------------------------------------------------

void EmRegsSZ::PortDataChanged(int port, uint8, uint8 newValue) {
    if (port < 'D') return;

    UpdatePortXInterrupts((char)port);
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::portXDataRead
// ---------------------------------------------------------------------------

uint32 EmRegsSZ::portXDataRead(emuptr address, int) {
    // The value read can come from three different places:
    //
    //	- the value what was written to the data register
    //	- any dedicated inputs
    //	- any GPIO inputs
    //
    // The value returned depends on the settings of the SEL and DIR
    // registers.  So let's get those settings, the values from the three
    // input sources, and build up a return value based on those.

    int port = GetPortFromAddress(address);

    uint8 sel = StdRead(address + 2, 1);
    uint8 dir = StdRead(address - 1, 1);
    uint8 output = StdRead(address + 0, 1);
    uint8 input = EmHAL::GetPortInputValue(port);
    uint8 intFn = EmHAL::GetPortInternalValue(port);

    if (port == 'D') {
        if (fPortDDataCount != 0xFFFFFFFF && ++fPortDDataCount >= 20 * 2) {
            fPortDDataCount = 0xFFFFFFFF;
            gSession->ReleaseBootKeys();
        }
    }

    // Use the internal chip function bits if the "sel" bits are zero.

    intFn &= ~sel;

    // Otherwise, use the I/O bits.

    output &= sel & dir;  // Use the output bits if the "dir" is one.
    input &= sel & ~dir;  // Use the input bits if the "dir" is zero.

    // Mush everything together.

    uint8 result = output | input | intFn;

    // If this is port D-R, flip the bits if the POLARITY register says to.
    // (!!! Does this inversion apply only to input bits?  That is, the
    // bits where the "dir" register has 0 bits?)

    if ((0 && port == 'D') || (0 && port == 'E') || (0 && port == 'F') || (0 && port == 'G') ||
        (0 && port == 'J') || (0 && port == 'K') || (0 && port == 'M') || (0 && port == 'N') ||
        (0 && port == 'P') || (0 && port == 'R')) {
        uint8 polarity = READ_REGISTER(portDIntPolarity);
        PRINTF("EmRegsSZ::portXDataRead: polarity = 0x%02lX", (uint32)polarity);
        result ^= polarity;
    }

    PRINTF("EmRegsSZ::port%cDataRead: sel    dir    output input  intFn  result", (char)port);
    PRINTF("EmRegsSZ::port%cDataRead: 0x%02lX   0x%02lX   0x%02lX   0x%02lX   0x%02lX   0x%02lX",
           (char)port, (uint32)sel, (uint32)dir, (uint32)output, (uint32)input, (uint32)intFn,
           (uint32)result);

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::tmr1StatusRead
// ---------------------------------------------------------------------------

uint32 EmRegsSZ::tmr1StatusRead(emuptr address, int size) {
    UpdateTimers();

    // Remember this guy for later (see EmRegsSZ::tmr1StatusWrite())
    fLastTmr1Status |= READ_REGISTER(tmr1Status);

    // Finish up by doing a standard read.

    return EmRegsSZ::StdRead(address, size);
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::tmr2StatusRead
// ---------------------------------------------------------------------------

uint32 EmRegsSZ::tmr2StatusRead(emuptr address, int size) {
    UpdateTimers();

    // Remember this guy for later (see EmRegsSZ::tmr2StatusWrite())
    fLastTmr2Status |= READ_REGISTER(tmr2Status);

    // Finish up by doing a standard read.

    return EmRegsSZ::StdRead(address, size);
}

uint32 EmRegsSZ::tmrRegisterRead(emuptr address, int size) {
    UpdateTimers();

    return EmRegsSZ::StdRead(address, size);
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::uart1Read
// ---------------------------------------------------------------------------

uint32 EmRegsSZ::uart1Read(emuptr address, int size) {
    // If this is a full read, get the next byte from the FIFO.

    Bool refreshRxData = (address == db_addressof(uReceive)) && (size == 2);

    // See if there's anything new ("Put the data on the bus")

    EmRegsSZ::UpdateUARTState(refreshRxData, 0);

    // Finish up by doing a standard read.

    return EmRegsSZ::StdRead(address, size);
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::uart2Read
// ---------------------------------------------------------------------------

uint32 EmRegsSZ::uart2Read(emuptr address, int size) {
    // If this is a full read, get the next byte from the FIFO.

    Bool refreshRxData = (address == db_addressof(u2Receive)) && (size == 2);

    // See if there's anything new ("Put the data on the bus")

    EmRegsSZ::UpdateUARTState(refreshRxData, 1);

    // Finish up by doing a standard read.

    return EmRegsSZ::StdRead(address, size);
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::rtcHourMinSecRead
// ---------------------------------------------------------------------------

uint32 EmRegsSZ::rtcHourMinSecRead(emuptr address, int size) {
    // Get the desktop machine's time.
    uint32 hour, min, sec;
    Platform::GetTime(hour, min, sec);

    // Update the register.
    WRITE_REGISTER(rtcHourMinSec, (hour << hwrSZ328RTCHourMinSecHoursOffset) |
                                      (min << hwrSZ328RTCHourMinSecMinutesOffset) |
                                      (sec << hwrSZ328RTCHourMinSecSecondsOffset));

    // Finish up by doing a standard read.
    return EmRegsSZ::StdRead(address, size);
}

uint32 EmRegsSZ::penSampleFifoRead(emuptr address, int size) {
    uint32 adctrl = READ_REGISTER(adcControl);

    if ((adctrl & 0x01) == 0) return 0;

    uint8 tag = ((adctrl & 0x8000) >> 13) | ((adctrl >> 12) & 0x03);
    uint8_t readIndex = padcFifoReadIndex++;
    padcFifoReadIndex %= 12;

    switch (tag) {
        case 0x0:
        case 0x4:
            return 0;

        case 0x1:
        case 0x5:
            return 0;

        case 0x2:
            return (readIndex % 3) ? 0 : GetADCValueU();

        case 0x6:
            return (readIndex % 4) ? 0 : GetADCValueU();

        default:
            return GetADCValueU();
    }
}

void EmRegsSZ::UpdateFramebufferLocation() {
    esram->SetFramebufferBase(READ_REGISTER(lcdStartAddr));
}

void EmRegsSZ::UpdateEsramLocation() {
    uint16 csg = READ_REGISTER(csGSelect);
    uint16 csggb = READ_REGISTER(csGGroupBase);

    if (csg & 0x0001) {
        esram->Enable((csggb & 0xfffc) << 16);
    } else {
        esram->Disable();
    }
}

void EmRegsSZ::UpdatePalette() {
    if (!clutDirty) return;
    clutDirty = false;

    for (int i = 0; i < 256; i++) palette[i] = convertColor_12bit(READ_REGISTER(lcdCLUT[i]));
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::csControl1Write
// ---------------------------------------------------------------------------

void EmRegsSZ::csControl1Write(emuptr address, int size, uint32 value) {
    // Get the current value.

    uint16 csControl = READ_REGISTER(csControl1);

    // Do a standard update of the register.

    EmRegsSZ::StdWrite(address, size, value);

    // Check to see if the unprotected memory range changed.

    const uint16 bits = EUPENMask | EUPS2Mask;

    if ((csControl & bits) != (READ_REGISTER(csControl1) & bits)) {
        EmAssert(gSession);

        gSession->ScheduleResetBanks();
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::csASelectWrite
// ---------------------------------------------------------------------------

void EmRegsSZ::csASelectWrite(emuptr address, int size, uint32 value) {
    // Get the current value.

    uint16 csASelect = READ_REGISTER(csASelect);

    // Do a standard update of the register.

    EmRegsSZ::StdWrite(address, size, value);

    // Check to see if the unprotected memory range changed.

    if ((csASelect & SIZMask) != (READ_REGISTER(csASelect) & SIZMask)) {
        EmAssert(gSession);

        gSession->ScheduleResetBanks();
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::csESelectWrite
// ---------------------------------------------------------------------------

void EmRegsSZ::csESelectWrite(emuptr address, int size, uint32 value) {
    // Get the current value.

    uint16 csESelect = READ_REGISTER(csESelect);

    // Do a standard update of the register.

    EmRegsSZ::StdWrite(address, size, value);

    // Check its new state and update our ram-protect flag.

    gMemAccessFlags.fProtect_SRAMSet = (READ_REGISTER(csESelect) & ROPMask) != 0;

    // Check to see if the unprotected memory range changed.

    if ((csESelect & UPSIZMask) != (READ_REGISTER(csESelect) & UPSIZMask)) {
        EmAssert(gSession);

        gSession->ScheduleResetBanks();
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::intMaskHiWrite
// ---------------------------------------------------------------------------

void EmRegsSZ::intMaskHiWrite(emuptr address, int size, uint32 value) {
    // Do a standard update of the register.

    EmRegsSZ::StdWrite(address, size, value);

    // Respond to the new interrupt state.

    EmRegsSZ::UpdateInterrupts();
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::intMaskLoWrite
// ---------------------------------------------------------------------------

void EmRegsSZ::intMaskLoWrite(emuptr address, int size, uint32 value) {
    // Do a standard update of the register.

    EmRegsSZ::StdWrite(address, size, value);

    // Respond to the new interrupt state.

    EmRegsSZ::UpdateInterrupts();
    UpdateTimers();
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::intStatusHiWrite
// ---------------------------------------------------------------------------

void EmRegsSZ::intStatusHiWrite(emuptr address, int size, uint32 value) {
    // IRQ1, IRQ2, IRQ3, IRQ6 and IRQ7 are cleared by writing to their
    // respective status bits.  We handle those there.  Since there are
    // no interrupt status bits like this in intStatusLo, we don't need
    // a handler for that register; we only handle intStatusHi.

    // Even though this is a 16-bit register as defined by the Palm headers,
    // it's a 32-bit register according to DragonballSZ docs, and is in fact
    // accessed that way in the kernal files (cf. HwrIRQ4Handler). In those
    // cases, we're still only interested in access to the IRQ# bits, so we
    // can turn 4-byte accesses into 2-byte accesses.

    if (size == 4) value >>= 16;

    // Take into account the possibility of 1-byte accesses, too. If we're
    // accessing the upper byte, just return. If we're accessing the lower
    // byte, we can treat it as a 2-byte access.

    else if (size == 1 && address == db_addressof(intStatusHi))
        return;

    // Now we can treat the rest of this function as a word-write to intStatusHi.

    uint16 intPendingHi = READ_REGISTER(intPendingHi);

    //	For each interrupt:
    //		If we're writing to that interrupt's status bit and its edge bit is set:
    //			- clear the interrupt's pending bit
    //			- respond to the new interrupt state.

#undef CLEAR_PENDING_INTERRUPT
#define CLEAR_PENDING_INTERRUPT(edge, irq)                       \
    if ((READ_REGISTER(intControl) & edge) && (value & (irq))) { \
        intPendingHi &= ~(irq);                                  \
    }

    CLEAR_PENDING_INTERRUPT(hwrSZ328IntCtlEdge1, hwrSZ328IntHiIRQ1);
    CLEAR_PENDING_INTERRUPT(hwrSZ328IntCtlEdge2, hwrSZ328IntHiIRQ2);
    CLEAR_PENDING_INTERRUPT(hwrSZ328IntCtlEdge3, hwrSZ328IntHiIRQ3);
    CLEAR_PENDING_INTERRUPT(hwrSZ328IntCtlEdge6, hwrSZ328IntHiIRQ6);

    // IRQ7 is not edge-programmable, so clear it if we're merely writing to it.
    // !!! Double check this for SZ!

    if (value & hwrSZ328IntHiEMU) {
        intPendingHi &= ~(hwrSZ328IntHiEMU);
    }

    // If we're emulating the user pressing the hotsync button, make sure the
    // interrupt stays asserted.  (!!! Should we use the same technique for
    // other buttons, too?  It doesn't seem to be needed right now, but doing
    // that may more closely mirror the hardware.)

    if (fHotSyncButtonDown) {
        intPendingHi |= hwrSZ328IntHiIRQ1;
    } else {
        intPendingHi &= ~hwrSZ328IntHiIRQ1;
    }

    // This makes the power on key work. If the signal is asserted, the
    // unit will not transition between asleep and awake (cf. HwrSleep, HwrWake).

    intPendingHi &= ~hwrSZ328IntHiIRQ6;

    WRITE_REGISTER(intPendingHi, intPendingHi);
    EmRegsSZ::UpdateInterrupts();
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::portXDataWrite
// ---------------------------------------------------------------------------

void EmRegsSZ::portXDataWrite(emuptr address, int size, uint32 value) {
    // Get the old value before updating it.

    uint8 oldValue = StdRead(address, size);

    // Take a snapshot of the line driver states.

    Bool driverStates[kUARTEnd];
    EmHAL::GetLineDriverStates(driverStates);

    // Now update the value with a standard write.

    StdWrite(address, size, value);

    // Let anyone know that it's changed.

    int port = GetPortFromAddress(address);
    PRINTF("EmRegsSZ::port%cDataWrite: oldValue = 0x%02lX", (char)port, (uint32)(uint8)oldValue);
    PRINTF("EmRegsSZ::port%cDataWrite: newValue = 0x%02lX", (char)port, (uint32)(uint8)value);

    EmHAL::PortDataChanged(port, oldValue, value);

    // Respond to any changes in the line driver states.

    EmHAL::CompareLineDriverStates(driverStates);
}

void EmRegsSZ::portXIntMaskWrite(emuptr address, int size, uint32 value) {
    StdWrite(address, size, value);

    UpdatePortXInterrupts(GetPortFromAddress(address));
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::portXIntStatusWrite
// ---------------------------------------------------------------------------

void EmRegsSZ::portXIntStatusWrite(emuptr address, int size, uint32 value) {
    int port = EmRegsSZ::GetPortFromAddress(address);
    fPortXEdge[port - 'D'] &= ~value;

    UpdatePortXInterrupts(EmRegsSZ::GetPortFromAddress(address));
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::tmr1StatusWrite
// ---------------------------------------------------------------------------

void EmRegsSZ::tmr1StatusWrite(emuptr address, int size, uint32 value) {
    UNUSED_PARAM(address)
    UNUSED_PARAM(size)

    EmAssert(size == 2 || size == 4);  // This function's a hell of a lot easier to write if
                                       // we assume only full-register access.

    // CSTODO: Review
    value &= 0xffff;

    // Get the current value.

    uint16 tmr1Status = READ_REGISTER(tmr1Status);

    // If the user had previously read the status bits while they
    // were set, then it's OK for them to be clear now.  Otherwise,
    // we have to merge any set status bits back in.

    tmr1Status &=
        value | ~fLastTmr1Status;  // fLastTmr1Status was set in EmRegsSZ::tmr1StatusRead()

    WRITE_REGISTER(tmr1Status, tmr1Status);

    fLastTmr1Status = 0;
    if ((tmr1Status & hwrSZ328TmrStatusCompare) == 0) {
        uint16 intPendingLo = READ_REGISTER(intPendingLo) & ~hwrSZ328IntLoTimer;
        WRITE_REGISTER(intPendingLo, intPendingLo);

        // Respond to the new interrupt state.

        EmRegsSZ::UpdateInterrupts();
    }

    UpdateTimers();
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::tmr2StatusWrite
// ---------------------------------------------------------------------------

void EmRegsSZ::tmr2StatusWrite(emuptr address, int size, uint32 value) {
    UNUSED_PARAM(address)
    UNUSED_PARAM(size)

    EmAssert(size == 2 || size == 4);  // This function's a hell of a lot easier to write if
                                       // we assume only full-register access.

    // CSTODO: Review
    value &= 0xffff;

    // Get the current value.

    uint16 tmr2Status = READ_REGISTER(tmr2Status);

    // If the user had previously read the status bits while they
    // were set, then it's OK for them to be clear now.  Otherwise,
    // we have to merge any set status bits back in.

    tmr2Status &=
        value | ~fLastTmr2Status;  // fLastTmr2Status was set in EmRegsSZ::tmr2StatusRead()

    WRITE_REGISTER(tmr2Status, tmr2Status);

    fLastTmr2Status = 0;
    if ((tmr2Status & hwrSZ328TmrStatusCompare) == 0) {
        uint16 intPendingLo = READ_REGISTER(intPendingLo) & ~hwrSZ328IntLoTimer2;
        WRITE_REGISTER(intPendingLo, intPendingLo);

        // Respond to the new interrupt state.

        EmRegsSZ::UpdateInterrupts();
    }

    UpdateTimers();
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::spiCont1Write
// ---------------------------------------------------------------------------

void EmRegsSZ::spiCont1Write(emuptr address, int size, uint32 value) {
    // Do a standard update of the register.

    EmRegsSZ::StdWrite(address, size, value);

    // Get the current value.

    uint16 spiCont1 = READ_REGISTER(spiCont1);

    // Check to see if data exchange and enable are enabled.

#define BIT_MASK (hwrSZ328SPIMControlExchange | hwrSZ328SPIMControlEnable)
    if ((spiCont1 & BIT_MASK) == BIT_MASK) {
        // Clear the exchange bit.

        spiCont1 &= ~hwrSZ328SPIMControlExchange;

        WRITE_REGISTER(spiCont1, spiCont1);
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::uart1Write
// ---------------------------------------------------------------------------

void EmRegsSZ::uart1Write(emuptr address, int size, uint32 value) {
    // Do a standard update of the register.

    EmRegsSZ::StdWrite(address, size, value);

    // If this write included the TX_DATA field, signal that it needs to
    // be transmitted.

    Bool sendTxData = ((address == db_addressof(uTransmit)) && (size == 2)) ||
                      ((address == db_addressof(uTransmit) + 1) && (size == 1));

    // React to any changes.

    EmRegsSZ::UARTStateChanged(sendTxData, 0);
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::uart2Write
// ---------------------------------------------------------------------------

void EmRegsSZ::uart2Write(emuptr address, int size, uint32 value) {
    // Do a standard update of the register.

    EmRegsSZ::StdWrite(address, size, value);

    // If this write included the TX_DATA field, signal that it needs to
    // be transmitted.

    Bool sendTxData = ((address == db_addressof(u2Transmit)) && (size == 2)) ||
                      ((address == db_addressof(u2Transmit) + 1) && (size == 1));

    // React to any changes.

    EmRegsSZ::UARTStateChanged(sendTxData, 1);
}

void EmRegsSZ::pllRegisterWrite(emuptr address, int size, uint32 value) {
    EmRegsSZ::StdWrite(address, size, value);

    gSystemState.MarkScreenDirty();

    EmHAL::onSystemClockChange.Dispatch();

    UpdateTimers();
    powerOffCached = GetAsleep();
}

void EmRegsSZ::tmrRegisterWrite(emuptr address, int size, uint32 value) {
    EmRegsSZ::StdWrite(address, size, value);

    UpdateTimers();
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::rtcControlWrite
// ---------------------------------------------------------------------------

void EmRegsSZ::rtcControlWrite(emuptr address, int size, uint32 value) {
    // Do a standard update of the register.

    EmRegsSZ::StdWrite(address, size, value);

    // Respond to the new interrupt state.

    EmRegsSZ::UpdateRTCInterrupts();
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::rtcIntStatusWrite
// ---------------------------------------------------------------------------

void EmRegsSZ::rtcIntStatusWrite(emuptr address, int size, uint32 value) {
    // Status bits are cleared by writing ones to them.

    // If we're doing a byte-write to the upper byte, shift the byte
    // so that we can treat the operation as a word write.  If we're
    // doing a byte-write to the lower byte, this extension will happen
    // automatically.

    if (address == db_addressof(rtcIntStatus) && size == 1) value <<= 8;

    // Get the current value.

    uint16 rtcIntStatus = READ_REGISTER(rtcIntStatus);

    // Clear the requested bits.

    rtcIntStatus &= ~value;

    // Update the register.

    WRITE_REGISTER(rtcIntStatus, rtcIntStatus);

    // Respond to the new interrupt state.

    EmRegsSZ::UpdateRTCInterrupts();
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::rtcIntEnableWrite
// ---------------------------------------------------------------------------

void EmRegsSZ::rtcIntEnableWrite(emuptr address, int size, uint32 value) {
    // Do a standard update of the register.

    EmRegsSZ::StdWrite(address, size, value);

    // Respond to the new interrupt state.

    EmRegsSZ::UpdateRTCInterrupts();
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::ButtonEvent
// ---------------------------------------------------------------------------
// Handles a Palm device button event by updating the appropriate registers.

void EmRegsSZ::DispatchButtonEvent(ButtonEvent event) {
    uint16 bitNumber = this->ButtonToBits(event.GetButton());

    // Get the bits that should have been set with the previous set
    // of pressed keys.  We use this old value to update the port D interrupts.

    uint8 oldBits = this->GetKeyBits();

    // Update the set of keys that are currently pressed.

    if (event.GetType() == ButtonEvent::Type::press) {
        fKeyBits |= bitNumber;  // Remember the key bit
    } else {
        fKeyBits &= ~bitNumber;  // Forget the key bit
    }

    // Now get the new set of bits that should be set.

    uint8 newBits = this->GetKeyBits();

    PRINTF("EmRegsSZ::ButtonEvent: fKeyBits = 0x%04lX", (uint32)fKeyBits);
    PRINTF("EmRegsSZ::ButtonEvent: oldBits   = 0x%02lX", (uint32)oldBits);
    PRINTF("EmRegsSZ::ButtonEvent: newBits   = 0x%02lX", (uint32)newBits);

    // Set the interrupt bits for the bits that went from off to on.
    // These get cleared when portDData is written to.

    fPortXEdge['D' - 'D'] |= newBits & ~oldBits;

    PRINTF("EmRegsSZ::ButtonEvent: fPortDEdge = 0x%02lX", (uint32)fPortXEdge['D' - 'D']);

    // Set the new interrupt state.

    UpdatePortXInterrupts('D');
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::HotSyncEvent
// ---------------------------------------------------------------------------
// Handles a HotSync button event by updating the appropriate registers.

void EmRegsSZ::HotSyncEvent(Bool buttonIsDown) {
    // If the button changes state, set or clear the HotSync interrupt.

    uint16 intPendingHi = READ_REGISTER(intPendingHi);

    if (buttonIsDown) {
        intPendingHi |= hwrSZ328IntHiIRQ1;
        fHotSyncButtonDown = true;
    } else {
        intPendingHi &= ~hwrSZ328IntHiIRQ1;
        fHotSyncButtonDown = false;
    }

    WRITE_REGISTER(intPendingHi, intPendingHi);

    EmRegsSZ::UpdateInterrupts();
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::GetKeyBits
// ---------------------------------------------------------------------------

uint8 EmRegsSZ::GetKeyBits(void) {
    // "Keys" (that is, buttons) are read from the Port D Data register.
    // There are 7 or 8 keys that can be pressed, but only 4 bits are
    // available in the Port D Data register for reporting pressed keys.
    // Therefore, the keys are organized into a matrix, one row or which
    // can be requested and reported at a time.  This function determines
    // what row is being requested, and sets the appropriate Port D Data
    // bits for the keys are are currently pressed.

    int numRows;
    int numCols;
    uint16 keyMap[16];
    Bool rows[4];

    this->GetKeyInfo(&numRows, &numCols, keyMap, rows);

    uint8 keyData = 0;

    // Walk the rows, looking for one that is requested.

    for (int row = 0; row < numRows; ++row) {
        if (rows[row]) {
            // Walk the columns, looking for ones that have a pressed key.

            for (int col = 0; col < numCols; ++col) {
                // Get the key corresponding to this row and column.
                // If we've recorded (in fKeyBits) that this key is
                // pressed, then set its column bit.

                uint16 key = keyMap[row * numCols + col];
                if ((key & fKeyBits) != 0) {
                    keyData |= (1 << col);
                }
            }
        }
    }

    return ~keyData & (0xff >> (8 - numCols));
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::ButtonToBits
// ---------------------------------------------------------------------------

uint16 EmRegsSZ::ButtonToBits(ButtonEvent::Button button) {
    switch (button) {
        case ButtonEvent::Button::power:
            return keyBitPower;

        case ButtonEvent::Button::rockerUp:
            return keyBitPageUp;

        case ButtonEvent::Button::rockerDown:
            return keyBitPageDown;

        case ButtonEvent::Button::app1:
            return keyBitHard1;

        case ButtonEvent::Button::app2:
            return keyBitHard2;

        case ButtonEvent::Button::app3:
            return keyBitHard3;

        case ButtonEvent::Button::app4:
            return keyBitHard4;

        case ButtonEvent::Button::cradle:
            return keyBitCradle;

        case ButtonEvent::Button::antenna:
            return keyBitAntenna;

        case ButtonEvent::Button::contrast:
            return keyBitContrast;

        default:
            return 0;
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::GetSPISlave
// ---------------------------------------------------------------------------

EmSPISlave* EmRegsSZ::GetSPISlave(void) { return NULL; }

// ---------------------------------------------------------------------------
//		� EmRegsSZ::UpdateInterrupts
// ---------------------------------------------------------------------------
// Determines whether an interrupt has occurred by copying the Interrupt
// Pending Register to the Interrupt Status Register.

void EmRegsSZ::UpdateInterrupts(void) {
    // Copy the Interrupt Pending Register to the Interrupt Status
    // Register, but ignore interrupts that are being masked.

    // Note: this function is not sensitive to the byte ordering of the registers,
    // so their contents don't need to be accessed via READ_REGISTER or WRITE_REGISTER.

    f68SZ328Regs.intStatusHi = f68SZ328Regs.intPendingHi & ~f68SZ328Regs.intMaskHi;
    f68SZ328Regs.intStatusLo = f68SZ328Regs.intPendingLo & ~f68SZ328Regs.intMaskLo;

    PRINTF("EmRegsSZ::UpdateInterrupts: intMask    = 0x%04lX %04lX", (uint32)f68SZ328Regs.intMaskHi,
           (uint32)f68SZ328Regs.intMaskLo);

    PRINTF("EmRegsSZ::UpdateInterrupts: intPending = 0x%04lX %04lX",
           (uint32)f68SZ328Regs.intPendingHi, (uint32)f68SZ328Regs.intPendingLo);

    // If the Interrupt Status Register isn't clear, flag an interrupt.

    if (f68SZ328Regs.intStatusHi || f68SZ328Regs.intStatusLo) {
        regs.spcflags |= SPCFLAG_INT;

        PRINTF("EmRegsSZ::UpdateInterrupts: intStatus  = 0x%04lX %04lX",
               (uint32)f68SZ328Regs.intStatusHi, (uint32)f68SZ328Regs.intStatusLo);
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::UpdatePortXInterrupts
// ---------------------------------------------------------------------------
// Determine what interrupts need to be generated based on the current
// settings in portXData and fPortXEdge.

void EmRegsSZ::UpdatePortXInterrupts(char port) {
    // Initialize the variable to hold the new interrupt settings.

    uint8 newBits = 0;

    // Get some other values we're going to need:

    uint8 portXDir;
    uint8 portXData;
    uint8 portXIntMask;
    uint8 portXIntStatus;
    uint8 portXIntEdge;
    uint8 portXIntPolarity;

    switch (port) {
        case 'D':
            portXDir = READ_REGISTER(portDDir);  // Interrupt on inputs only (when pin is low)
            portXData = EmHAL::GetPortInputValue('D');
            portXIntMask = READ_REGISTER(portDIntMask);
            portXIntStatus = READ_REGISTER(portDIntStatus);
            portXIntEdge = READ_REGISTER(portDIntEdge);
            portXIntPolarity = READ_REGISTER(portDIntPolarity);
            break;

        case 'E':
            portXDir = READ_REGISTER(portEDir);  // Interrupt on inputs only (when pin is low)
            portXData = EmHAL::GetPortInputValue('E');
            portXIntMask = READ_REGISTER(portEIntMask);
            portXIntStatus = READ_REGISTER(portEIntStatus);
            portXIntEdge = READ_REGISTER(portEIntEdge);
            portXIntPolarity = READ_REGISTER(portEIntPolarity);
            break;

        case 'F':
            portXDir = READ_REGISTER(portFDir);  // Interrupt on inputs only (when pin is low)
            portXData = EmHAL::GetPortInputValue('F');
            portXIntMask = READ_REGISTER(portFIntMask);
            portXIntStatus = READ_REGISTER(portFIntStatus);
            portXIntEdge = READ_REGISTER(portFIntEdge);
            portXIntPolarity = READ_REGISTER(portFIntPolarity);
            break;

        case 'G':
            portXDir = READ_REGISTER(portGDir);  // Interrupt on inputs only (when pin is low)
            portXData = EmHAL::GetPortInputValue('G');
            portXIntMask = READ_REGISTER(portGIntMask);
            portXIntStatus = READ_REGISTER(portGIntStatus);
            portXIntEdge = READ_REGISTER(portGIntEdge);
            portXIntPolarity = READ_REGISTER(portGIntPolarity);
            break;

        case 'J':
            portXDir = READ_REGISTER(portJDir);  // Interrupt on inputs only (when pin is low)
            portXData = EmHAL::GetPortInputValue('J');
            portXIntMask = READ_REGISTER(portJIntMask);
            portXIntStatus = READ_REGISTER(portJIntStatus);
            portXIntEdge = READ_REGISTER(portJIntEdge);
            portXIntPolarity = READ_REGISTER(portJIntPolarity);
            break;

        case 'K':
            portXDir = READ_REGISTER(portKDir);  // Interrupt on inputs only (when pin is low)
            portXData = EmHAL::GetPortInputValue('K');
            portXIntMask = READ_REGISTER(portKIntMask);
            portXIntStatus = READ_REGISTER(portKIntStatus);
            portXIntEdge = READ_REGISTER(portKIntEdge);
            portXIntPolarity = READ_REGISTER(portKIntPolarity);
            break;

        case 'M':
            portXDir = READ_REGISTER(portMDir);  // Interrupt on inputs only (when pin is low)
            portXData = EmHAL::GetPortInputValue('M');
            portXIntMask = READ_REGISTER(portMIntMask);
            portXIntStatus = READ_REGISTER(portMIntStatus);
            portXIntEdge = READ_REGISTER(portMIntEdge);
            portXIntPolarity = READ_REGISTER(portMIntPolarity);
            break;

        case 'N':
            portXDir = READ_REGISTER(portNDir);  // Interrupt on inputs only (when pin is low)
            portXData = EmHAL::GetPortInputValue('N');
            portXIntMask = READ_REGISTER(portNIntMask);
            portXIntStatus = READ_REGISTER(portNIntStatus);
            portXIntEdge = READ_REGISTER(portNIntEdge);
            portXIntPolarity = READ_REGISTER(portNIntPolarity);
            break;

        case 'P':
            portXDir = READ_REGISTER(portPDir);  // Interrupt on inputs only (when pin is low)
            portXData = EmHAL::GetPortInputValue('P');
            portXIntMask = READ_REGISTER(portPIntMask);
            portXIntStatus = READ_REGISTER(portPIntStatus);
            portXIntEdge = READ_REGISTER(portPIntEdge);
            portXIntPolarity = READ_REGISTER(portPIntPolarity);
            break;

        case 'R':
            portXDir = READ_REGISTER(portRDir);  // Interrupt on inputs only (when pin is low)
            portXData = EmHAL::GetPortInputValue('R');
            portXIntMask = READ_REGISTER(portRIntMask);
            portXIntStatus = READ_REGISTER(portRIntStatus);
            portXIntEdge = READ_REGISTER(portRIntEdge);
            portXIntPolarity = READ_REGISTER(portRIntPolarity);
            break;

        default:
            EmAssert(false);
            // take care of compiler warnings
            portXDir = 0;
            portXData = 0;
            portXIntMask = 0;
            portXIntStatus = 0;
            portXIntEdge = 0;
            portXIntPolarity = 0;
    }

    // We have a line-level interrupt if:
    //
    //	- line-level interrupts are requested
    //	- the GPIO bit matches the polarity bit

    newBits |= ~portXIntEdge & portXData & ~portXIntPolarity;
    newBits |= ~portXIntEdge & ~portXData & portXIntPolarity;

    // We have an edge interrupt if:
    //
    //	- edge interrupts are requested
    //	- an edge has been recorded
    //
    // Note that we should distinguish between rising and falling edges.
    // For historical reasons, that's not done, and the Palm OS doesn't
    // look for them, so it's OK for now.
    //
    // Edge interrupts on INT[3:0] should not wake up a sleeping device.

#if 0
    uint16 pllControl = READ_REGISTER(pllControl);

    if ((pllControl & hwrSZ328PLLControlMCUDisable) && !(gSession->GetDevice().EdgeHack())) {
        newBits |= portXIntEdge & fPortXEdge[port - 'D'] & portXIntPolarity;
        newBits |= portXIntEdge & 0 & ~portXIntPolarity;
    } else
#endif
    {
        newBits |= portXIntEdge & fPortXEdge[port - 'D'] & portXIntPolarity;
        newBits |= portXIntEdge & 0 & ~portXIntPolarity;
    }

    // Only have interrupts if they're enabled and the pin is configured for input.

    newBits &= portXIntMask & ~portXDir;

    // Merge in the new values and write out the result.

    portXIntStatus = 0;
    portXIntStatus |= newBits;
    portXIntStatus &= portXIntMask;
    uint8 intBit = 0;

    switch (port) {
        case 'D':
            WRITE_REGISTER(portDIntStatus, portXIntStatus);
            intBit = 11;
            break;

        case 'E':
            WRITE_REGISTER(portEIntStatus, portXIntStatus);
            intBit = 10;
            break;

        case 'F':
            WRITE_REGISTER(portFIntStatus, portXIntStatus);
            intBit = 9;
            break;

        case 'G':
            WRITE_REGISTER(portGIntStatus, portXIntStatus);
            intBit = 8;
            break;

        case 'J':
            WRITE_REGISTER(portJIntStatus, portXIntStatus);
            intBit = 6;
            break;

        case 'K':
            WRITE_REGISTER(portKIntStatus, portXIntStatus);
            intBit = 28;
            break;

        case 'M':
            WRITE_REGISTER(portMIntStatus, portXIntStatus);
            intBit = 27;
            break;

        case 'N':
            WRITE_REGISTER(portNIntStatus, portXIntStatus);
            intBit = 26;
            break;

        case 'P':
            WRITE_REGISTER(portPIntStatus, portXIntStatus);
            intBit = 25;
            break;

        case 'R':
            WRITE_REGISTER(portRIntStatus, portXIntStatus);
            intBit = 20;
            break;
    }

    if (intBit != 0 && intBit <= 15) {
        uint16 intPendingLo = READ_REGISTER(intPendingLo);

        if (portXIntStatus > 0)
            intPendingLo |= (1 << intBit);
        else
            intPendingLo &= ~(1 << intBit);

        WRITE_REGISTER(intPendingLo, intPendingLo);
    }

    if (intBit != 0 && intBit > 15) {
        uint16 intPendingHi = READ_REGISTER(intPendingHi);
        intBit -= 16;

        if (portXIntStatus > 0)
            intPendingHi |= (1 << intBit);
        else
            intPendingHi &= ~(1 << intBit);

        WRITE_REGISTER(intPendingHi, intPendingHi);
    }

    // Respond to the new interrupt state.

    EmRegsSZ::UpdateInterrupts();
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::UpdateRTCInterrupts
// ---------------------------------------------------------------------------
// Determine whether to set or clear the RTC bit in the interrupt pending
// register based on the current RTC register values.

void EmRegsSZ::UpdateRTCInterrupts(void) {
    // See if the RTC is enabled.

    Bool rtcEnabled = (READ_REGISTER(rtcControl) & hwrSZ328RTCControlRTCEnable) != 0;

    // See if there are any RTC events that need to trigger an interrupt.

#define BITS_TO_CHECK                                                                 \
    (hwrSZ328RTCIntEnableSec | hwrSZ328RTCIntEnable24Hr | hwrSZ328RTCIntEnableAlarm | \
     hwrSZ328RTCIntEnableMinute | hwrSZ328RTCIntEnableStopWatch)

    uint16 rtcIntStatus = READ_REGISTER(rtcIntStatus);
    uint16 rtcIntEnable = READ_REGISTER(rtcIntEnable);
    uint16 rtcIntPending = rtcIntStatus & rtcIntEnable & BITS_TO_CHECK;

    Bool havePendingEvents = rtcIntPending != 0;

    // If the RTC is enabled and there are pending events, set the interrupt.
    // Otherwise, clear the interrupt.

    uint16 intPendingLo = READ_REGISTER(intPendingLo);
    // Workaround clang bug
    UNUSED_PARAM(intPendingLo);

    if (rtcEnabled && havePendingEvents) {
        intPendingLo |= hwrSZ328IntLoRTC;  // have events, so set interrupt
    } else {
        intPendingLo &= ~hwrSZ328IntLoRTC;  // no events, so clear interrupt
    }

    WRITE_REGISTER(intPendingLo, intPendingLo);

    // Respond to the new interrupt state.

    EmRegsSZ::UpdateInterrupts();
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::UARTStateChanged
// ---------------------------------------------------------------------------

void EmRegsSZ::UARTStateChanged(Bool sendTxData, int uartNum) {
    EmUARTDragonball::State state(EmUARTDragonball::kUART_DragonballVZ);

    EmRegsSZ::MarshalUARTState(state, uartNum);

    fUART[uartNum]->StateChanged(state, sendTxData);

    EmRegsSZ::UnmarshalUARTState(state, uartNum);

    EmRegsSZ::UpdateUARTInterrupts(state, uartNum);
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::UpdateUARTState
// ---------------------------------------------------------------------------

void EmRegsSZ::UpdateUARTState(Bool refreshRxData, int uartNum) {
    EmUARTDragonball::State state(EmUARTDragonball::kUART_DragonballVZ);

    EmRegsSZ::MarshalUARTState(state, uartNum);

    fUART[uartNum]->UpdateState(state, refreshRxData);

    EmRegsSZ::UnmarshalUARTState(state, uartNum);

    EmRegsSZ::UpdateUARTInterrupts(state, uartNum);
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::UpdateUARTInterrupts
// ---------------------------------------------------------------------------

void EmRegsSZ::UpdateUARTInterrupts(const EmUARTDragonball::State& state, int uartNum) {
    // Generate the appropriate interrupts.

    uint16 whichBit = uartNum == 0 ? hwrSZ328IntLoUART : hwrSZ328IntLoUART2;

    if ((state.RX_FULL_ENABLE && state.RX_FIFO_FULL) ||
        (state.RX_HALF_ENABLE && state.RX_FIFO_HALF) || (state.RX_RDY_ENABLE && state.DATA_READY) ||
        (state.TX_EMPTY_ENABLE && state.TX_FIFO_EMPTY) ||
        (state.TX_HALF_ENABLE && state.TX_FIFO_HALF) || (state.TX_AVAIL_ENABLE && state.TX_AVAIL)) {
        // Set the UART interrupt.

        WRITE_REGISTER(intPendingLo, READ_REGISTER(intPendingLo) | whichBit);
    } else {
        // Clear the UART interrupt.

        WRITE_REGISTER(intPendingLo, READ_REGISTER(intPendingLo) & ~whichBit);
    }

    // Respond to the new interrupt state.

    EmRegsSZ::UpdateInterrupts();
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::MarshalUARTState
// ---------------------------------------------------------------------------

void EmRegsSZ::MarshalUARTState(EmUARTDragonball::State& state, int uartNum) {
    uint16 uControl;
    uint16 uBaud;
    uint16 uReceive;
    uint16 uTransmit;
    uint16 uMisc;
    uint16 uLevel;

    if (uartNum == 0) {
        uControl = READ_REGISTER(uControl);
        uBaud = READ_REGISTER(uBaud);
        uReceive = READ_REGISTER(uReceive);
        uTransmit = READ_REGISTER(uTransmit);
        uMisc = READ_REGISTER(uMisc);
        uLevel = READ_REGISTER(uFIFOHMark);
    } else {
        uControl = READ_REGISTER(u2Control);
        uBaud = READ_REGISTER(u2Baud);
        uReceive = READ_REGISTER(u2Receive);
        uTransmit = READ_REGISTER(u2Transmit);
        uMisc = READ_REGISTER(u2Misc);
        uLevel = READ_REGISTER(u2FIFOHMark);
    }

    state.UART_ENABLE = (uControl & hwrSZ328UControlUARTEnable) != 0;
    state.RX_ENABLE = (uControl & hwrSZ328UControlRxEnable) != 0;
    state.TX_ENABLE = (uControl & hwrSZ328UControlTxEnable) != 0;
    state.RX_CLK_CONT = (uControl & hwrSZ328UControlRxClock1xSync) != 0;
    state.PARITY_EN = (uControl & hwrSZ328UControlParityEn) != 0;
    state.ODD_EVEN = (uControl & hwrSZ328UControlParityOdd) != 0;
    state.STOP_BITS = (uControl & hwrSZ328UControlStopBits2) != 0;
    state.CHAR8_7 = (uControl & hwrSZ328UControlDataBits8) != 0;
    //	state.GPIO_DELTA_ENABLE	= (uControl & hwr328UControlGPIODeltaEn) != 0;	// 68328 only
    state.OLD_ENABLE = (uControl & hwrSZ328UControlOldDataEn) != 0;  // 68SZ328 only
    state.CTS_DELTA_ENABLE = (uControl & hwrSZ328UControlCTSDeltaEn) != 0;
    state.RX_FULL_ENABLE = (uControl & hwrSZ328UControlRxFullEn) != 0;
    state.RX_HALF_ENABLE = (uControl & hwrSZ328UControlRxHalfEn) != 0;
    state.RX_RDY_ENABLE = (uControl & hwrSZ328UControlRxRdyEn) != 0;
    state.TX_EMPTY_ENABLE = (uControl & hwrSZ328UControlTxEmptyEn) != 0;
    state.TX_HALF_ENABLE = (uControl & hwrSZ328UControlTxHalfEn) != 0;
    state.TX_AVAIL_ENABLE = (uControl & hwrSZ328UControlTxAvailEn) != 0;

    // Baud control register bits
    // These are all values the user sets; we just look at them.

    //	state.GPIO_DELTA		= (uBaud & hwr328UBaudGPIODelta) != 0;			//
    // 68328 only 	state.GPIO				= (uBaud & hwr328UBaudGPIOData) !=
    // 0;
    //// 68328 only
    //	state.GPIO_DIR			= (uBaud & hwr328UBaudGPIODirOut) != 0;			//
    // 68328 only 	state.GPIO_SRC			= (uBaud & hwrSZ328UBaudGPIOSrcBaudGen) !=
    // 0;
    // // 68328 only
    state.UCLK_DIR = (uBaud & hwrSZ328UBaudUCLKDirOut) != 0;  // 68SZ328 only
    state.BAUD_SRC = (uBaud & hwrSZ328UBaudBaudSrcUCLK) != 0;
    state.DIVIDE = (uBaud & hwrSZ328UBaudDivider) >> hwrSZ328UBaudDivideBitOffset;
    state.PRESCALER = (uBaud & hwrSZ328UBaudPrescaler);

    // Receive register bits
    // These are all input bits; we set them, not the user.

    state.RX_FIFO_FULL = (uReceive & hwrSZ328UReceiveFIFOFull) != 0;
    state.RX_FIFO_HALF = (uReceive & hwrSZ328UReceiveFIFOHalf) != 0;
    state.DATA_READY = (uReceive & hwrSZ328UReceiveDataRdy) != 0;
    state.OLD_DATA = (uReceive & hwrSZ328UReceiveOldData) != 0;  // 68SZ328 only
    state.OVRUN = (uReceive & hwrSZ328UReceiveOverrunErr) != 0;
    state.FRAME_ERROR = (uReceive & hwrSZ328UReceiveFrameErr) != 0;
    state.BREAK = (uReceive & hwrSZ328UReceiveBreakErr) != 0;
    state.PARITY_ERROR = (uReceive & hwrSZ328UReceiveParityErr) != 0;
    state.RX_DATA = (uReceive & hwrSZ328UReceiveData);

    // Transmitter register bits
    // We set everything except TX_DATA; the user sets that
    // value and ONLY that value.

    state.TX_FIFO_EMPTY = (uTransmit & hwrSZ328UTransmitFIFOEmpty) != 0;
    state.TX_FIFO_HALF = (uTransmit & hwrSZ328UTransmitFIFOHalf) != 0;
    state.TX_AVAIL = (uTransmit & hwrSZ328UTransmitTxAvail) != 0;
    state.SEND_BREAK = (uTransmit & hwrSZ328UTransmitSendBreak) != 0;
    state.IGNORE_CTS = (uTransmit & hwrSZ328UTransmitIgnoreCTS) != 0;
    state.BUSY = (uTransmit & hwrSZ328UTransmitBusy) != 0;  // 68SZ328 only
    state.CTS_STATUS = (uTransmit & hwrSZ328UTransmitCTSStatus) != 0;
    state.CTS_DELTA = (uTransmit & hwrSZ328UTransmitCTSDelta) != 0;
    state.TX_DATA = (uTransmit & hwrSZ328UTransmitData);

    // Misc register bits
    // These are all values the user sets; we just look at them.

    state.BAUD_TEST = (uMisc & hwrSZ328UMiscBaudTest) != 0;  // 68SZ328 only
    state.CLK_SRC = (uMisc & hwrSZ328UMiscClkSrcUCLK) != 0;
    state.FORCE_PERR = (uMisc & hwrSZ328UMiscForceParityErr) != 0;
    state.LOOP = (uMisc & hwrSZ328UMiscLoopback) != 0;
    state.BAUD_RESET = (uMisc & hwrSZ328UMiscBaudReset) != 0;  // 68SZ328 only
    state.IR_TEST = (uMisc & hwrSZ328UMiscIRTestEn) != 0;      // 68SZ328 only
    state.RTS_CONT = (uMisc & hwrSZ328UMiscRTSThruFIFO) != 0;
    state.RTS = (uMisc & hwrSZ328UMiscRTSOut) != 0;
    state.IRDA_ENABLE = (uMisc & hwrSZ328UMiscIRDAEn) != 0;
    state.IRDA_LOOP = (uMisc & hwrSZ328UMiscLoopIRDA) != 0;
    state.RX_POL = (uMisc & hwrSZ328UMiscRXPolarityInv) != 0;  // 68SZ328 only
    state.TX_POL = (uMisc & hwrSZ328UMiscTXPolarityInv) != 0;  // 68SZ328 only

    // Level Marker Interrupt

    state.TXFIFO_LEVEL_MARKER = ((uLevel >> 8) & 0x0F);  // 68SZ328 only
    state.RXFIFO_LEVEL_MARKER = ((uLevel >> 0) & 0x0F);  // 68SZ328 only
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::UnmarshalUARTState
// ---------------------------------------------------------------------------

void EmRegsSZ::UnmarshalUARTState(const EmUARTDragonball::State& state, int uartNum) {
    uint16 uControl = 0;
    uint16 uBaud = 0;
    uint16 uReceive = 0;
    uint16 uTransmit = 0;
    uint16 uMisc = 0;
    uint16 uLevel = 0;

    if (state.UART_ENABLE) uControl |= hwrSZ328UControlUARTEnable;
    if (state.RX_ENABLE) uControl |= hwrSZ328UControlRxEnable;
    if (state.TX_ENABLE) uControl |= hwrSZ328UControlTxEnable;
    if (state.RX_CLK_CONT) uControl |= hwrSZ328UControlRxClock1xSync;
    if (state.PARITY_EN) uControl |= hwrSZ328UControlParityEn;
    if (state.ODD_EVEN) uControl |= hwrSZ328UControlParityOdd;
    if (state.STOP_BITS) uControl |= hwrSZ328UControlStopBits2;
    if (state.CHAR8_7) uControl |= hwrSZ328UControlDataBits8;
    //	if (state.GPIO_DELTA_ENABLE)uControl |= hwr328UControlGPIODeltaEn;	// 68328 only
    if (state.OLD_ENABLE) uControl |= hwrSZ328UControlOldDataEn;  // 68SZ328 only
    if (state.CTS_DELTA_ENABLE) uControl |= hwrSZ328UControlCTSDeltaEn;
    if (state.RX_FULL_ENABLE) uControl |= hwrSZ328UControlRxFullEn;
    if (state.RX_HALF_ENABLE) uControl |= hwrSZ328UControlRxHalfEn;
    if (state.RX_RDY_ENABLE) uControl |= hwrSZ328UControlRxRdyEn;
    if (state.TX_EMPTY_ENABLE) uControl |= hwrSZ328UControlTxEmptyEn;
    if (state.TX_HALF_ENABLE) uControl |= hwrSZ328UControlTxHalfEn;
    if (state.TX_AVAIL_ENABLE) uControl |= hwrSZ328UControlTxAvailEn;

    // Baud control register bits
    // These are all values the user sets; we just look at them.

    //	if (state.GPIO_DELTA)		uBaud |= hwr328UBaudGPIODelta;		// 68328 only
    //	if (state.GPIO)				uBaud |= hwr328UBaudGPIOData;		// 68328
    // only 	if (state.GPIO_DIR)			uBaud |= hwr328UBaudGPIODirOut;		//
    // 68328
    // only 	if (state.GPIO_SRC)			uBaud |= hwr328UBaudGPIOSrcBaudGen;	//
    // 68328 only
    if (state.UCLK_DIR) uBaud |= hwrSZ328UBaudUCLKDirOut;  // 68SZ328 only
    if (state.BAUD_SRC) uBaud |= hwrSZ328UBaudBaudSrcUCLK;

    uBaud |= (state.DIVIDE << hwrSZ328UBaudDivideBitOffset) & hwrSZ328UBaudDivider;
    uBaud |= (state.PRESCALER) & hwrSZ328UBaudPrescaler;

    // Receive register bits
    // These are all input bits; we set them, not the user.

    if (state.RX_FIFO_FULL) uReceive |= hwrSZ328UReceiveFIFOFull;
    if (state.RX_FIFO_HALF) uReceive |= hwrSZ328UReceiveFIFOHalf;
    if (state.DATA_READY) uReceive |= hwrSZ328UReceiveDataRdy;
    if (state.OLD_DATA) uReceive |= hwrSZ328UReceiveOldData;  // 68SZ328 only
    if (state.OVRUN) uReceive |= hwrSZ328UReceiveOverrunErr;
    if (state.FRAME_ERROR) uReceive |= hwrSZ328UReceiveFrameErr;
    if (state.BREAK) uReceive |= hwrSZ328UReceiveBreakErr;
    if (state.PARITY_ERROR) uReceive |= hwrSZ328UReceiveParityErr;

    uReceive |= (state.RX_DATA) & hwrSZ328UReceiveData;

    // Transmitter register bits
    // We set everything except TX_DATA; the user sets that
    // value and ONLY that value.

    if (state.TX_FIFO_EMPTY) uTransmit |= hwrSZ328UTransmitFIFOEmpty;
    if (state.TX_FIFO_HALF) uTransmit |= hwrSZ328UTransmitFIFOHalf;
    if (state.TX_AVAIL) uTransmit |= hwrSZ328UTransmitTxAvail;
    if (state.SEND_BREAK) uTransmit |= hwrSZ328UTransmitSendBreak;
    if (state.IGNORE_CTS) uTransmit |= hwrSZ328UTransmitIgnoreCTS;
    if (state.BUSY) uTransmit |= hwrSZ328UTransmitBusy;  // 68SZ328 only
    if (state.CTS_STATUS) uTransmit |= hwrSZ328UTransmitCTSStatus;
    if (state.CTS_DELTA) uTransmit |= hwrSZ328UTransmitCTSDelta;

    uTransmit |= (state.TX_DATA) & hwrSZ328UTransmitData;

    // Misc register bits
    // These are all values the user sets; we just look at them.

    if (state.BAUD_TEST) uMisc |= hwrSZ328UMiscBaudTest;  // 68SZ328 only
    if (state.CLK_SRC) uMisc |= hwrSZ328UMiscClkSrcUCLK;
    if (state.FORCE_PERR) uMisc |= hwrSZ328UMiscForceParityErr;
    if (state.LOOP) uMisc |= hwrSZ328UMiscLoopback;
    if (state.BAUD_RESET) uMisc |= hwrSZ328UMiscBaudReset;  // 68SZ328 only
    if (state.IR_TEST) uMisc |= hwrSZ328UMiscIRTestEn;      // 68SZ328 only
    if (state.RTS_CONT) uMisc |= hwrSZ328UMiscRTSThruFIFO;
    if (state.RTS) uMisc |= hwrSZ328UMiscRTSOut;
    if (state.IRDA_ENABLE) uMisc |= hwrSZ328UMiscIRDAEn;
    if (state.IRDA_LOOP) uMisc |= hwrSZ328UMiscLoopIRDA;
    if (state.RX_POL) uMisc |= hwrSZ328UMiscRXPolarityInv;  // 68SZ328 only
    if (state.TX_POL) uMisc |= hwrSZ328UMiscTXPolarityInv;  // 68SZ328 only

    // Level Marker Interrupt

    uLevel |= (state.TXFIFO_LEVEL_MARKER) << 8;
    uLevel |= (state.RXFIFO_LEVEL_MARKER) << 0;

    if (uartNum == 0) {
        WRITE_REGISTER(uControl, uControl);
        WRITE_REGISTER(uBaud, uBaud);
        WRITE_REGISTER(uReceive, uReceive);
        WRITE_REGISTER(uTransmit, uTransmit);
        WRITE_REGISTER(uMisc, uMisc);
        WRITE_REGISTER(uFIFOHMark, uLevel);
    } else {
        WRITE_REGISTER(u2Control, uControl);
        WRITE_REGISTER(u2Baud, uBaud);
        WRITE_REGISTER(u2Receive, uReceive);
        WRITE_REGISTER(u2Transmit, uTransmit);
        WRITE_REGISTER(u2Misc, uMisc);
        WRITE_REGISTER(u2FIFOHMark, uLevel);
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::GetPortFromAddress
// ---------------------------------------------------------------------------
// Given an address, return a value indicating what port it is associated with.

int EmRegsSZ::GetPortFromAddress(emuptr address) {
    const long MASK = 0x00000FF8;

    switch (address & MASK) {
        case 0x0400:
            return 'A';
        case 0x0408:
            return 'B';
        case 0x0410:
            return 'C';
        case 0x0418:
            return 'D';
        case 0x0420:
            return 'E';
        case 0x0428:
            return 'F';
        case 0x0430:
            return 'G';
        case 0x0438:
            return 'J';
        case 0x0440:
            return 'K';
        case 0x0448:
            return 'M';
        case 0x0450:
            return 'N';
        case 0x0458:
            return 'P';
        case 0x0460:
            return 'R';
        default:
            EmAssert(false);
    }

    return 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsSZ::GetAddressFromPort
// ---------------------------------------------------------------------------
// Given an address, return a value indicating what port it is associated with.

emuptr EmRegsSZ::GetAddressFromPort(int port) {
    const long DragonballBase = 0xFFFFF000;
    emuptr address;

    switch (port) {
        case 'A':
            address = 0x0400;
            break;
        case 'B':
            address = 0x0408;
            break;
        case 'C':
            address = 0x0410;
            break;
        case 'D':
            address = 0x0418;
            break;
        case 'E':
            address = 0x0420;
            break;
        case 'F':
            address = 0x0428;
            break;
        case 'G':
            address = 0x0430;
            break;
        case 'J':
            address = 0x0438;
            break;
        case 'K':
            address = 0x0440;
            break;
        case 'M':
            address = 0x0448;
            break;
        case 'N':
            address = 0x0450;
            break;
        case 'P':
            address = 0x0458;
            break;
        case 'R':
            address = 0x0460;
            break;
        default:
            EmAssert(false);
            address = 0x0000;  // Silence compiler warning.
            break;
    }

    return (address + DragonballBase);
}

bool EmRegsSZ::CopyLCDFrame(Frame& frame, bool fullRefresh) {
    const emuptr startAddress = READ_REGISTER(lcdStartAddr) & ~1;
    const uint16 screenWidth = (READ_REGISTER(lcdScreenSize) >> 9) * 8;
    const uint16 screenHeight = READ_REGISTER(lcdScreenSize) & 0x01ff;
    const uint16 virtualPageWidth = (READ_REGISTER(lcdPageWidth) & 0x03ff) * 2;
    const uint8 panningOffset = READ_REGISTER(lcdPanningOffset) & 0x0f;
    const uint16_t lcdControl1 = READ_REGISTER(lcdPanelControl1);
    const uint8 bpp = 1 << ((lcdControl1 >> 9) & 0x07);

    if (screenWidth % 8 != 0) {
        return false;
    }

    frame.bpp = 24;
    frame.lineWidth = screenWidth;
    frame.lines = screenHeight;
    frame.margin = 0;
    frame.bytesPerLine = screenWidth * 4;
    frame.scaleX = frame.scaleY = 1;
    frame.lineWidth = screenWidth;

    frame.UpdateDirtyLines(gSystemState, startAddress, virtualPageWidth, fullRefresh);
    if (!frame.hasChanges) return true;

    uint32* dest =
        reinterpret_cast<uint32*>(frame.GetBuffer()) + frame.firstDirtyLine * frame.lineWidth;

    switch (bpp) {
        case 4: {
            UpdatePalette();
            uint8* base = EmMemGetRealAddress(startAddress);
            Nibbler<4, true> nibbler;

            for (uint32 y = frame.firstDirtyLine; y <= frame.lastDirtyLine; y++) {
                nibbler.reset(base + y * virtualPageWidth, panningOffset);

                for (uint32 x = 0; x < screenWidth; x++) {
                    *(dest++) = palette[nibbler.nibble()];
                }
            }

            return true;
        }

        case 8: {
            UpdatePalette();
            uint8* base = EmMemGetRealAddress(startAddress);

            for (uint32 y = frame.firstDirtyLine; y <= frame.lastDirtyLine; y++) {
                uint8* src = base + y * virtualPageWidth + (panningOffset >> 3);

                for (uint32 x = 0; x < screenWidth; x++) {
                    *(dest++) = palette[EmMemDoGet8(src++)];
                }
            }

            return true;
        }

        case 16: {
            uint16* base = reinterpret_cast<uint16*>(EmMemGetRealAddress(startAddress & ~1));

            for (uint32 y = frame.firstDirtyLine; y <= frame.lastDirtyLine; y++) {
                uint16* src = base + y * (virtualPageWidth >> 1) + (panningOffset >> 4);

                for (uint32 x = 0; x < screenWidth; x++) {
                    *(dest++) = convertColor_16bit(EmMemDoGet16(src++));
                }
            }

            return true;
        }
    }

    return false;
}

uint16 EmRegsSZ::GetLCD2bitMapping() { return 0x3210; }

uint16 EmRegsSZ::GetADCValueU() { return 0; }

void EmRegsSZ::MarkScreen() {
    if (!markScreen) return;

    if (!esram->IsFramebuffer()) markScreenWith(MetaMemory::MarkScreen, f68SZ328Regs);

    markScreen = false;
}

void EmRegsSZ::UnmarkScreen() {
    if (!esram->IsFramebuffer()) markScreenWith(MetaMemory::UnmarkScreen, f68SZ328Regs);

    markScreen = true;
}

uint32 EmRegsSZ::CyclesToNextInterrupt(uint64 systemCycles) {
    this->systemCycles = systemCycles;

    if (systemCycles >= nextTimerEventAfterCycle) return 1;
    return std::min(nextTimerEventAfterCycle - systemCycles, (uint64)0xffffffff);
}

int32 EmRegsSZ::GetSysClk() {
    // WARNING! The terminology is off here. "SystemClockFrequency" in Cloudpilot
    // really refers to the CPU clock. However, for the SZ the CPU clock is equal
    // to the DMA clock, and this goes through an additional divider to generate
    // the actual system clock which drives the timer.

    const uint32 pllcr = READ_REGISTER(pllControl);
    const uint32 sysclkDiv = (pllcr & (1 << 10)) ? 1 : 2 << ((pllcr >> 8) & 0x03);

    return GetSystemClockFrequency() / sysclkDiv;
}

void EmRegsSZ::UpdateTimers() {
    nextTimerEventAfterCycle = ~0;
    if (GetAsleep()) return;

    double clocksPerSecond = gSession->GetClocksPerSecond();
    int32 systemClockFrequency = GetSysClk();

    double timer1TicksPerSecond = TimerTicksPerSecond(
        READ_REGISTER(tmr1Control), READ_REGISTER(tmr1Prescaler), systemClockFrequency);
    double timer2TicksPerSecond = TimerTicksPerSecond(
        READ_REGISTER(tmr2Control), READ_REGISTER(tmr2Prescaler), systemClockFrequency);

    if (((READ_REGISTER(tmr1Control) & hwrSZ328TmrControlEnable) != 0) &&
        timer1TicksPerSecond > 0 && clocksPerSecond > 0) {
        uint32 ticks = ((double)systemCycles - tmr1LastProcessedSystemCycles) / clocksPerSecond *
                       timer1TicksPerSecond;

        tmr1LastProcessedSystemCycles += (double)ticks / timer1TicksPerSecond * clocksPerSecond;

        uint32 updatedCounter = READ_REGISTER(tmr1Counter) + ticks;
        WRITE_REGISTER(tmr1Counter, static_cast<uint16>(updatedCounter));

        uint16 tcmp = READ_REGISTER(tmr1Compare);

        if (updatedCounter >= tcmp) {
            // Flag the occurrence of the successful comparison.
            WRITE_REGISTER(tmr1Status, READ_REGISTER(tmr1Status) | hwrSZ328TmrStatusCompare);

            // If the Free Run/Restart flag is not set, clear the counter.
            if ((READ_REGISTER(tmr1Control) & hwrSZ328TmrControlFreeRun) == 0) {
                WRITE_REGISTER(tmr1Counter, static_cast<uint16>(updatedCounter - tcmp));
            }

            // If the timer interrupt is enabled, post an interrupt.
            if ((READ_REGISTER(tmr1Control) & hwrSZ328TmrControlEnInterrupt) != 0) {
                WRITE_REGISTER(intPendingLo, READ_REGISTER(intPendingLo) | hwrSZ328IntLoTimer);
                EmRegsSZ::UpdateInterrupts();
            }
        }

        if (!(READ_REGISTER(intMaskLo) & hwrSZ328IntLoTimer) &&
            (READ_REGISTER(tmr1Control) & hwrSZ328TmrControlEnInterrupt)) {
            uint16 tcn = READ_REGISTER(tmr1Counter);
            uint16 delta = tcmp - tcn;
            uint64 cycles = ceil((double)delta / timer1TicksPerSecond * clocksPerSecond);

            if ((uint32)(((double)(cycles + systemCycles) - tmr1LastProcessedSystemCycles) /
                         clocksPerSecond * timer1TicksPerSecond) < delta)

                cycles += clocksPerSecond / timer1TicksPerSecond;

            if (cycles < nextTimerEventAfterCycle - systemCycles)
                nextTimerEventAfterCycle = systemCycles + cycles;
        }

    } else {
        tmr1LastProcessedSystemCycles = systemCycles;
    }

    if (((READ_REGISTER(tmr2Control) & hwrSZ328TmrControlEnable) != 0) &&
        timer2TicksPerSecond > 0) {
        uint32 ticks = ((double)systemCycles - tmr2LastProcessedSystemCycles) / clocksPerSecond *
                       timer2TicksPerSecond;

        tmr2LastProcessedSystemCycles += (double)ticks / timer2TicksPerSecond * clocksPerSecond;

        uint32 updatedCounter = READ_REGISTER(tmr2Counter) + ticks;
        WRITE_REGISTER(tmr2Counter, static_cast<uint16>(updatedCounter));

        uint16 tcmp = READ_REGISTER(tmr2Compare);

        if (updatedCounter >= tcmp) {
            // Flag the occurrence of the successful comparison.
            WRITE_REGISTER(tmr2Status, READ_REGISTER(tmr2Status) | hwrSZ328TmrStatusCompare);

            // If the Free Run/Restart flag is not set, clear the counter.
            if ((READ_REGISTER(tmr2Control) & hwrSZ328TmrControlFreeRun) == 0) {
                WRITE_REGISTER(tmr2Counter, static_cast<uint16>(updatedCounter - tcmp));
            }

            // If the timer interrupt is enabled, post an interrupt.
            if ((READ_REGISTER(tmr2Control) & hwrSZ328TmrControlEnInterrupt) != 0) {
                WRITE_REGISTER(intPendingLo, READ_REGISTER(intPendingLo) | hwrSZ328IntLoTimer2);
                EmRegsSZ::UpdateInterrupts();
            }
        }

        if (!(READ_REGISTER(intMaskLo) & hwrSZ328IntLoTimer2) &&
            (READ_REGISTER(tmr2Control) & hwrSZ328TmrControlEnInterrupt)) {
            uint16 tcn = READ_REGISTER(tmr2Counter);
            uint16 delta = tcmp - tcn;
            uint64 cycles = ceil((double)delta / timer2TicksPerSecond * clocksPerSecond);

            if ((uint32)(((double)(cycles + systemCycles) - tmr2LastProcessedSystemCycles) /
                         clocksPerSecond * timer2TicksPerSecond) < delta)

                cycles += clocksPerSecond / timer2TicksPerSecond;

            if (cycles < nextTimerEventAfterCycle - systemCycles)
                nextTimerEventAfterCycle = systemCycles + cycles;
        }
    } else {
        tmr2LastProcessedSystemCycles = systemCycles;
    }
}

void EmRegsSZ::HandleDayRollover() {
    if ((READ_REGISTER(rtcIntEnable) & hwrSZ328RTCIntEnable24Hr) != 0 &&
        (READ_REGISTER(rtcIntStatus) & hwrSZ328RTCIntStatus24Hr) == 0) {
        WRITE_REGISTER(rtcIntStatus, READ_REGISTER(rtcIntStatus) | hwrSZ328RTCIntStatus24Hr);
        UpdateRTCInterrupts();
    }
}

bool EmRegsSZ::EnableRAM() { return (READ_REGISTER(sdramControlE) >> 28) == 0x8; }

void EmRegsSZ::sdramControlEWrite(emuptr address, int size, uint32 value) {
    EmRegsSZ::StdWrite(address, size, value);

    gSession->ScheduleResetBanks();
}

void EmRegsSZ::pwmc1Write(emuptr address, int size, uint32 value) {
    EmRegsSZ::StdWrite(address, size, value);

    if (pwmActive && !(value & 0x10)) {
        pwmActive = false;
        DispatchPwmChange();
    }
}

void EmRegsSZ::pwms1Write(emuptr address, int size, uint32 value) {
    EmRegsSZ::StdWrite(address, size, value);

    if (READ_REGISTER(pwmControl) & 0x10) {
        pwmActive = true;
        DispatchPwmChange();
    }
}

void EmRegsSZ::pwmp1Write(emuptr address, int size, uint32 value) {
    EmRegsSZ::StdWrite(address, size, value);

    if (pwmActive) DispatchPwmChange();
}

void EmRegsSZ::csgRegWrite(emuptr address, int size, uint32 value) {
    EmRegsSZ::StdWrite(address, size, value);

    UpdateEsramLocation();
    gSession->ScheduleResetBanks();
}

void EmRegsSZ::lcdRegisterWrite(emuptr address, int size, uint32 value) {
    switch (address) {
        case db_addressof(lcdScreenSize):
        case db_addressof(lcdPageWidth):
        case db_addressof(lcdStartAddr):
            UnmarkScreen();
            break;
    }

    EmRegsSZ::StdWrite(address, size, value);

    gSystemState.MarkScreenDirty();
}

void EmRegsSZ::lcdStartAddrWrite(emuptr address, int size, uint32 value) {
    lcdRegisterWrite(address, size, value);

    UpdateFramebufferLocation();
}

void EmRegsSZ::clutWrite(emuptr address, int size, uint32 value) {
    EmRegsSZ::lcdRegisterWrite(address, size, value);

    clutDirty = true;
}

void EmRegsSZ::adcControlWrite(emuptr address, int size, uint32 value) {
    StdWrite(address, size, value);

    uint32 istatr = READ_REGISTER(adcInterruptErrorStatus);
    uint32 adctrl = READ_REGISTER(adcControl);

    if ((adctrl & 0x01) == 0) padcFifoReadIndex = 0;

    istatr &= ~0x80;
    istatr |= (adctrl & 0x01) << 7;

    istatr &= ~0x01;
    istatr |= (adctrl >> 1) & 0x01;

    WRITE_REGISTER(adcInterruptErrorStatus, istatr);
}

void EmRegsSZ::DispatchPwmChange() {
    uint16 pwmc1 = READ_REGISTER(pwmControl);
    uint8 pwms1 = READ_REGISTER(pwmSampleLo);
    uint8 pwmp1 = READ_REGISTER(pwmPeriod);

    if (!pwmActive) {
        EmHAL::onPwmChange.Dispatch(-1, -1);

        return;
    }

    uint8 prescaler = (pwmc1 >> 8) & 0x7f;
    uint8 clksel = pwmc1 & 0x03;
    uint32 baseFreq = (pwmc1 & 0x8000) ? 32768 : GetSysClk();

    double freq = static_cast<double>(baseFreq) / (prescaler + 1) / (2 << clksel) /
                  min(256u, static_cast<uint32>(pwmp1) + 2);

    double dutyCycle = static_cast<double>(pwms1) / pwmp1;

    if (freq <= 20000) EmHAL::onPwmChange.Dispatch(freq, dutyCycle);
}

bool EmRegsSZNoScreen::CopyLCDFrame(Frame& frame, bool fullRefresh) {
    return EmHALHandler::CopyLCDFrame(frame, fullRefresh);
}

uint16 EmRegsSZNoScreen::GetLCD2bitMapping() { return EmHALHandler::GetLCD2bitMapping(); }

void EmRegsSZNoScreen::MarkScreen() { return; }

void EmRegsSZNoScreen::UnmarkScreen() { return; }
