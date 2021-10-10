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

#include "EmRegsVZ.h"

#include <algorithm>
#include <cmath>

#include "Byteswapping.h"  // Canonical
#include "ChunkHelper.h"
#include "EmBankSRAM.h"
#include "EmCommon.h"
#include "EmDevice.h"
#include "EmHAL.h"     // EmHAL
#include "EmMemory.h"  // gMemAccessFlags, EmMem_memcpy
#include "EmRegsVZPrv.h"
#include "EmSPISlave.h"  // DoExchange
#include "EmSession.h"   // gSession
#include "EmSystemState.h"
#include "Frame.h"
#include "Logging.h"  // LogAppendMsg
#include "MetaMemory.h"
#include "Miscellaneous.h"  // GetHostTime
#include "Savestate.h"
#include "SavestateLoader.h"
#include "SavestateProbe.h"
#include "SavestateStructures.h"
#include "UAE.h"  // regs, SPCFLAG_INT

// clang-format off
#include "PalmPack.h"
// clang-format on

#define NON_PORTABLE
#include "HwrMiscFlags.h"  // hwrMiscFlagID1

// Some platform-specific -- yet fairly portable -- defines.
#define hwrVZPortDKbdCol0 0x01  // (H) Keyboard Column 0		(aka INT0)
#define hwrVZPortDKbdCol1 0x02  // (H) Keyboard Column 1		(aka INT1)
#define hwrVZPortDKbdCol2 0x04  // (H) Keyboard Column 2		(aka INT2)
#define hwrVZPortDKbdCol3 0x08  // (H) Keyboard Column 3		(aka INT3)
#define hwrVZPortDKeyBits 0x0F  // All Keyboard Columns

#undef NON_PORTABLE

#include "PalmPackPop.h"

static const uint16 UPSIZMask = 0x1800;  // Mask to get the unprotected memory size from csDSelect.
static const uint16 DRAMMask = 0x0200;
static const uint16 SIZMask = 0x000E;  // Mask to get the memory size from csASelect.
static const uint16 ENMask = 0x0001;   // Mask to get the enable bit from csASelect.

static const int UPSIZShift = 11;
static const int SIZShift = 1;

static const uint16 DUPS2Mask = 0x0010;
static const uint16 DSIZ3Mask = 0x0040;
static const uint16 EUPENMask = 0x4000;

static const int DUPS2Shift = 4;

static const int kBaseAddressShift = 13;  // Shift to get base address from CSGBx register value

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

    template <class T>
    void markScreenWith(T marker, HwrM68VZ328Type& f68VZ328Regs) {
        emuptr firstLineAddr = READ_REGISTER(lcdStartAddr);
        emuptr lastLineAddr =
            firstLineAddr + (READ_REGISTER(lcdScreenHeight) + 1) * READ_REGISTER(lcdPageWidth) * 2;

        if (EmMemGetBankPtr(firstLineAddr) == nullptr) return;

        marker(firstLineAddr, lastLineAddr);
    }
}  // namespace

// Values used to initialize the DragonBallVZ registers.

static const HwrM68VZ328Type kInitial68VZ328RegisterValues = {
    0x1C,  // UInt8	scr;						// $000: System Control
           // Register

    {0},  // UInt8
          // ___filler0[0x003-0x001];

    0x00,    // UInt8	pcr;						// $003: Peripheral Control
             // Register
    0x56,    // UInt8	chipID;						// $004: Chip ID Register
    0x01,    // UInt8	maskID;						// $005: Mask ID Register
    0x0000,  // UInt16	swID;						// $006: Software ID
             // Register
    0x1FFF,  // UInt16	ioDriveControl;				// $008: I/O Drive Control Register

    {0},  // UInt8
          // ___filler1[0x100-0x00A];

    0x0000,  // UInt16	csAGroupBase;				// $100: Chip Select Group A Base
             // Register
    0x0000,  // UInt16	csBGroupBase;				// $102: Chip Select Group B Base
             // Register
    0x0000,  // UInt16	csCGroupBase;				// $104: Chip Select Group C Base
             // Register
    0x0000,  // UInt16	csDGroupBase;				// $106: Chip Select Group D Base
             // Register

    0x0000,  // UInt16	csUGroupBase;				// $108: Chip Select Upper Group
             // Base Register

    0x0000,  // UInt16	csControl1;					// $10A: Chip Select Control
             // Register
    0x0000,  // UInt16	csControl2;					// $10C: Chip Select Control
             // Register

    {0},  // UInt8
          // ___filler2[0x110-0x10c];

    0x00B0,  // UInt16	csASelect;					// $110: Group A Chip Select
             // Register
    0x0000,  // UInt16	csBSelect;					// $112: Group B Chip Select
             // Register
    0x0000,  // UInt16	csCSelect;					// $114: Group C Chip Select
             // Register
    0x0200,  // UInt16	csDSelect;					// $116: Group D Chip Select
             // Register

    0x0060,  // UInt16	emuCS;						// $118: EMU Chip Select
             // Register

    {0},  // UInt8
          // ___filler3[0x200-0x11A];

    0x0000,  // UInt16	csControl3;					// $150: Chip Select Control
             // Register

    {0},  // UInt8
          // ___filler3[0x200-0x11A];

    0x24B3,  // UInt16	pllControl;					// $200: PLL Control
             // Register
    0x0123,  // UInt16	pllFreqSel;					// $202: CGM Frequency
             // Select Register

    {0},  // UInt8
          // ___filler4[0x207-0x204];

    0x1F,  // UInt8	pwrControl;					// $207: Power Control
           // Register

    {0},  // UInt8
          // ___filler5[0x300-0x208];

    0x00,    // UInt8	intVector;					// $300: Interrupt Vector
             // Register
    0x00,    // UInt8									___filler6;
    0x0000,  // UInt16	intControl;					// $302: Interrupt Control
             // Register
    0x00FF,  // UInt16	intMaskHi;					// $304: Interrupt Mask
             // Register/HIGH word
    0xFFFF,  // UInt16	intMaskLo;					// $306: Interrupt Mask
             // Register/LOW word
    {0},     // UInt8
             // ___filler7[0x30c-0x308];
    0x0000,  // UInt16	intStatusHi;				// $30C: Interrupt Status
             // Register/HIGH word
    0x0000,  // UInt16	intStatusLo;				// $30E: Interrupt Status
             // Register/LOW word
    0x0000,  // UInt16	intPendingHi;				// $310: Interrupt Pending
             // Register/HIGH word
    0x0000,  // UInt16	intPendingLo;				// $312: Interrupt Pending
             // Register/LOW word
    0x6533,  // UInt16	intLevelControl;			// $314: Interrupt Level Control
             // Register

    {0},  // UInt8
          // ___filler4a[0x400-0x316];

    0x00,  // UInt8	portADir;					// $400: Port A Direction
           // Register
    0x00,  // UInt8	portAData;					// $401: Port A Data
           // Register
    0xFF,  // UInt8	portAPullupEn;				// $402: Port A Pullup Enable

    {0},  // UInt8 ___filler8[5];

    0x00,  // UInt8	portBDir;					// $408: Port B Direction
           // Register
    0x00,  // UInt8	portBData;					// $409: Port B Data
           // Register
    0xFF,  // UInt8	portBPullupEn;				// $40A: Port B Pullup Enable
    0xFF,  // UInt8	portBSelect;				// $40B: Port B Select Register

    {0},  // UInt8 ___filler9[4];

    0x00,  // UInt8	portCDir;					// $410: Port C Direction
           // Register
    0x00,  // UInt8	portCData;					// $411: Port C Data
           // Register
    0xFF,  // UInt8	portCPulldnEn;				// $412: Port C Pulldown Enable
    0xFF,  // UInt8	portCSelect;				// $413: Port C Select Register

    {0},  // UInt8 ___filler10[4];

    0x00,  // UInt8	portDDir;					// $418: Port D Direction
           // Register
    0x00,  // UInt8	portDData;					// $419: Port D Data
           // Register
    0xFF,  // UInt8	portDPullupEn;				// $41A: Port D Pull-up Enable
    0xFF,  // UInt8	portDSelect;				// $41B: Port D Select Register
    0x00,  // UInt8	portDPolarity;				// $41C: Port D Polarity Register
    0x00,  // UInt8	portDIntReqEn;				// $41D: Port D Interrupt Request
           // Enable
    0x00,  // UInt8	portDKbdIntEn;				// $41E: Port D Keyboard Interrupt
           // Enable
    0x00,  // UInt8	portDIntEdge;				// $41F: Port D IRQ Edge Register

    0x00,  // UInt8	portEDir;					// $420: Port E Direction
           // Register
    0x00,  // UInt8	portEData;					// $421: Port E Data
           // Register
    0xFF,  // UInt8	portEPullupEn;				// $422: Port E Pull-up Enable
    0xFF,  // UInt8	portESelect;				// $423: Port E Select Register

    {0},  // UInt8 ___filler14[4];

    0x00,  // UInt8	portFDir;					// $428: Port F Direction
           // Register
    0x00,  // UInt8	portFData;					// $429: Port F Data
           // Register
    0xFF,  // UInt8	portFPullupdnEn;			// $42A: Port F Pull-up/down Enable
    0x8F,  // UInt8	portFSelect;				// $42B: Port F Select Register

    {0},  // UInt8 ___filler16[4];

    0x00,  // UInt8	portGDir;					// $430: Port G Direction
           // Register
    0x00,  // UInt8	portGData;					// $431: Port G Data
           // Register
    0x3D,  // UInt8	portGPullupEn;				// $432: Port G Pull-up Enable
    0x08,  // UInt8	portGSelect;				// $433: Port G Select Register

    {0},  // UInt8
          // ___filler17[0x438-0x434];

    0x00,  // UInt8	portJDir;					// $438: Port J Direction
           // Register
    0x00,  // UInt8	portJData;					// $439: Port J Data
           // Register
    0xFF,  // UInt8	portJPullupEn;				// $43A: Port J Pull-up Enable
    0xEF,  // UInt8	portJSelect;				// $43B: Port J Select Register

    {0},  // UInt8
          // ___filler18[0x440-0x43C];

    0x00,  // UInt8	portKDir;					// $440: Port K Direction
           // Register
    0x00,  // UInt8	portKData;					// $441: Port K Data
           // Register
    0xFF,  // UInt8	portKPullupdnEn;			// $442: Port K Pull-up/down Enable
    0xFF,  // UInt8	portKSelect;				// $443: Port K Select Register

    {0},  // UInt8
          // ___filler19[0x448-0x444];

    0x00,  // UInt8	portMDir;					// $448: Port M Direction
           // Register
    0x00,  // UInt8	portMData;					// $449: Port M Data
           // Register
    0x3F,  // UInt8	portMPullupdnEn;			// $44A: Port M Pull-up/down Enable
    0x3F,  // UInt8	portMSelect;				// $44B: Port M Select Register

    {0},  // UInt8
          // ___filler20[0x500-0x44C];

    0x0020,  // UInt16	pwmControl;					// $500: PWM 1 Control
             // Register  (PWM 1 is the same as the single PWM in EZ)
    0x00,    // UInt8	pwmSampleHi;    			// $502: PWM 1 Sample - high byte
    0x00,    // UInt8	pwmSampleLo;				// $503: PWM 1 Sample - low byte
    0xFE,    // UInt8	pwmPeriod;					// $504: PWM 1 Period
    0x00,    // UInt8	pwmCounter;					// $505: PWM 1 Counter

    {0},  // UInt8
          // ___filler22[0x510-0x506];

    0x0000,  // UInt16	pwm2Control;				// $510: PWM 2 Control Register
    0x0000,  // UInt16	pwm2Period;					// $512: PWM 2 Period
    0x0000,  // UInt16	pwm2Width;					// $514: PWM 2 Width
    0x0000,  // UInt16	pwm2Counter;				// $516: PWM 2 Counter

    {0},  // UInt8
          // ___filler23[0x600-0x518];

    0x0000,  // UInt16	tmr1Control;				// $600: Timer 1 Control Register
    0x0000,  // UInt16	tmr1Prescaler;				// $602: Timer 1 Prescaler Register
    0xFFFF,  // UInt16	tmr1Compare;				// $604: Timer 1 Compare Register
    0x0000,  // UInt16	tmr1Capture;				// $606: Timer 1 Capture Register
    0x0000,  // UInt16	tmr1Counter;				// $608: Timer 1 Counter Register
    0x0000,  // UInt16	tmr1Status;					// $60A: Timer 1 Status
             // Register

    {0},  // UInt8
          // ___filler24[0x610-0x60C];

    0x0000,  // UInt16	tmr2Control;				// $610: Timer 2 Control Register
    0x0000,  // UInt16	tmr2Prescaler;				// $612: Timer 2 Prescaler Register
    0xFFFF,  // UInt16	tmr2Compare;				// $614: Timer 2 Compare Register
    0x0000,  // UInt16	tmr2Capture;				// $616: Timer 2 Capture Register
    0x0000,  // UInt16	tmr2Counter;				// $618: Timer 2 Counter Register
    0x0000,  // UInt16	tmr2Status;					// $61A: Timer 2 Status
             // Register

    {0},  // UInt8
          // ___filler25[0x700-0x61C];

    0x0000,  // UInt16	spiRxD;						// $700: SPI Unit 1 Receive
             // Data Register
    0x0000,  // UInt16	spiTxD;						// $702: SPI Unit 1 Transmit
             // Data Register
    0x0000,  // UInt16	spiCont1;					// $704: SPI Unit 1
             // Control/Status Register
    0x0000,  // UInt16	spiIntCS;					// $706: SPI Unit 1
             // Interrupt control/Status Register
    0x0000,  // UInt16	spiTest;					// $708: SPI Unit 1 Test
             // Register
    0x0000,  // UInt16	spiSpc;						// $70A: SPI Unit 1 Sample
             // period counter register

    {0},  // UInt8
          // ___filler26[0x800-0x706];

    0x0000,  // UInt16	spiMasterData;				// $800: SPI Unit 2 Data Register
             // (SPI 2 is the same as the single SPI Master in EZ)
    0x0000,  // UInt16	spiMasterControl;			// $802: SPI Unit 2 Control/Status
             // Register

    {0},  // UInt8
          // ___filler27[0x900-0x804];

    0x0000,  // UInt16	uControl;					// $900: Uart 1
             // Status/Control Register (Uart 1 is the same as the single Uart in EZ)
    0x003F,  // UInt16	uBaud;						// $902: Uart 1 Baud Control
             // Register
    0x0000,  // UInt16	uReceive;					// $904: Uart 1 Receive
             // Register
    0x0000,  // UInt16	uTransmit;					// $906: Uart 1 Transmit
             // Register
    0x0000,  // UInt16	uMisc;						// $908: Uart 1
             // Miscellaneous Register
    0x0000,  // UInt16	uNonIntPresc;				// $90A: Uart 1 Non-Integer
             // Prescaler

    {0},  // UInt8
          // ___filler28[0x910-0x90C];

    0x0000,  // UInt16	u2Control;					// $910: Uart 2
             // Status/Control Register
    0x003F,  // UInt16	u2Baud;						// $912: Uart 2 Baud Control
             // Register
    0x0000,  // UInt16	u2Receive;					// $914: Uart 2 Receiver
             // Register
    0x0000,  // UInt16	u2Transmit;					// $916: Uart 2 Transmitter
             // Register
    0x0000,  // UInt16	u2Misc;						// $918: Uart 2
             // Miscellaneous Register
    0x0000,  // UInt16	u2NonIntPresc;				// $91A: Uart 2 Non-Integer
             // Prescaler
    0x0000,  // UInt16	u2FIFOHMark;				// $91C: Uart 2 Half Mark FIFO
             // Register

    {0},  // UInt8
          // ___filler28a[0xA00-0x91E];

    0x00000000,  // UInt32	lcdStartAddr;				// $A00: Screen Starting
                 // Address Register
    0,           // UInt8									___filler29;
    0xFF,        // UInt8	lcdPageWidth;				// $A05: Virtual Page Width Register
    {0},         // UInt8 ___filler30[2];
    0x03F0,      // UInt16	lcdScreenWidth;				// $A08: Screen Width Register
    0x01FF,      // UInt16	lcdScreenHeight;			// $A0A: Screen Height Register
    {0},         // UInt8
                 // ___filler31[0xA18-0xA0C];
    0x0000,      // UInt16	lcdCursorXPos;				// $A18: Cursor X Position
    0x0000,      // UInt16	lcdCursorYPos;				// $A1A: Cursor Y Position
    0x0101,      // UInt16	lcdCursorWidthHeight;		// $A1C: Cursor Width and Height
    0,           // UInt8									___filler32;
    0x7F,        // UInt8	lcdBlinkControl;			// $A1F: Blink Control Register
    0x00,        // UInt8	lcdPanelControl;			// $A20: Panel Interface
                 // Configuration Register
    0x00,        // UInt8	lcdPolarity;				// $A21: Polarity Config Register
    0,           // UInt8									___filler33;
    0x00,        // UInt8	lcdACDRate;					// $A23: ACD (M) Rate
                 // Control Register
    0,           // UInt8									___filler34;
    0x00,        // UInt8	lcdPixelClock;				// $A25: Pixel Clock Divider
                 // Register
    0,           // UInt8									___filler35;
    0x00,        // UInt8	lcdClockControl;			// $A27: Clocking Control Register
    0x00FF,      // UInt16	lcdRefreshRateAdj;			// $A28: Refresh Rate Adjustment
                 // Register
    0,           // UInt8									___filler37;
    0x00,        // UInt8	lcdReserved1;				// $A2B: Reserved
    0,           // UInt8									___filler38;
    0x00,        // UInt8    lcdPanningOffset;			// $A2D: Panning Offset Register

    {0},  // UInt8
          // ___filler39[0xA31-0xA2E];

    0x00,    // UInt8	lcdFrameRate;				// $A31: Frame Rate Control
             // Modulation Register
    0,       // UInt8 ___filler2004;
    0x84,    // UInt8	lcdGrayPalette;				// $A33: Gray Palette Mapping
             // Register
    0x00,    // UInt8	lcdReserved2;				// $A34: Reserved
    0,       // UInt8 ___filler2005;
    0x0000,  // UInt16	lcdContrastControlPWM;		// $A36: Contrast Control
    0x00,    // UInt8	lcdRefreshModeControl;		// $A38: Refresh Mode Control Register
    0x62,    // UInt8	lcdDMAControl;				// $A39: DMA Control Register

    {0},  // UInt8
          // ___filler40[0xB00-0xA3a];

    0x00000000,  // UInt32	rtcHourMinSec;				// $B00: RTC Hours, Minutes,
                 // Seconds Register
    0x00000000,  // UInt32	rtcAlarm;					// $B04: RTC Alarm
                 // Register
    {0},         // UInt8
                 // ___filler2001[0xB0A-0xB08];
    0x0001,      // UInt16   rtcWatchDog;				// $B0A: RTC Watchdog Timer
    0x0080,      // UInt16	rtcControl;					// $B0C: RTC Control
                 // Register
    0x0000,      // UInt16	rtcIntStatus;				// $B0E: RTC Interrupt Status
                 // Register
    0x0000,      // UInt16	rtcIntEnable;				// $B10: RTC Interrupt Enable
                 // Register
    0x003F,      // UInt16	stopWatch;					// $B12: Stopwatch Minutes
    {0},         // UInt8
                 // ___filler2002[0xB1A-0xB14];
    0x0000,      // UInt16	rtcDay;						// $B1A: RTC Day
    0x0000,      // UInt16   rtcDayAlarm;				// $B1C: RTC Day Alarm

    {0},  // UInt8
          // ___filler41[0xC00-0xB1E];

    0x0000,  // UInt16	dramConfig;					// $C00: DRAM Memory Config
             // Register
    0x003C,  // UInt16   dramControl;				// $C02: DRAM Control Register
    0x0000,  // UInt16   sdramControl;				// $C04: SDRAM Control Register
    0x0000,  // UInt16   sdramPwDn;              	// $C06: SDRAM Power Down Register

    {0},  // UInt8
          // ___filler42[0xD00-0xC08];

    0x00000000,  // UInt32	emuAddrCompare;				// $D00: Emulation Address
                 // Compare Register
    0x00000000,  // UInt32	emuAddrMask;				// $D04: Emulation Address
                 // Mask Register
    0x0000,      // UInt16	emuControlCompare;			// $D08: Emulation Control Compare
                 // Register
    0x0000,      // UInt16	emuControlMask;				// $D0A: Emulation Control Mask
                 // Register
    0x0000,      // UInt16	emuControl;					// $DOC: Emulation Control
                 // Register
    0x0000       // UInt16	emuStatus;					// $D0E: Emulation Status
                 // Register
};

// ---------------------------------------------------------------------------
//		� EmRegsVZ::EmRegsVZ
// ---------------------------------------------------------------------------

EmRegsVZ::EmRegsVZ(void)
    : EmRegs(),
      f68VZ328Regs(),
      fHotSyncButtonDown(0),
      fKeyBits(0),
      fLastTmr1Status(0),
      fLastTmr2Status(0),
      fPortDEdge(0),
      fPortDDataCount(0) {
    fUART[0] = NULL;
    fUART[1] = NULL;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::~EmRegsVZ
// ---------------------------------------------------------------------------

EmRegsVZ::~EmRegsVZ(void) {}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::Initialize
// ---------------------------------------------------------------------------

void EmRegsVZ::Initialize(void) {
    EmRegs::Initialize();
    rtcDayAtWrite = 0;
    lastRtcAlarmCheck = -1;
    pwmActive = false;
    afterLoad = false;
    onCycleHandle = EmHAL::onCycle.AddHandler(
        [&](uint64 systemCycles, bool sleeping) { this->Cycle(systemCycles, sleeping); });

    tmr1LastProcessedSystemCycles = gSession->GetSystemCycles();
    tmr2LastProcessedSystemCycles = gSession->GetSystemCycles();
    UpdateTimerTicksPerSecond();

    fUART[0] = new EmUARTDragonball(EmUARTDragonball::kUART_DragonballVZ, 0);
    fUART[1] = new EmUARTDragonball(EmUARTDragonball::kUART_DragonballVZ, 1);

    onMarkScreenCleanHandle = gSystemState.onMarkScreenClean.AddHandler([this]() { MarkScreen(); });

    ApplySdctl();
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::Reset
// ---------------------------------------------------------------------------

void EmRegsVZ::Reset(Bool hardwareReset) {
    EmRegs::Reset(hardwareReset);
    UnmarkScreen();

    if (hardwareReset) {
        tmr1LastProcessedSystemCycles = gSession->GetSystemCycles();
        tmr2LastProcessedSystemCycles = gSession->GetSystemCycles();

        f68VZ328Regs = kInitial68VZ328RegisterValues;

        // Byteswap all the words in the DragonballVZ registers (if necessary).

        Canonical(f68VZ328Regs);
        ByteswapWords(&f68VZ328Regs, sizeof(f68VZ328Regs));

        fKeyBits = 0;
        fLastTmr1Status = 0;
        fLastTmr2Status = 0;
        fPortDEdge = 0;
        fPortDDataCount = 0;

        pwmActive = false;

        // React to the new data in the UART registers.

        Bool sendTxData = false;
        EmRegsVZ::UARTStateChanged(sendTxData, 0);
        EmRegsVZ::UARTStateChanged(sendTxData, 1);

        ApplySdctl();
    }

    UpdateTimerTicksPerSecond();
}

void EmRegsVZ::Save(Savestate& savestate) { DoSave(savestate); }

void EmRegsVZ::Save(SavestateProbe& savestate) { DoSave(savestate); }

void EmRegsVZ::Load(SavestateLoader& loader) {
    if (fSPISlaveADC) fSPISlaveADC->Load(loader);

    Chunk* chunk = loader.GetChunk(ChunkType::regsVZ);
    if (!chunk) return;

    const uint32 version = chunk->Get32();
    if (version > SAVESTATE_VERSION) {
        logging::printf("unable to restore RegsVZ: unsupported savestate version\n");
        loader.NotifyError();

        return;
    }

    pwmActive = false;

    LoadChunkHelper helper(*chunk);
    DoSaveLoad(helper, version);

    Bool sendTxData = false;
    EmRegsVZ::UARTStateChanged(sendTxData, 0);
    EmRegsVZ::UARTStateChanged(sendTxData, 1);

    gMemAccessFlags.fProtect_SRAMSet = (READ_REGISTER(csDSelect) & 0x2000) != 0;

    ApplySdctl();

    markScreen = true;
    afterLoad = true;
}

template <typename T>
void EmRegsVZ::DoSave(T& savestate) {
    if (fSPISlaveADC) fSPISlaveADC->Save(savestate);

    typename T::chunkT* chunk = savestate.GetChunk(ChunkType::regsVZ);
    if (!chunk) return;

    chunk->Put32(SAVESTATE_VERSION);

    SaveChunkHelper helper(*chunk);
    DoSaveLoad(helper, SAVESTATE_VERSION);
}

template <typename T>
void EmRegsVZ::DoSaveLoad(T& helper, uint32 version) {
    ::DoSaveLoad(helper, f68VZ328Regs);

    helper.DoBool(fHotSyncButtonDown)
        .Do16(fKeyBits)
        .Do16(fLastTmr1Status)
        .Do16(fLastTmr2Status)
        .Do8(fPortDEdge)
        .Do32(fPortDDataCount)
        .DoDouble(tmr1LastProcessedSystemCycles)
        .DoDouble(tmr2LastProcessedSystemCycles)
        .DoDouble(timer1TicksPerSecond)
        .DoDouble(timer2TicksPerSecond)
        .Do32(rtcDayAtWrite)
        .Do32(lastRtcAlarmCheck);

    if (version > 1) helper.DoBool(pwmActive);
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::Dispose
// ---------------------------------------------------------------------------

void EmRegsVZ::Dispose(void) {
    delete fUART[0];
    fUART[0] = NULL;
    delete fUART[1];
    fUART[1] = NULL;

    EmRegs::Dispose();

    gSystemState.onMarkScreenClean.RemoveHandler(onMarkScreenCleanHandle);

    EmHAL::onCycle.RemoveHandler(onCycleHandle);
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::SetSubBankHandlers
// ---------------------------------------------------------------------------

void EmRegsVZ::SetSubBankHandlers(void) {
    // Install base handlers.

    EmRegs::SetSubBankHandlers();

    // Now add standard/specialized handers for the defined registers.

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

    INSTALL_HANDLER(StdRead, StdWrite, csUGroupBase);

    INSTALL_HANDLER(StdRead, csControl1Write, csControl1);
    INSTALL_HANDLER(StdRead, StdWrite, csControl2);
    INSTALL_HANDLER(StdRead, StdWrite, csControl3);

    INSTALL_HANDLER(StdRead, csASelectWrite, csASelect);
    INSTALL_HANDLER(StdRead, StdWrite, csBSelect);
    INSTALL_HANDLER(StdRead, StdWrite, csCSelect);
    INSTALL_HANDLER(StdRead, csDSelectWrite, csDSelect);

    INSTALL_HANDLER(StdRead, StdWrite, emuCS);

    INSTALL_HANDLER(StdRead, pllRegisterWrite, pllControl);
    INSTALL_HANDLER(pllFreqSelRead, pllRegisterWrite, pllFreqSel);
    INSTALL_HANDLER(StdRead, StdWrite, pwrControl);

    INSTALL_HANDLER(StdRead, StdWrite, intVector);
    INSTALL_HANDLER(StdRead, StdWrite, intControl);
    INSTALL_HANDLER(StdRead, intMaskHiWrite, intMaskHi);
    INSTALL_HANDLER(StdRead, intMaskLoWrite, intMaskLo);
    INSTALL_HANDLER(StdRead, intStatusHiWrite, intStatusHi);
    INSTALL_HANDLER(StdRead, NullWrite, intStatusLo);
    INSTALL_HANDLER(StdRead, NullWrite, intPendingHi);
    INSTALL_HANDLER(StdRead, NullWrite, intPendingLo);
    INSTALL_HANDLER(StdRead, StdWrite, intLevelControl);

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
    INSTALL_HANDLER(StdRead, StdWrite, portDPolarity);
    INSTALL_HANDLER(StdRead, portDIntReqEnWrite, portDIntReqEn);
    INSTALL_HANDLER(StdRead, StdWrite, portDKbdIntEn);
    INSTALL_HANDLER(StdRead, StdWrite, portDIntEdge);

    INSTALL_HANDLER(StdRead, StdWrite, portEDir);
    INSTALL_HANDLER(portXDataRead, portXDataWrite, portEData);
    INSTALL_HANDLER(StdRead, StdWrite, portEPullupEn);
    INSTALL_HANDLER(StdRead, StdWrite, portESelect);

    INSTALL_HANDLER(StdRead, StdWrite, portFDir);
    INSTALL_HANDLER(portXDataRead, portXDataWrite, portFData);
    INSTALL_HANDLER(StdRead, StdWrite, portFPullupdnEn);
    INSTALL_HANDLER(StdRead, StdWrite, portFSelect);

    INSTALL_HANDLER(StdRead, StdWrite, portGDir);
    INSTALL_HANDLER(portXDataRead, portXDataWrite, portGData);
    INSTALL_HANDLER(StdRead, StdWrite, portGPullupEn);
    INSTALL_HANDLER(StdRead, StdWrite, portGSelect);

    INSTALL_HANDLER(StdRead, StdWrite, portJDir);
    INSTALL_HANDLER(portXDataRead, portXDataWrite, portJData);
    INSTALL_HANDLER(StdRead, StdWrite, portJPullupEn);
    INSTALL_HANDLER(StdRead, StdWrite, portJSelect);

    INSTALL_HANDLER(StdRead, StdWrite, portKDir);
    INSTALL_HANDLER(portXDataRead, portXDataWrite, portKData);
    INSTALL_HANDLER(StdRead, StdWrite, portKPullupdnEn);
    INSTALL_HANDLER(StdRead, StdWrite, portKSelect);

    INSTALL_HANDLER(StdRead, StdWrite, portMDir);
    INSTALL_HANDLER(portXDataRead, portXDataWrite, portMData);
    INSTALL_HANDLER(StdRead, StdWrite, portMPullupdnEn);
    INSTALL_HANDLER(StdRead, StdWrite, portMSelect);

    INSTALL_HANDLER(StdRead, pwmc1Write, pwmControl);
    INSTALL_HANDLER(StdRead, StdWrite, pwmSampleHi);
    INSTALL_HANDLER(StdRead, pwms1Write, pwmSampleLo);
    INSTALL_HANDLER(StdRead, pwmp1Write, pwmPeriod);
    INSTALL_HANDLER(StdRead, NullWrite, pwmCounter);

    INSTALL_HANDLER(StdRead, StdWrite, pwm2Control);
    INSTALL_HANDLER(StdRead, StdWrite, pwm2Period);
    INSTALL_HANDLER(StdRead, StdWrite, pwm2Width);
    INSTALL_HANDLER(StdRead, NullWrite, pwm2Counter);

    INSTALL_HANDLER(StdRead, tmr1RegisterWrite, tmr1Control);
    INSTALL_HANDLER(StdRead, tmr1RegisterWrite, tmr1Prescaler);
    INSTALL_HANDLER(StdRead, StdWrite, tmr1Compare);
    INSTALL_HANDLER(StdRead, StdWrite, tmr1Capture);
    INSTALL_HANDLER(StdRead, NullWrite, tmr1Counter);
    INSTALL_HANDLER(tmr1StatusRead, tmr1StatusWrite, tmr1Status);

    INSTALL_HANDLER(StdRead, StdWrite, tmr2Control);
    INSTALL_HANDLER(StdRead, StdWrite, tmr2Prescaler);
    INSTALL_HANDLER(StdRead, StdWrite, tmr2Compare);
    INSTALL_HANDLER(StdRead, StdWrite, tmr2Capture);
    INSTALL_HANDLER(StdRead, NullWrite, tmr2Counter);
    INSTALL_HANDLER(tmr2StatusRead, tmr2StatusWrite, tmr2Status);

    INSTALL_HANDLER(StdRead, StdWrite, spiRxD);
    INSTALL_HANDLER(StdRead, StdWrite, spiTxD);
    INSTALL_HANDLER(StdRead, spiCont1Write, spiCont1);
    INSTALL_HANDLER(StdRead, StdWrite, spiIntCS);
    INSTALL_HANDLER(StdRead, StdWrite, spiTest);
    INSTALL_HANDLER(StdRead, StdWrite, spiSpc);

    INSTALL_HANDLER(StdRead, StdWrite, spiMasterData);
    INSTALL_HANDLER(StdRead, spiMasterControlWrite, spiMasterControl);

    INSTALL_HANDLER(uart1Read, uart1Write, uControl);
    INSTALL_HANDLER(uart1Read, uart1Write, uBaud);
    INSTALL_HANDLER(uart1Read, uart1Write, uReceive);
    INSTALL_HANDLER(uart1Read, uart1Write, uTransmit);
    INSTALL_HANDLER(uart1Read, uart1Write, uMisc);
    INSTALL_HANDLER(uart1Read, uart1Write, uNonIntPresc);

    INSTALL_HANDLER(uart2Read, uart2Write, u2Control);
    INSTALL_HANDLER(uart2Read, uart2Write, u2Baud);
    INSTALL_HANDLER(uart2Read, uart2Write, u2Receive);
    INSTALL_HANDLER(uart2Read, uart2Write, u2Transmit);
    INSTALL_HANDLER(uart2Read, uart2Write, u2Misc);
    INSTALL_HANDLER(uart2Read, uart2Write, u2NonIntPresc);
    INSTALL_HANDLER(uart2Read, uart2Write, u2FIFOHMark);

    INSTALL_HANDLER(StdRead, lcdRegisterWrite, lcdStartAddr);
    INSTALL_HANDLER(StdRead, lcdRegisterWrite, lcdPageWidth);
    INSTALL_HANDLER(StdRead, lcdRegisterWrite, lcdScreenWidth);
    INSTALL_HANDLER(StdRead, lcdRegisterWrite, lcdScreenHeight);
    INSTALL_HANDLER(StdRead, StdWrite, lcdCursorXPos);
    INSTALL_HANDLER(StdRead, StdWrite, lcdCursorYPos);
    INSTALL_HANDLER(StdRead, StdWrite, lcdCursorWidthHeight);
    INSTALL_HANDLER(StdRead, StdWrite, lcdBlinkControl);
    INSTALL_HANDLER(StdRead, lcdRegisterWrite, lcdPanelControl);
    INSTALL_HANDLER(StdRead, StdWrite, lcdPolarity);
    INSTALL_HANDLER(StdRead, StdWrite, lcdACDRate);
    INSTALL_HANDLER(StdRead, StdWrite, lcdPixelClock);
    INSTALL_HANDLER(StdRead, StdWrite, lcdClockControl);
    INSTALL_HANDLER(StdRead, StdWrite, lcdRefreshRateAdj);
    INSTALL_HANDLER(StdRead, lcdRegisterWrite, lcdPanningOffset);
    INSTALL_HANDLER(StdRead, StdWrite, lcdFrameRate);
    INSTALL_HANDLER(StdRead, lcdRegisterWrite, lcdGrayPalette);
    INSTALL_HANDLER(StdRead, StdWrite, lcdContrastControlPWM);
    INSTALL_HANDLER(StdRead, StdWrite, lcdRefreshModeControl);
    INSTALL_HANDLER(StdRead, StdWrite, lcdDMAControl);

    INSTALL_HANDLER(rtcHourMinSecRead, StdWrite, rtcHourMinSec);
    INSTALL_HANDLER(StdRead, StdWrite, rtcAlarm);
    INSTALL_HANDLER(StdRead, StdWrite, rtcWatchDog);
    INSTALL_HANDLER(StdRead, rtcControlWrite, rtcControl);
    INSTALL_HANDLER(StdRead, rtcIntStatusWrite, rtcIntStatus);
    INSTALL_HANDLER(StdRead, rtcIntEnableWrite, rtcIntEnable);
    INSTALL_HANDLER(StdRead, StdWrite, stopWatch);
    INSTALL_HANDLER(rtcDayRead, rtcDayWrite, rtcDay);
    INSTALL_HANDLER(StdRead, StdWrite, rtcDayAlarm);

    INSTALL_HANDLER(StdRead, StdWrite, dramConfig);
    INSTALL_HANDLER(StdRead, StdWrite, dramControl);
    INSTALL_HANDLER(StdRead, sdctlWrite, sdramControl);
    INSTALL_HANDLER(StdRead, StdWrite, sdramPwDn);

    INSTALL_HANDLER(StdRead, StdWrite, emuAddrCompare);
    INSTALL_HANDLER(StdRead, StdWrite, emuAddrMask);
    INSTALL_HANDLER(StdRead, StdWrite, emuControlCompare);
    INSTALL_HANDLER(StdRead, StdWrite, emuControlMask);
    INSTALL_HANDLER(StdRead, StdWrite, emuControl);
    INSTALL_HANDLER(StdRead, StdWrite, emuStatus);
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::GetRealAddress
// ---------------------------------------------------------------------------

uint8* EmRegsVZ::GetRealAddress(emuptr address) {
    uint8* loc = ((uint8*)&f68VZ328Regs) + (address - kMemoryStart);

    return loc;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::GetAddressStart
// ---------------------------------------------------------------------------

emuptr EmRegsVZ::GetAddressStart(void) { return kMemoryStart; }

// ---------------------------------------------------------------------------
//		� EmRegsVZ::GetAddressRange
// ---------------------------------------------------------------------------

uint32 EmRegsVZ::GetAddressRange(void) { return kMemorySize; }

// ---------------------------------------------------------------------------
//		� EmRegsVZ::Cycle
// ---------------------------------------------------------------------------
// Handles periodic events that need to occur when the processor cycles (like
// updating timer registers).  This function is called in two places from
// Emulator::Execute.  Interestingly, the loop runs 3% FASTER if this function
// is in its own separate function instead of being inline.

void EmRegsVZ::Cycle(uint64 systemCycles, Bool sleeping) {
    if (afterLoad) {
        DispatchPwmChange();
        afterLoad = false;
    }

    if (GetAsleep()) return;

    double clocksPerSecond = gSession->GetClocksPerSecond();

    if (((READ_REGISTER(tmr1Control) & hwrVZ328TmrControlEnable) != 0) &&
        timer1TicksPerSecond > 0) {
        // If so, increment the timer.

        uint32 ticks = ((double)systemCycles - tmr1LastProcessedSystemCycles) / clocksPerSecond *
                       timer1TicksPerSecond;

        tmr1LastProcessedSystemCycles += (double)ticks / timer1TicksPerSecond * clocksPerSecond;

        WRITE_REGISTER(tmr1Counter, READ_REGISTER(tmr1Counter) + ticks);

        // Determine whether the timer has reached the specified count.

        uint16 tcmp = READ_REGISTER(tmr1Compare);
        uint16 tcn = READ_REGISTER(tmr1Counter);

        if (tcn >= tcmp) {
            // Flag the occurrence of the successful comparison.

            WRITE_REGISTER(tmr1Status, READ_REGISTER(tmr1Status) | hwrVZ328TmrStatusCompare);

            // If the Free Run/Restart flag is not set, clear the counter.

            if ((READ_REGISTER(tmr1Control) & hwrVZ328TmrControlFreeRun) == 0) {
                WRITE_REGISTER(tmr1Counter, tcn - tcmp);
            }

            // If the timer interrupt is enabled, post an interrupt.

            if ((READ_REGISTER(tmr1Control) & hwrVZ328TmrControlEnInterrupt) != 0) {
                WRITE_REGISTER(intPendingLo, READ_REGISTER(intPendingLo) | hwrVZ328IntLoTimer);
                EmRegsVZ::UpdateInterrupts();
            }
        }
    }

    if (((READ_REGISTER(tmr2Control) & hwrVZ328TmrControlEnable) != 0) &&
        timer2TicksPerSecond > 0) {
        // If so, increment the timer.

        uint32 ticks = ((double)systemCycles - tmr2LastProcessedSystemCycles) / clocksPerSecond *
                       timer2TicksPerSecond;

        tmr2LastProcessedSystemCycles += (double)ticks / timer2TicksPerSecond * clocksPerSecond;

        WRITE_REGISTER(tmr2Counter, READ_REGISTER(tmr2Counter) + ticks);

        // Determine whether the timer has reached the specified count.

        uint16 tcmp = READ_REGISTER(tmr2Compare);
        uint16 tcn = READ_REGISTER(tmr2Counter);

        if (tcn >= tcmp) {
            // Flag the occurrence of the successful comparison.

            WRITE_REGISTER(tmr2Status, READ_REGISTER(tmr2Status) | hwrVZ328TmrStatusCompare);

            // If the Free Run/Restart flag is not set, clear the counter.

            if ((READ_REGISTER(tmr2Control) & hwrVZ328TmrControlFreeRun) == 0) {
                WRITE_REGISTER(tmr2Counter, tcn - tcmp);
            }

            // If the timer interrupt is enabled, post an interrupt.

            if ((READ_REGISTER(tmr2Control) & hwrVZ328TmrControlEnInterrupt) != 0) {
                WRITE_REGISTER(intPendingLo, READ_REGISTER(intPendingLo) | hwrVZ328IntLoTimer2);
                EmRegsVZ::UpdateInterrupts();
            }
        }
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::CycleSlowly
// ---------------------------------------------------------------------------
// Handles periodic events that need to occur when the processor cycles (like
// updating timer registers).  This function is called in two places from
// Emulator::Execute.  Interestingly, the loop runs 3% FASTER if this function
// is in its own separate function instead of being inline.

void EmRegsVZ::CycleSlowly(Bool sleeping) {
    UNUSED_PARAM(sleeping)

    // See if a hard button is pressed.

    EmAssert(gSession);

    if (gSession->HasButtonEvent()) {
        ButtonEventT event = gSession->NextButtonEvent();
        if (event.GetButton() == ButtonEventT::Button::cradle)
            EmRegsVZ::HotSyncEvent(event.GetType() == ButtonEventT::Type::press);
        else
            EmRegsVZ::ButtonEvent(event);
    }

    // See if there's anything new ("Put the data on the bus")

    EmRegsVZ::UpdateUARTState(false, 0);
    EmRegsVZ::UpdateUARTState(false, 1);

    // Check to see if the RTC alarm is ready to go off.  First see
    // if the RTC is enabled, and that the alarm event isn't already
    // registered (the latter check is just an optimization).

    if ((READ_REGISTER(rtcIntEnable) & hwrVZ328RTCIntEnableAlarm) != 0 &&
        (READ_REGISTER(rtcIntStatus) & hwrVZ328RTCIntStatusAlarm) == 0) {
        uint32 rtcAlarm = READ_REGISTER(rtcAlarm);

        uint32 almHour = (rtcAlarm & hwrVZ328RTCAlarmHoursMask) >> hwrVZ328RTCAlarmHoursOffset;
        uint32 almMin = (rtcAlarm & hwrVZ328RTCAlarmMinutesMask) >> hwrVZ328RTCAlarmMinutesOffset;
        uint32 almSec = (rtcAlarm & hwrVZ328RTCAlarmSecondsMask) >> hwrVZ328RTCAlarmSecondsOffset;
        int32 almInSeconds = (almHour * 60 * 60) + (almMin * 60) + almSec;

        uint32 hour, min, sec;
        Platform::GetTime(hour, min, sec);
        int32 nowInSeconds = hour * 3600 + min * 60 + sec;

        if (lastRtcAlarmCheck < almInSeconds && almInSeconds <= nowInSeconds) {
            WRITE_REGISTER(rtcIntStatus, READ_REGISTER(rtcIntStatus) | hwrVZ328RTCIntStatusAlarm);
            EmRegsVZ::UpdateRTCInterrupts();
        }

        lastRtcAlarmCheck = nowInSeconds;
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::TurnSoundOff
// ---------------------------------------------------------------------------

void EmRegsVZ::TurnSoundOff(void) {
    uint16 pwmControl = READ_REGISTER(pwmControl);
    WRITE_REGISTER(pwmControl, pwmControl & ~hwrVZ328PWMControlEnable);
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::ResetTimer
// ---------------------------------------------------------------------------

void EmRegsVZ::ResetTimer(void) {
    WRITE_REGISTER(tmr1Counter, 0);
    WRITE_REGISTER(tmr2Counter, 0);
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::ResetRTC
// ---------------------------------------------------------------------------

void EmRegsVZ::ResetRTC(void) {}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::GetInterruptLevel
// ---------------------------------------------------------------------------

int32 EmRegsVZ::GetInterruptLevel(void) {
    /*
            From the DragonBall VZ manual, Chapter 9:

                    * EMUIRQ or hardware breakpoint interrupt (level 7)

                    * IRQ6 external interrupt (level 6)
                    * Timer unit 1 (level 6)
                    * Timer unit 2 (configurable from level 1 to 6)
                    * Pulse-width modulator unit 1 (level 6)
                    * Pulse-width modulator unit 2 (configurable from level 1 to 6)

                    * IRQ5 external interrupt-pen (level 5)

                    * Serial peripheral interface unit 1 (configurable from level 1 to 6)
                    * Serial peripheral interface unit 2 (level 4)
                    * UART unit 1 (level 4)
                    * UART unit 2 (configurable from level 1 to 6)
                    * Software watchdog timer interrupt (level 4)
                    * Real-time clock (level 4)
                    * Real-time interrupt (level 4)
                    * Keyboard interrupt (level 4)
                    * General-purpose interrupt INT[3:0] (level 4)-these pins can be
                      used as keyboard interrupts

                    * IRQ3 external interrupt (level 3)
                    * IRQ2 external interrupt (level 2)
                    * IRQ1 external interrupt (level 1)
    */

    static int8 intLevel[32] = {
        4,  // 0x00 - hwrVZ328IntLoSPIM
        6,  // 0x01 - hwrVZ328IntLoTimer
        4,  // 0x02 - hwrVZ328IntLoUART
        4,  // 0x03 - hwrVZ328IntLoWDT
        4,  // 0x04 - hwrVZ328IntLoRTC
        0,  // 0x05 - hwrVZ328IntLoTimer2 (configurable - 3 by default, reprogrammed to level 6)
        4,  // 0x06 - hwrVZ328IntLoKbd
        6,  // 0x07 - hwrVZ328IntLoPWM

        4,   // 0x08 - hwrVZ328IntLoInt0
        4,   // 0x09 - hwrVZ328IntLoInt1
        4,   // 0x0A - hwrVZ328IntLoInt2
        4,   // 0x0B - hwrVZ328IntLoInt3
        0,   // 0x0C - hwrVZ328IntLoUART2 (configurable - 5 by default, reprogrammed to level 4)
        0,   // 0x0D - PWM2 (configurable - 3 by default)
        -2,  // 0x0E - <Unused>
        -2,  // 0x0F - <Unused>

        1,  // 0x10 - hwrVZ328IntHiIRQ1
        2,  // 0x11 - hwrVZ328IntHiIRQ2
        3,  // 0x12 - hwrVZ328IntHiIRQ3
        6,  // 0x13 - hwrVZ328IntHiIRQ6
        5,  // 0x14 - hwrVZ328IntHiPen
        0,  // 0x15 - SPI1 (configurable - 6 by default)
        4,  // 0x16 - hwrVZ328IntHiSampleTimer
        7,  // 0x17 - hwrVZ328IntHiEMU

        -2,  // 0x18 - <Unused>
        -2,  // 0x19 - <Unused>
        -2,  // 0x1A - <Unused>
        -2,  // 0x1B - <Unused>
        -2,  // 0x1C - <Unused>
        -2,  // 0x1D - <Unused>
        -2,  // 0x1E - <Unused>
        -2   // 0x1F - <Unused>
    };

    // Load in the configurable interrupt levels.

    uint16 intLevelControl = READ_REGISTER(intLevelControl);

    intLevel[0x15] = (intLevelControl >> 12) & 0x000F;
    intLevel[0x0C] = (intLevelControl >> 8) & 0x000F;
    intLevel[0x0D] = (intLevelControl >> 4) & 0x000F;
    intLevel[0x05] = (intLevelControl >> 0) & 0x000F;

    // !!! HACK: Skywalker sets the interrupt level to zero in order
    // to clear it out before setting it to six.  Zero is not allowed,
    // so mask the problem by replacing it with six.

    if (intLevel[0x05] == 0) intLevel[0x05] = 6;

#ifndef NDEBUG
    for (int ii = 0; ii < 32; ++ii) {
        EmAssert(intLevel[ii] != 0);
        EmAssert(intLevel[ii] != 7 || ii == 0x17);
    }
#endif

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
//		� EmRegsVZ::GetInterruptBase
// ---------------------------------------------------------------------------

int32 EmRegsVZ::GetInterruptBase(void) { return READ_REGISTER(intVector) & 0xF8; }

// ---------------------------------------------------------------------------
//		� EmRegsVZ::GetLCDHasFrame
// ---------------------------------------------------------------------------

Bool EmRegsVZ::GetLCDHasFrame(void) { return false; }

// ---------------------------------------------------------------------------
//		� EmRegsVZ::GetLCDBeginEnd
// ---------------------------------------------------------------------------

void EmRegsVZ::GetLCDBeginEnd(emuptr& begin, emuptr& end) {
    emuptr baseAddr = READ_REGISTER(lcdStartAddr);
    int rowBytes = READ_REGISTER(lcdPageWidth) * 2;
    int height = READ_REGISTER(lcdScreenHeight) + 1;

    begin = baseAddr;
    end = baseAddr + rowBytes * height;
}

bool EmRegsVZ::CopyLCDFrame(Frame& frame) {
    // Get the screen metrics.

    frame.bpp = 1 << (READ_REGISTER(lcdPanelControl) & 0x03);
    frame.lineWidth = READ_REGISTER(lcdScreenWidth);
    frame.lines = READ_REGISTER(lcdScreenHeight) + 1;
    frame.bytesPerLine = READ_REGISTER(lcdPageWidth) * 2;
    frame.margin = READ_REGISTER(lcdPanningOffset);
    emuptr baseAddr = READ_REGISTER(lcdStartAddr);

    if (baseAddr == 0) return false;

    switch (frame.bpp) {
        case 1:
            frame.margin &= 0x0f;
            break;

        case 2:
            frame.margin &= 0x07;
            break;

        case 4:
            frame.margin &= 0x03;
            break;

        default:
            return false;
    }

    // Determine first and last scanlines to fetch, and fetch them.

    emuptr firstLineAddr = baseAddr;
    emuptr lastLineAddr = baseAddr + frame.lines * frame.bytesPerLine;

    uint8* dst = frame.GetBuffer();

    EmASSERT(frame.GetBufferSize() >= lastLineAddr - firstLineAddr);
    EmMem_memcpy((void*)dst, firstLineAddr, lastLineAddr - firstLineAddr);

    return true;
}

uint16 EmRegsVZ::GetLCD2bitMapping() {
    uint8 lgpmr = READ_REGISTER(lcdGrayPalette);
    return ((lgpmr & 0x0f) << 4) | ((lgpmr & 0xf0) << 4) | 0xf000;
}

void EmRegsVZ::MarkScreen() {
    if (!markScreen) return;

    markScreenWith(MetaMemory::MarkScreen, f68VZ328Regs);

    markScreen = false;
}

void EmRegsVZ::UnmarkScreen() {
    markScreenWith(MetaMemory::UnmarkScreen, f68VZ328Regs);

    markScreen = true;
}

void EmRegsVZ::MarkScreenDirty() { gSystemState.MarkScreenDirty(); }

void EmRegsVZ::UpdateTimerTicksPerSecond() {
    int32 systemClockFrequency = GetSystemClockFrequency();

    timer1TicksPerSecond = TimerTicksPerSecond(READ_REGISTER(tmr1Control),
                                               READ_REGISTER(tmr1Prescaler), systemClockFrequency);
    timer2TicksPerSecond = TimerTicksPerSecond(READ_REGISTER(tmr2Control),
                                               READ_REGISTER(tmr2Prescaler), systemClockFrequency);
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::GetDynamicHeapSize
// ---------------------------------------------------------------------------

int32 EmRegsVZ::GetDynamicHeapSize(void) {
    int32 result = 0;

    uint16 csControl = READ_REGISTER(csControl1);
    uint16 csDSelect = READ_REGISTER(csDSelect);

    uint16 csDUPSIZ = (csDSelect & UPSIZMask) >> UPSIZShift;
    uint16 csDSIZ = (csDSelect & SIZMask) >> SIZShift;

    if ((csControl & EUPENMask) != 0) {
        // Unprotected size = Chip-select Size/2^(7-upsize)

        // Determine upsize.

        // Merge in the extended bit.  UPSIZ contains [1:0].  The bit
        // in DUPS2 extends this to [2:0].

        csDUPSIZ |= ((csControl & DUPS2Mask) >> DUPS2Shift) << 2;

        // Determine Chip-select Size.

        long chip_select_size = (32 * 1024L) << csDSIZ;

        if ((csControl & DSIZ3Mask) != 0 && (csDSelect & DRAMMask) != 0 && csDSIZ <= 0x01) {
            chip_select_size = (8 * 1024L * 1024L) << csDSIZ;
        }

        result = chip_select_size / (1 << (7 - csDUPSIZ));
    } else {
        result = (32 * 1024L) << csDUPSIZ;
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::GetROMSize
// ---------------------------------------------------------------------------

int32 EmRegsVZ::GetROMSize(void) {
    /*
            SIZ	Chip-Select Size

            This field determines the memory range of the chip-select. For CSA
            and CSB, chip-select size is between 128K and 16M. For CSC and CSD,
            chip-select size is between 32K and 4M.

            000 = 128K (32K for CSC and CSD).
            001 = 256K (64K for CSC and CSD).
            010 = 512K (128K for CSC and CSD).
            011 = 1M (256K for CSC and CSD).
            100 = 2M (512K for CSC and CSD).
            101 = 4M (1M for CSC and CSD).
            110 = 8M (2M for CSC and CSD).
            111 = 16M (4M for CSC and CSD).
    */

    uint16 csASelect = READ_REGISTER(csASelect);
    uint32 result = (128 * 1024L) << ((csASelect & SIZMask) >> SIZShift);

    if ((csASelect & ENMask) == 0) {
        result = 16 * 1024L * 1024L;
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::GetROMBaseAddress
// ---------------------------------------------------------------------------

uint32 EmRegsVZ::GetROMBaseAddress(void) {
    /*
            csAGroupBase: Chip-select Group Base Address register

            The csAGroupBase register stores the base address (bits 14-28 of
            the address) in the top 15 bits.  The low bit is always zero.
            Shifting this value by 13 gives the ROM base address.

            E.g:  If the base address is 0x10C00000, then csAGroupBase will
            contain 0x10C00000 >> 13 (base 10) = 0x8600.

            If the enable bit of the CSA register is low, the chip selects
            have not yet been set up.  In this case, return an invalid value.
    */

    if (!this->ChipSelectsConfigured()) {
        return 0xFFFFFFFF;
    }

    uint16 csAGroupBase = READ_REGISTER(csAGroupBase);
    uint32 result = csAGroupBase << kBaseAddressShift;

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::ChipSelectsConfigured
// ---------------------------------------------------------------------------

Bool EmRegsVZ::ChipSelectsConfigured(void) { return READ_REGISTER(csASelect) & ENMask; }

// ---------------------------------------------------------------------------
//		� EmRegsVZ::GetSystemClockFrequency
// ---------------------------------------------------------------------------

int32 EmRegsVZ::GetSystemClockFrequency(void) {
    uint16 pllControl = READ_REGISTER(pllControl);
    uint16 pllFreqSel = READ_REGISTER(pllFreqSel);

    // Convert the 32.768KHz clock (CLK32) into the PLLCLK frequency.

    uint16 PC = (pllFreqSel & 0x00FF);
    uint16 QC = (pllFreqSel & 0x0F00) >> 8;

    uint32 result = 32768L * 2 * (14 * (PC + 1) + QC + 1);

    // Divide by the first prescaler, if needed.

    if ((pllControl & hwrVZ328PLLControlPreSc1Div2) != 0) {
        result /= 2;
    }

    // Divide by the second prescaler, if needed.

    if ((pllControl & hwrVZ328PLLControlPreSc2Div2) != 0) {
        result /= 2;
    }

    // Divide by the system clock scaler, if needed.

    switch (pllControl & 0x0F00) {
        case hwrVZ328PLLControlSysDMADiv2:
            result /= 2;
            break;

        case hwrVZ328PLLControlSysDMADiv4:
            result /= 4;
            break;

        case hwrVZ328PLLControlSysDMADiv8:
            result /= 8;
            break;

        case hwrVZ328PLLControlSysDMADiv16:
            result /= 16;
            break;
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::GetAsleep
// ---------------------------------------------------------------------------

Bool EmRegsVZ::GetAsleep(void) {
    return ((READ_REGISTER(pllControl) & hwrVZ328PLLControlDisable) != 0);
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::GetPortInputValue
// ---------------------------------------------------------------------------
// Return the GPIO values for the pins on the port.  These values are used
// if the select pins are high.

uint8 EmRegsVZ::GetPortInputValue(int port) {
    uint8 result = 0;

    if (port == 'D') {
        result = this->GetPortInternalValue(port);
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::GetPortInternalValue
// ---------------------------------------------------------------------------
// Return the dedicated values for the pins on the port.  These values are
// used if the select pins are low.

uint8 EmRegsVZ::GetPortInternalValue(int port) {
    uint8 result = 0;

    if (port == 'D') {
        // Get the INT bits that need to be set.

        result = this->GetKeyBits();
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::PortDataChanged
// ---------------------------------------------------------------------------

void EmRegsVZ::PortDataChanged(int port, uint8, uint8 newValue) {
    if (port == 'D') {
        // Clear the interrupt bits that are having a 1 written to them.
        // Only clear them if they're configured as edge-senstive.

        uint8 portDIntEdge = READ_REGISTER(portDIntEdge);

        PRINTF("EmRegsVZ::PortDataChanged (D): fPortDEdge  = 0x%02lX", (uint32)(uint8)fPortDEdge);
        PRINTF("EmRegsVZ::PortDataChanged (D): portDIntEdge = 0x%02lX",
               (uint32)(uint8)portDIntEdge);
        PRINTF("EmRegsVZ::PortDataChanged (D): newValue     = 0x%02lX", (uint32)(uint8)newValue);

        fPortDEdge &= ~(newValue & portDIntEdge);

        PRINTF("EmRegsVZ::PortDataChanged (D): fPortDEdge  = 0x%02lX", (uint32)(uint8)fPortDEdge);

        // Set the new interrupt state.

        EmRegsVZ::UpdatePortDInterrupts();
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::pllFreqSelRead
// ---------------------------------------------------------------------------

uint32 EmRegsVZ::pllFreqSelRead(emuptr address, int size) {
    // Simulate the rising and falling of the CLK32 signal so that functions
    // like HwrPreRAMInit, HwrShutDownPLL, PrvSetPLL, and PrvShutDownPLL
    // won't hang.

    uint16 pllFreqSel = READ_REGISTER(pllFreqSel) ^ 0x8000;
    WRITE_REGISTER(pllFreqSel, pllFreqSel);

    // Finish up by doing a standard read.

    return EmRegsVZ::StdRead(address, size);
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::portXDataRead
// ---------------------------------------------------------------------------

uint32 EmRegsVZ::portXDataRead(emuptr address, int) {
    // The value read can come from three different places:
    //
    //	- the value what was written to the data register
    //	- any dedicated inputs
    //	- any GPIO inputs
    //
    // The value returned depends on the settings of the SEL and DIR
    // registers.  So let's get those settings, the values from the three
    // input sources, and build up a return value based on those.

    int port = GetPort(address);

    uint8 sel = StdRead(address + 2, 1);
    uint8 dir = StdRead(address - 1, 1);
    uint8 output = StdRead(address + 0, 1);
    uint8 input = EmHAL::GetPortInputValue(port);
    uint8 intFn = EmHAL::GetPortInternalValue(port);

    if (port == 'D') {
        sel |= 0x0F;  // No "select" bit in low nybble, so set for IO values.

        // The system will poll portD 18 times in KeyBootKeys to see
        // if any keys are down.  Wait at least that long before
        // letting up any boot keys maintained by the session.  When we
        // do call ReleaseBootKeys, set our counter to -1 as a flag not
        // to call it any more.

        if (fPortDDataCount != 0xFFFFFFFF && ++fPortDDataCount >= 18 * 2) {
            fPortDDataCount = 0xFFFFFFFF;
            gSession->ReleaseBootKeys();
        }
    }

    // Use the internal chip function bits if the "sel" bits are zero.

    intFn &= ~sel;

    // Otherwise, use the I/O bits.

    output &= sel & dir;  // Use the output bits if the "dir" is one.
    input &= sel & ~dir;  // Use the input bits if the "dir" is zero.

    // Assert that there are no overlaps.

    EmAssert((output & input) == 0);
    EmAssert((output & intFn) == 0);
    EmAssert((input & intFn) == 0);

    // Mush everything together.

    uint8 result = output | input | intFn;

    // If this is port D, flip the bits if the POLARITY register says to.
    // (!!! Does this inversion apply only to input bits?  That is, the
    // bits where the "dir" register has 0 bits?)

    if (0 && port == 'D') {
        uint8 polarity = READ_REGISTER(portDPolarity);
        PRINTF("EmRegsVZ::portXDataRead: polarity = 0x%02lX", (uint32)polarity);
        result ^= polarity;
    }

    PRINTF("EmRegsVZ::port%cDataRead: sel    dir    output input  intFn  result", (char)port);
    PRINTF("EmRegsVZ::port%cDataRead: 0x%02lX   0x%02lX   0x%02lX   0x%02lX   0x%02lX   0x%02lX",
           (char)port, (uint32)sel, (uint32)dir, (uint32)output, (uint32)input, (uint32)intFn,
           (uint32)result);

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::tmr1StatusRead
// ---------------------------------------------------------------------------

uint32 EmRegsVZ::tmr1StatusRead(emuptr address, int size) {
    // Remember this guy for later (see EmRegsVZ::tmr2StatusWrite())

    fLastTmr1Status |= READ_REGISTER(tmr1Status);

    // Finish up by doing a standard read.

    return EmRegsVZ::StdRead(address, size);
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::tmr2StatusRead
// ---------------------------------------------------------------------------

uint32 EmRegsVZ::tmr2StatusRead(emuptr address, int size) {
    // Remember this guy for later (see EmRegsVZ::tmr2StatusWrite())

    fLastTmr2Status |= READ_REGISTER(tmr2Status);

    // Finish up by doing a standard read.

    return EmRegsVZ::StdRead(address, size);
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::uart1Read
// ---------------------------------------------------------------------------

uint32 EmRegsVZ::uart1Read(emuptr address, int size) {
    // If this is a full read, get the next byte from the FIFO.

    Bool refreshRxData = (address == db_addressof(uReceive)) && (size == 2);

    // See if there's anything new ("Put the data on the bus")

    EmRegsVZ::UpdateUARTState(refreshRxData, 0);

    // Finish up by doing a standard read.

    return EmRegsVZ::StdRead(address, size);
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::uart2Read
// ---------------------------------------------------------------------------

uint32 EmRegsVZ::uart2Read(emuptr address, int size) {
    // If this is a full read, get the next byte from the FIFO.

    Bool refreshRxData = (address == db_addressof(u2Receive)) && (size == 2);

    // See if there's anything new ("Put the data on the bus")

    EmRegsVZ::UpdateUARTState(refreshRxData, 1);

    // Finish up by doing a standard read.

    return EmRegsVZ::StdRead(address, size);
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::rtcHourMinSecRead
// ---------------------------------------------------------------------------

uint32 EmRegsVZ::rtcHourMinSecRead(emuptr address, int size) {
    // Get the desktop machine's time.

    uint32 hour, min, sec;
    Platform::GetTime(hour, min, sec);

    // Update the register.

    WRITE_REGISTER(rtcHourMinSec, (hour << hwrVZ328RTCHourMinSecHoursOffset) |
                                      (min << hwrVZ328RTCHourMinSecMinutesOffset) |
                                      (sec << hwrVZ328RTCHourMinSecSecondsOffset));

    // Finish up by doing a standard read.

    return EmRegsVZ::StdRead(address, size);
}

void EmRegsVZ::rtcDayWrite(emuptr address, int size, uint32 value) {
    EmRegsVZ::StdWrite(address, size, value);

    rtcDayAtWrite = Platform::GetMilliseconds() / (3600 * 24 * 1000);
}

void EmRegsVZ::sdctlWrite(emuptr address, int size, uint32 value) {
    EmRegsVZ::StdWrite(address, size, value);

    ApplySdctl();
}

void EmRegsVZ::ApplySdctl() {
    EmAssert(gSession);
    if (!gSession->GetDevice().NeedsSDCTLHack()) return;

    // We emulate just what is required for PalmOS to detect the full 16MB
    // of RAM on the m515. Credits for this go to Cuttlefish and Mu.

    uint16 sdctl = READ_REGISTER(sdramControl);

    gRAMBank_Mask = 0x003fffff;

    if ((sdctl & 0x0c) == 0x08) gRAMBank_Mask |= 0x00800000;
    if ((sdctl & 0x30) == 0x10) gRAMBank_Mask |= 0x00400000;
}

uint32 EmRegsVZ::rtcDayRead(emuptr address, int size) {
    WRITE_REGISTER(rtcDay, (rtcDayAtWrite + Platform::GetMilliseconds() / (1000)) & 0x01ff);

    return EmRegsVZ::StdRead(address, size);
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::csControl1Write
// ---------------------------------------------------------------------------

void EmRegsVZ::csControl1Write(emuptr address, int size, uint32 value) {
    // Get the current value.

    uint16 csControl = READ_REGISTER(csControl1);

    // Do a standard update of the register.

    EmRegsVZ::StdWrite(address, size, value);

    // Check to see if the unprotected memory range changed.

    const uint16 bits = EUPENMask | DSIZ3Mask | DUPS2Mask;

    if ((csControl & bits) != (READ_REGISTER(csControl1) & bits)) {
        EmAssert(gSession);

        gSession->ScheduleResetBanks();
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::csASelectWrite
// ---------------------------------------------------------------------------

void EmRegsVZ::csASelectWrite(emuptr address, int size, uint32 value) {
    // Get the current value.

    uint16 csASelect = READ_REGISTER(csASelect);

    // Do a standard update of the register.

    EmRegsVZ::StdWrite(address, size, value);

    // Check to see if the unprotected memory range changed.

    if ((csASelect & SIZMask) != (READ_REGISTER(csASelect) & SIZMask)) {
        EmAssert(gSession);

        gSession->ScheduleResetBanks();
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::csDSelectWrite
// ---------------------------------------------------------------------------

void EmRegsVZ::csDSelectWrite(emuptr address, int size, uint32 value) {
    // Get the current value.

    uint16 csDSelect = READ_REGISTER(csDSelect);

    // Do a standard update of the register.

    EmRegsVZ::StdWrite(address, size, value);

    // Check its new state and update our ram-protect flag.

    gMemAccessFlags.fProtect_SRAMSet = (READ_REGISTER(csDSelect) & 0x2000) != 0;

    // Check to see if the unprotected memory range changed.

    if ((csDSelect & UPSIZMask) != (READ_REGISTER(csDSelect) & UPSIZMask)) {
        EmAssert(gSession);

        gSession->ScheduleResetBanks();
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::intMaskHiWrite
// ---------------------------------------------------------------------------

void EmRegsVZ::intMaskHiWrite(emuptr address, int size, uint32 value) {
    // Do a standard update of the register.

    EmRegsVZ::StdWrite(address, size, value);

    // Respond to the new interrupt state.

    EmRegsVZ::UpdateInterrupts();
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::intMaskLoWrite
// ---------------------------------------------------------------------------

void EmRegsVZ::intMaskLoWrite(emuptr address, int size, uint32 value) {
    // Do a standard update of the register.

    EmRegsVZ::StdWrite(address, size, value);

    // Respond to the new interrupt state.

    EmRegsVZ::UpdateInterrupts();
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::intStatusHiWrite
// ---------------------------------------------------------------------------

void EmRegsVZ::intStatusHiWrite(emuptr address, int size, uint32 value) {
    // IRQ1, IRQ2, IRQ3, IRQ6 and IRQ7 are cleared by writing to their
    // respective status bits.  We handle those there.  Since there are
    // no interrupt status bits like this in intStatusLo, we don't need
    // a handler for that register; we only handle intStatusHi.

    // Even though this is a 16-bit register as defined by the Palm headers,
    // it's a 32-bit register according to DragonballVZ docs, and is in fact
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

    CLEAR_PENDING_INTERRUPT(hwrVZ328IntCtlEdge1, hwrVZ328IntHiIRQ1);
    CLEAR_PENDING_INTERRUPT(hwrVZ328IntCtlEdge2, hwrVZ328IntHiIRQ2);
    CLEAR_PENDING_INTERRUPT(hwrVZ328IntCtlEdge3, hwrVZ328IntHiIRQ3);
    CLEAR_PENDING_INTERRUPT(hwrVZ328IntCtlEdge6, hwrVZ328IntHiIRQ6);

    // IRQ7 is not edge-programmable, so clear it if we're merely writing to it.
    // !!! Double check this for VZ!

    if (value & hwrVZ328IntHiEMU) {
        intPendingHi &= ~(hwrVZ328IntHiEMU);
    }

    // If we're emulating the user pressing the hotsync button, make sure the
    // interrupt stays asserted.  (!!! Should we use the same technique for
    // other buttons, too?  It doesn't seem to be needed right now, but doing
    // that may more closely mirror the hardware.)

    if (fHotSyncButtonDown) {
        intPendingHi |= hwrVZ328IntHiIRQ1;
    } else {
        intPendingHi &= ~hwrVZ328IntHiIRQ1;
    }

    // This makes the power on key work. If the signal is asserted, the
    // unit will not transition between asleep and awake (cf. HwrSleep, HwrWake).

    intPendingHi &= ~hwrVZ328IntHiIRQ6;

    WRITE_REGISTER(intPendingHi, intPendingHi);
    EmRegsVZ::UpdateInterrupts();
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::portXDataWrite
// ---------------------------------------------------------------------------

void EmRegsVZ::portXDataWrite(emuptr address, int size, uint32 value) {
    // Get the old value before updating it.

    uint8 oldValue = StdRead(address, size);

    // Take a snapshot of the line driver states.

    Bool driverStates[kUARTEnd];
    EmHAL::GetLineDriverStates(driverStates);

    // Take a snapshot of the DTR pin.

    Bool dtrOn = EmHAL::GetDTR(1);

    // Now update the value with a standard write.

    StdWrite(address, size, value);

    // Let anyone know that it's changed.

    int port = GetPort(address);
    PRINTF("EmRegsVZ::port%cDataWrite: oldValue = 0x%02lX", (char)port, (uint32)(uint8)oldValue);
    PRINTF("EmRegsVZ::port%cDataWrite: newValue = 0x%02lX", (char)port, (uint32)(uint8)value);

    EmHAL::PortDataChanged(port, oldValue, value);

    // Respond to any changes in the line driver states.

    EmHAL::CompareLineDriverStates(driverStates);

    // Respond to any change in the DTR pin.

    if (EmHAL::GetDTR(1) != dtrOn) {
        EmHAL::DTRChanged(1);
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::portDIntReqEnWrite
// ---------------------------------------------------------------------------

void EmRegsVZ::portDIntReqEnWrite(emuptr address, int size, uint32 value) {
    // Do a standard update of the register.

    EmRegsVZ::StdWrite(address, size, value);

    // Set the new interrupt state.

    EmRegsVZ::UpdatePortDInterrupts();
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::tmr1StatusWrite
// ---------------------------------------------------------------------------

void EmRegsVZ::tmr1StatusWrite(emuptr address, int size, uint32 value) {
    UNUSED_PARAM(address)
    UNUSED_PARAM(size)

    EmAssert(size == 2);  // This function's a hell of a lot easier to write if
                          // we assume only full-register access.

    // Get the current value.

    uint16 tmr1Status = READ_REGISTER(tmr1Status);

    // If the user had previously read the status bits while they
    // were set, then it's OK for them to be clear now.  Otherwise,
    // we have to merge any set status bits back in.

    tmr1Status &=
        value | ~fLastTmr1Status;  // fLastTmr1Status was set in EmRegsVZ::tmr1StatusRead()

    WRITE_REGISTER(tmr1Status, tmr1Status);

    fLastTmr1Status = 0;
    if ((tmr1Status & hwrVZ328TmrStatusCompare) == 0) {
        uint16 intPendingLo = READ_REGISTER(intPendingLo) & ~hwrVZ328IntLoTimer;
        WRITE_REGISTER(intPendingLo, intPendingLo);

        // Respond to the new interrupt state.

        EmRegsVZ::UpdateInterrupts();
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::tmr2StatusWrite
// ---------------------------------------------------------------------------

void EmRegsVZ::tmr2StatusWrite(emuptr address, int size, uint32 value) {
    UNUSED_PARAM(address)
    UNUSED_PARAM(size)

    EmAssert(size == 2);  // This function's a hell of a lot easier to write if
                          // we assume only full-register access.

    // Get the current value.

    uint16 tmr2Status = READ_REGISTER(tmr2Status);

    // If the user had previously read the status bits while they
    // were set, then it's OK for them to be clear now.  Otherwise,
    // we have to merge any set status bits back in.

    tmr2Status &=
        value | ~fLastTmr2Status;  // fLastTmr2Status was set in EmRegsVZ::tmr2StatusRead()

    WRITE_REGISTER(tmr2Status, tmr2Status);

    fLastTmr2Status = 0;
    if ((tmr2Status & hwrVZ328TmrStatusCompare) == 0) {
        uint16 intPendingLo = READ_REGISTER(intPendingLo) & ~hwrVZ328IntLoTimer2;
        WRITE_REGISTER(intPendingLo, intPendingLo);

        // Respond to the new interrupt state.

        EmRegsVZ::UpdateInterrupts();
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::spiCont1Write
// ---------------------------------------------------------------------------

void EmRegsVZ::spiCont1Write(emuptr address, int size, uint32 value) {
    // Do a standard update of the register.

    EmRegsVZ::StdWrite(address, size, value);

    // Get the current value.

    uint16 spiCont1 = READ_REGISTER(spiCont1);

    // Check to see if data exchange and enable are enabled.

#define BIT_MASK (hwrVZ328SPIMControlExchange | hwrVZ328SPIMControlEnable)
    if ((spiCont1 & BIT_MASK) == BIT_MASK) {
        // Clear the exchange bit.

        spiCont1 &= ~hwrVZ328SPIMControlExchange;

        WRITE_REGISTER(spiCont1, spiCont1);
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::spiMasterControlWrite
// ---------------------------------------------------------------------------

void EmRegsVZ::spiMasterControlWrite(emuptr address, int size, uint32 value) {
    // Do a standard update of the register.

    EmRegsVZ::StdWrite(address, size, value);

    // Get the current value.

    uint16 spiMasterData = READ_REGISTER(spiMasterData);
    uint16 spiMasterControl = READ_REGISTER(spiMasterControl);

    // Check to see if data exchange and enable are enabled.

#define BIT_MASK (hwrVZ328SPIMControlExchange | hwrVZ328SPIMControlEnable)
    if ((spiMasterControl & BIT_MASK) == BIT_MASK) {
        // If the SPI is hooked up to something, talk with it.

        EmSPISlave* spiSlave = this->GetSPISlave();
        if (spiSlave) {
            // Write out the old data, read in the new data.

            uint16 newData = spiSlave->DoExchange(spiMasterControl, spiMasterData);

            // Shift in the new data.

            uint16 numBits = (spiMasterControl & hwrVZ328SPIMControlBitsMask) + 1;

            uint16 oldBitsMask = ~0 << numBits;
            uint16 newBitsMask = ~oldBitsMask;

            spiMasterData = /*((spiMasterData << numBits) & oldBitsMask) | */
                (newData & newBitsMask);

            WRITE_REGISTER(spiMasterData, spiMasterData);
        }

        // Assert the interrupt and clear the exchange bit.

        spiMasterControl |= hwrVZ328SPIMControlIntStatus;
        spiMasterControl &= ~hwrVZ328SPIMControlExchange;

        WRITE_REGISTER(spiMasterControl, spiMasterControl);

        // If hwrVZ328SPIMControlIntEnable is set, trigger an interrupt.

        if ((spiMasterControl & hwrVZ328SPIMControlIntEnable) != 0) {
            uint16 intPendingLo = READ_REGISTER(intPendingLo);
            intPendingLo |= hwrVZ328IntLoSPIM;
            WRITE_REGISTER(intPendingLo, intPendingLo);
            this->UpdateInterrupts();
        }
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::uart1Write
// ---------------------------------------------------------------------------

void EmRegsVZ::uart1Write(emuptr address, int size, uint32 value) {
    // Do a standard update of the register.

    EmRegsVZ::StdWrite(address, size, value);

    // If this write included the TX_DATA field, signal that it needs to
    // be transmitted.

    Bool sendTxData = ((address == db_addressof(uTransmit)) && (size == 2)) ||
                      ((address == db_addressof(uTransmit) + 1) && (size == 1));

    // React to any changes.

    EmRegsVZ::UARTStateChanged(sendTxData, 0);
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::uart2Write
// ---------------------------------------------------------------------------

void EmRegsVZ::uart2Write(emuptr address, int size, uint32 value) {
    // Do a standard update of the register.

    EmRegsVZ::StdWrite(address, size, value);

    // If this write included the TX_DATA field, signal that it needs to
    // be transmitted.

    Bool sendTxData = ((address == db_addressof(u2Transmit)) && (size == 2)) ||
                      ((address == db_addressof(u2Transmit) + 1) && (size == 1));

    // React to any changes.

    EmRegsVZ::UARTStateChanged(sendTxData, 1);
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::lcdRegisterWrite
// ---------------------------------------------------------------------------

void EmRegsVZ::lcdRegisterWrite(emuptr address, int size, uint32 value) {
    switch (address) {
        case db_addressof(lcdStartAddr):
            // Make sure the low-bit is always zero.
            // Make sure bits 31-29 are always zero.
            value &= 0x1FFFFFFE;

            UnmarkScreen();

            break;

        case db_addressof(lcdPageWidth):
        case db_addressof(lcdScreenWidth):
        case db_addressof(lcdScreenHeight):
            UnmarkScreen();

            break;
    }

    EmRegsVZ::StdWrite(address, size, value);

    MarkScreenDirty();
}

void EmRegsVZ::pllRegisterWrite(emuptr address, int size, uint32 value) {
    EmRegsVZ::StdWrite(address, size, value);

    UpdateTimerTicksPerSecond();

    gSystemState.MarkScreenDirty();

    EmHAL::onSystemClockChange.Dispatch();
}

void EmRegsVZ::tmr1RegisterWrite(emuptr address, int size, uint32 value) {
    EmRegsVZ::StdWrite(address, size, value);

    timer1TicksPerSecond = TimerTicksPerSecond(
        READ_REGISTER(tmr1Control), READ_REGISTER(tmr1Prescaler), GetSystemClockFrequency());
}

void EmRegsVZ::tmr2RegisterWrite(emuptr address, int size, uint32 value) {
    EmRegsVZ::StdWrite(address, size, value);

    timer2TicksPerSecond = TimerTicksPerSecond(
        READ_REGISTER(tmr2Control), READ_REGISTER(tmr2Prescaler), GetSystemClockFrequency());
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::rtcControlWrite
// ---------------------------------------------------------------------------

void EmRegsVZ::rtcControlWrite(emuptr address, int size, uint32 value) {
    // Do a standard update of the register.

    EmRegsVZ::StdWrite(address, size, value);

    // Respond to the new interrupt state.

    EmRegsVZ::UpdateRTCInterrupts();
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::rtcIntStatusWrite
// ---------------------------------------------------------------------------

void EmRegsVZ::rtcIntStatusWrite(emuptr address, int size, uint32 value) {
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

    EmRegsVZ::UpdateRTCInterrupts();
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::rtcIntEnableWrite
// ---------------------------------------------------------------------------

void EmRegsVZ::rtcIntEnableWrite(emuptr address, int size, uint32 value) {
    // Do a standard update of the register.

    EmRegsVZ::StdWrite(address, size, value);

    // Respond to the new interrupt state.

    EmRegsVZ::UpdateRTCInterrupts();
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::ButtonEvent
// ---------------------------------------------------------------------------
// Handles a Palm device button event by updating the appropriate registers.

void EmRegsVZ::ButtonEvent(ButtonEventT event) {
    uint16 bitNumber = this->ButtonToBits(event.GetButton());

    // Get the bits that should have been set with the previous set
    // of pressed keys.  We use this old value to update the port D interrupts.

    uint8 oldBits = this->GetKeyBits();

    // Update the set of keys that are currently pressed.

    if (event.GetType() == ButtonEventT::Type::press) {
        fKeyBits |= bitNumber;  // Remember the key bit
    } else {
        fKeyBits &= ~bitNumber;  // Forget the key bit
    }

    // Now get the new set of bits that should be set.

    uint8 newBits = this->GetKeyBits();

    PRINTF("EmRegsVZ::ButtonEvent: fKeyBits = 0x%04lX", (uint32)fKeyBits);
    PRINTF("EmRegsVZ::ButtonEvent: oldBits   = 0x%02lX", (uint32)oldBits);
    PRINTF("EmRegsVZ::ButtonEvent: newBits   = 0x%02lX", (uint32)newBits);

    // Set the interrupt bits for the bits that went from off to on.
    // These get cleared when portDData is written to.

    fPortDEdge |= newBits & ~oldBits;

    PRINTF("EmRegsVZ::ButtonEvent: fPortDEdge = 0x%02lX", (uint32)fPortDEdge);

    // Set the new interrupt state.

    EmRegsVZ::UpdatePortDInterrupts();
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::HotSyncEvent
// ---------------------------------------------------------------------------
// Handles a HotSync button event by updating the appropriate registers.

void EmRegsVZ::HotSyncEvent(Bool buttonIsDown) {
    // If the button changes state, set or clear the HotSync interrupt.

    uint16 intPendingHi = READ_REGISTER(intPendingHi);

    if (buttonIsDown) {
        intPendingHi |= hwrVZ328IntHiIRQ1;
        fHotSyncButtonDown = true;
    } else {
        intPendingHi &= ~hwrVZ328IntHiIRQ1;
        fHotSyncButtonDown = false;
    }

    WRITE_REGISTER(intPendingHi, intPendingHi);

    EmRegsVZ::UpdateInterrupts();
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::GetKeyBits
// ---------------------------------------------------------------------------

uint8 EmRegsVZ::GetKeyBits(void) {
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

    return keyData;
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::ButtonToBits
// ---------------------------------------------------------------------------

uint16 EmRegsVZ::ButtonToBits(ButtonEventT::Button button) {
    switch (button) {
        case ButtonEventT::Button::power:
            return keyBitPower;

        case ButtonEventT::Button::rockerUp:
            return keyBitPageUp;

        case ButtonEventT::Button::rockerDown:
            return keyBitPageDown;

        case ButtonEventT::Button::app1:
            return keyBitHard1;

        case ButtonEventT::Button::app2:
            return keyBitHard2;

        case ButtonEventT::Button::app3:
            return keyBitHard3;

        case ButtonEventT::Button::app4:
            return keyBitHard4;

        case ButtonEventT::Button::cradle:
            return keyBitCradle;

        case ButtonEventT::Button::antenna:
            return keyBitAntenna;

        case ButtonEventT::Button::contrast:
            return keyBitContrast;

        default:
            return 0;
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::GetSPISlave
// ---------------------------------------------------------------------------

EmSPISlave* EmRegsVZ::GetSPISlave(void) { return NULL; }

// ---------------------------------------------------------------------------
//		� EmRegsVZ::UpdateInterrupts
// ---------------------------------------------------------------------------
// Determines whether an interrupt has occurred by copying the Interrupt
// Pending Register to the Interrupt Status Register.

void EmRegsVZ::UpdateInterrupts(void) {
    // Copy the Interrupt Pending Register to the Interrupt Status
    // Register, but ignore interrupts that are being masked.

    // Note: this function is not sensitive to the byte ordering of the registers,
    // so their contents don't need to be accessed via READ_REGISTER or WRITE_REGISTER.

    f68VZ328Regs.intStatusHi = f68VZ328Regs.intPendingHi & ~f68VZ328Regs.intMaskHi;
    f68VZ328Regs.intStatusLo = f68VZ328Regs.intPendingLo & ~f68VZ328Regs.intMaskLo;

    PRINTF("EmRegsVZ::UpdateInterrupts: intMask    = 0x%04lX %04lX", (uint32)f68VZ328Regs.intMaskHi,
           (uint32)f68VZ328Regs.intMaskLo);

    PRINTF("EmRegsVZ::UpdateInterrupts: intPending = 0x%04lX %04lX",
           (uint32)f68VZ328Regs.intPendingHi, (uint32)f68VZ328Regs.intPendingLo);

    // If the Interrupt Status Register isn't clear, flag an interrupt.

    if (f68VZ328Regs.intStatusHi || f68VZ328Regs.intStatusLo) {
        regs.spcflags |= SPCFLAG_INT;

        PRINTF("EmRegsVZ::UpdateInterrupts: intStatus  = 0x%04lX %04lX",
               (uint32)f68VZ328Regs.intStatusHi, (uint32)f68VZ328Regs.intStatusLo);
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::UpdatePortDInterrupts
// ---------------------------------------------------------------------------
// Determine what interrupts need to be generated based on the current
// settings in portDData and fPortDEdge.

void EmRegsVZ::UpdatePortDInterrupts(void) {
    // Update INT0-INT3 of the Interrupt-Pending register (bits 8-11 of the low word).

    // First, get those bits and clear them out.

    uint16 intPendingLo = READ_REGISTER(intPendingLo) & ~hwrVZ328IntLoAllKeys;

    // Initialize the variable to hold the new interrupt settings.

    uint8 newBits = 0;

    // Get some other values we're going to need:

    uint8 portDDir = READ_REGISTER(portDDir);  // Interrupt on inputs only (when pin is low)
    uint8 portDData = EmHAL::GetPortInputValue('D');
    uint8 portDPolarity = READ_REGISTER(portDPolarity);
    uint8 portDIntReqEn = READ_REGISTER(portDIntReqEn);
    uint8 portDKbdIntEn = READ_REGISTER(portDKbdIntEn);
    uint8 portDIntEdge = READ_REGISTER(portDIntEdge);

    // We have a line-level interrupt if:
    //
    //	- line-level interrupts are requested
    //	- the GPIO bit matches the polarity bit

    newBits |= ~portDIntEdge & portDData & portDPolarity;
    newBits |= ~portDIntEdge & ~portDData & ~portDPolarity;

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
	if ((pllControl & hwrVZ328PLLControlDisable) && !(gSession->GetDevice ().EdgeHack ()))
	{
		newBits |= portDIntEdge & fPortDEdge & portDPolarity & 0xF0;
		newBits |= portDIntEdge & 0 & ~portDPolarity & 0xF0;
	}
	else
#endif
    {
        newBits |= portDIntEdge & fPortDEdge & portDPolarity;
        newBits |= portDIntEdge & 0 & ~portDPolarity;
    }

    // Only have interrupts if they're enabled and the pin is configured for input.

    newBits &= portDIntReqEn & ~portDDir;

    PRINTF("EmRegsVZ::UpdatePortDInterrupts: Dir  Data Pol  Req  Edg  PDE  bits");
    PRINTF(
        "EmRegsVZ::UpdatePortDInterrupts: 0x%02lX 0x%02lX 0x%02lX 0x%02lX 0x%02lX 0x%02lX 0x%02lX",
        (uint32)portDDir, (uint32)portDData, (uint32)portDPolarity, (uint32)portDIntReqEn,
        (uint32)portDIntEdge, (uint32)fPortDEdge, (uint32)newBits);

    // Determine if the KB interrupt needs to be asserted.  It is if:
    //
    //	A Port D Data bit is on.
    //	The bit is configured for input (?)
    //	The bit is configured to be OR'd into the interrupt.

    if (!gSession->GetDevice().EdgeHack()) {
        uint8 KB = portDData & ~portDDir & portDKbdIntEn;

        if (KB)
            intPendingLo |= hwrVZ328IntLoKbd;
        else
            intPendingLo &= ~hwrVZ328IntLoKbd;
    }

    // Merge in the new values and write out the result.

    intPendingLo |= (((uint16)newBits) << hwrVZ328IntLoInt0Bit) & hwrVZ328IntLoAllKeys;
    WRITE_REGISTER(intPendingLo, intPendingLo);

    // Respond to the new interrupt state.

    EmRegsVZ::UpdateInterrupts();
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::UpdateRTCInterrupts
// ---------------------------------------------------------------------------
// Determine whether to set or clear the RTC bit in the interrupt pending
// register based on the current RTC register values.

void EmRegsVZ::UpdateRTCInterrupts(void) {
    // See if the RTC is enabled.

    Bool rtcEnabled = (READ_REGISTER(rtcControl) & hwrVZ328RTCControlRTCEnable) != 0;

    // See if there are any RTC events that need to trigger an interrupt.

#define BITS_TO_CHECK                                                                 \
    (hwrVZ328RTCIntEnableSec | hwrVZ328RTCIntEnable24Hr | hwrVZ328RTCIntEnableAlarm | \
     hwrVZ328RTCIntEnableMinute | hwrVZ328RTCIntEnableStopWatch)

    uint16 rtcIntStatus = READ_REGISTER(rtcIntStatus);
    uint16 rtcIntEnable = READ_REGISTER(rtcIntEnable);
    uint16 rtcIntPending = rtcIntStatus & rtcIntEnable & BITS_TO_CHECK;

    Bool havePendingEvents = rtcIntPending != 0;

    // If the RTC is enabled and there are pending events, set the interrupt.
    // Otherwise, clear the interrupt.

    uint16 intPendingLo = READ_REGISTER(intPendingLo);

    if (rtcEnabled && havePendingEvents) {
        intPendingLo |= hwrVZ328IntLoRTC;  // have events, so set interrupt
    } else {
        intPendingLo &= ~hwrVZ328IntLoRTC;  // no events, so clear interrupt
    }

    // Update the interrupt pending register.

    WRITE_REGISTER(intPendingLo, intPendingLo);

    // Respond to the new interrupt state.

    EmRegsVZ::UpdateInterrupts();
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::UARTStateChanged
// ---------------------------------------------------------------------------

void EmRegsVZ::UARTStateChanged(Bool sendTxData, int uartNum) {
    EmUARTDragonball::State state(EmUARTDragonball::kUART_DragonballVZ);

    EmRegsVZ::MarshalUARTState(state, uartNum);

    fUART[uartNum]->StateChanged(state, sendTxData);

    EmRegsVZ::UnmarshalUARTState(state, uartNum);

    EmRegsVZ::UpdateUARTInterrupts(state, uartNum);
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::UpdateUARTState
// ---------------------------------------------------------------------------

void EmRegsVZ::UpdateUARTState(Bool refreshRxData, int uartNum) {
    EmUARTDragonball::State state(EmUARTDragonball::kUART_DragonballVZ);

    EmRegsVZ::MarshalUARTState(state, uartNum);

    fUART[uartNum]->UpdateState(state, refreshRxData);

    EmRegsVZ::UnmarshalUARTState(state, uartNum);

    EmRegsVZ::UpdateUARTInterrupts(state, uartNum);
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::UpdateUARTInterrupts
// ---------------------------------------------------------------------------

void EmRegsVZ::UpdateUARTInterrupts(const EmUARTDragonball::State& state, int uartNum) {
    // Generate the appropriate interrupts.

    uint16 whichBit = uartNum == 0 ? hwrVZ328IntLoUART : hwrVZ328IntLoUART2;

#if 0
	LogAppendMsg ("UpdateUARTInterrupts for UART %d.", uartNum + 1);

	if (uartNum == 1)
	{
		LogAppendMsg ("RX_FIFO_FULL:    %s   RX_FIFO_HALF:   %s   DATA_READY:      %s",
			state.RX_FIFO_FULL ? "ON " : "off",
			state.RX_FIFO_HALF ? "ON " : "off",
			state.DATA_READY ? "ON " : "off");

		LogAppendMsg ("RX_FULL_ENABLE:  %s   RX_HALF_ENABLE: %s   RX_RDY_ENABLE:   %s",
			state.RX_FULL_ENABLE ? "ON " : "off",
			state.RX_HALF_ENABLE ? "ON " : "off",
			state.RX_RDY_ENABLE ? "ON " : "off");

		LogAppendMsg ("TX_FIFO_EMPTY:   %s   TX_FIFO_HALF:   %s   TX_AVAIL:        %s",
			state.TX_FIFO_EMPTY ? "ON " : "off",
			state.TX_FIFO_HALF ? "ON " : "off",
			state.TX_AVAIL ? "ON " : "off");

		LogAppendMsg ("TX_EMPTY_ENABLE: %s   TX_HALF_ENABLE: %s   TX_AVAIL_ENABLE: %s",
			state.TX_EMPTY_ENABLE ? "ON " : "off",
			state.TX_HALF_ENABLE ? "ON " : "off",
			state.TX_AVAIL_ENABLE ? "ON " : "off");
	}
#endif

    if ((state.RX_FULL_ENABLE && state.RX_FIFO_FULL) ||
        (state.RX_HALF_ENABLE && state.RX_FIFO_HALF) || (state.RX_RDY_ENABLE && state.DATA_READY) ||
        (state.TX_EMPTY_ENABLE && state.TX_FIFO_EMPTY) ||
        (state.TX_HALF_ENABLE && state.TX_FIFO_HALF) || (state.TX_AVAIL_ENABLE && state.TX_AVAIL)) {
        // Set the UART interrupt.

        WRITE_REGISTER(intPendingLo, READ_REGISTER(intPendingLo) | whichBit);
#if 0
		if (uartNum == 1)
		{
			LogAppendMsg ("Setting UART %d interrupt.", uartNum + 1);
		}
#endif
    } else {
        // Clear the UART interrupt.

        WRITE_REGISTER(intPendingLo, READ_REGISTER(intPendingLo) & ~whichBit);
#if 0
		if (uartNum == 1)
		{
			LogAppendMsg ("Clearing UART %d interrupt.", uartNum + 1);
		}
#endif
    }

    // Respond to the new interrupt state.

    EmRegsVZ::UpdateInterrupts();

#if 0
	if (uartNum == 1)
	{
		LogAppendMsg ("intPending  = 0x%04lX %04lX",
				(uint32) f68VZ328Regs.intPendingHi,
				(uint32) f68VZ328Regs.intPendingLo);

		LogAppendMsg ("intMask     = 0x%04lX %04lX",
				(uint32) f68VZ328Regs.intMaskHi,
				(uint32) f68VZ328Regs.intMaskLo);

		LogAppendMsg ("intStatus   = 0x%04lX %04lX",
				(uint32) f68VZ328Regs.intStatusHi,
				(uint32) f68VZ328Regs.intStatusLo);
	}
#endif
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::MarshalUARTState
// ---------------------------------------------------------------------------

void EmRegsVZ::MarshalUARTState(EmUARTDragonball::State& state, int uartNum) {
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
        uLevel = 0;
    } else {
        uControl = READ_REGISTER(u2Control);
        uBaud = READ_REGISTER(u2Baud);
        uReceive = READ_REGISTER(u2Receive);
        uTransmit = READ_REGISTER(u2Transmit);
        uMisc = READ_REGISTER(u2Misc);
        uLevel = READ_REGISTER(u2FIFOHMark);
    }

    state.UART_ENABLE = (uControl & hwrVZ328UControlUARTEnable) != 0;
    state.RX_ENABLE = (uControl & hwrVZ328UControlRxEnable) != 0;
    state.TX_ENABLE = (uControl & hwrVZ328UControlTxEnable) != 0;
    state.RX_CLK_CONT = (uControl & hwrVZ328UControlRxClock1xSync) != 0;
    state.PARITY_EN = (uControl & hwrVZ328UControlParityEn) != 0;
    state.ODD_EVEN = (uControl & hwrVZ328UControlParityOdd) != 0;
    state.STOP_BITS = (uControl & hwrVZ328UControlStopBits2) != 0;
    state.CHAR8_7 = (uControl & hwrVZ328UControlDataBits8) != 0;
    //	state.GPIO_DELTA_ENABLE	= (uControl & hwr328UControlGPIODeltaEn) != 0;	// 68328 only
    state.OLD_ENABLE = (uControl & hwrVZ328UControlOldDataEn) != 0;  // 68VZ328 only
    state.CTS_DELTA_ENABLE = (uControl & hwrVZ328UControlCTSDeltaEn) != 0;
    state.RX_FULL_ENABLE = (uControl & hwrVZ328UControlRxFullEn) != 0;
    state.RX_HALF_ENABLE = (uControl & hwrVZ328UControlRxHalfEn) != 0;
    state.RX_RDY_ENABLE = (uControl & hwrVZ328UControlRxRdyEn) != 0;
    state.TX_EMPTY_ENABLE = (uControl & hwrVZ328UControlTxEmptyEn) != 0;
    state.TX_HALF_ENABLE = (uControl & hwrVZ328UControlTxHalfEn) != 0;
    state.TX_AVAIL_ENABLE = (uControl & hwrVZ328UControlTxAvailEn) != 0;

    // Baud control register bits
    // These are all values the user sets; we just look at them.

    //	state.GPIO_DELTA		= (uBaud & hwr328UBaudGPIODelta) != 0;			//
    // 68328 only 	state.GPIO				= (uBaud & hwr328UBaudGPIOData) !=
    // 0;
    //// 68328 only
    //	state.GPIO_DIR			= (uBaud & hwr328UBaudGPIODirOut) != 0;			//
    // 68328 only 	state.GPIO_SRC			= (uBaud & hwrVZ328UBaudGPIOSrcBaudGen) !=
    // 0;
    // // 68328 only
    state.UCLK_DIR = (uBaud & hwrVZ328UBaudUCLKDirOut) != 0;  // 68VZ328 only
    state.BAUD_SRC = (uBaud & hwrVZ328UBaudBaudSrcUCLK) != 0;
    state.DIVIDE = (uBaud & hwrVZ328UBaudDivider) >> hwrVZ328UBaudDivideBitOffset;
    state.PRESCALER = (uBaud & hwrVZ328UBaudPrescaler);

    // Receive register bits
    // These are all input bits; we set them, not the user.

    state.RX_FIFO_FULL = (uReceive & hwrVZ328UReceiveFIFOFull) != 0;
    state.RX_FIFO_HALF = (uReceive & hwrVZ328UReceiveFIFOHalf) != 0;
    state.DATA_READY = (uReceive & hwrVZ328UReceiveDataRdy) != 0;
    state.OLD_DATA = (uReceive & hwrVZ328UReceiveOldData) != 0;  // 68VZ328 only
    state.OVRUN = (uReceive & hwrVZ328UReceiveOverrunErr) != 0;
    state.FRAME_ERROR = (uReceive & hwrVZ328UReceiveFrameErr) != 0;
    state.BREAK = (uReceive & hwrVZ328UReceiveBreakErr) != 0;
    state.PARITY_ERROR = (uReceive & hwrVZ328UReceiveParityErr) != 0;
    state.RX_DATA = (uReceive & hwrVZ328UReceiveData);

    // Transmitter register bits
    // We set everything except TX_DATA; the user sets that
    // value and ONLY that value.

    state.TX_FIFO_EMPTY = (uTransmit & hwrVZ328UTransmitFIFOEmpty) != 0;
    state.TX_FIFO_HALF = (uTransmit & hwrVZ328UTransmitFIFOHalf) != 0;
    state.TX_AVAIL = (uTransmit & hwrVZ328UTransmitTxAvail) != 0;
    state.SEND_BREAK = (uTransmit & hwrVZ328UTransmitSendBreak) != 0;
    state.IGNORE_CTS = (uTransmit & hwrVZ328UTransmitIgnoreCTS) != 0;
    state.BUSY = (uTransmit & hwrVZ328UTransmitBusy) != 0;  // 68VZ328 only
    state.CTS_STATUS = (uTransmit & hwrVZ328UTransmitCTSStatus) != 0;
    state.CTS_DELTA = (uTransmit & hwrVZ328UTransmitCTSDelta) != 0;
    state.TX_DATA = (uTransmit & hwrVZ328UTransmitData);

    // Misc register bits
    // These are all values the user sets; we just look at them.

    state.BAUD_TEST = (uMisc & hwrVZ328UMiscBaudTest) != 0;  // 68VZ328 only
    state.CLK_SRC = (uMisc & hwrVZ328UMiscClkSrcUCLK) != 0;
    state.FORCE_PERR = (uMisc & hwrVZ328UMiscForceParityErr) != 0;
    state.LOOP = (uMisc & hwrVZ328UMiscLoopback) != 0;
    state.BAUD_RESET = (uMisc & hwrVZ328UMiscBaudReset) != 0;  // 68VZ328 only
    state.IR_TEST = (uMisc & hwrVZ328UMiscIRTestEn) != 0;      // 68VZ328 only
    state.RTS_CONT = (uMisc & hwrVZ328UMiscRTSThruFIFO) != 0;
    state.RTS = (uMisc & hwrVZ328UMiscRTSOut) != 0;
    state.IRDA_ENABLE = (uMisc & hwrVZ328UMiscIRDAEn) != 0;
    state.IRDA_LOOP = (uMisc & hwrVZ328UMiscLoopIRDA) != 0;
    state.RX_POL = (uMisc & hwrVZ328UMiscRXPolarityInv) != 0;  // 68VZ328 only
    state.TX_POL = (uMisc & hwrVZ328UMiscTXPolarityInv) != 0;  // 68VZ328 only

    // Level Marker Interrupt

    state.TXFIFO_LEVEL_MARKER = ((uLevel >> 8) & 0x0F);  // 68VZ328 only
    state.RXFIFO_LEVEL_MARKER = ((uLevel >> 0) & 0x0F);  // 68VZ328 only
}

// ---------------------------------------------------------------------------
//		� EmRegsVZ::UnmarshalUARTState
// ---------------------------------------------------------------------------

void EmRegsVZ::UnmarshalUARTState(const EmUARTDragonball::State& state, int uartNum) {
    uint16 uControl = 0;
    uint16 uBaud = 0;
    uint16 uReceive = 0;
    uint16 uTransmit = 0;
    uint16 uMisc = 0;
    uint16 uLevel = 0;

    if (state.UART_ENABLE) uControl |= hwrVZ328UControlUARTEnable;
    if (state.RX_ENABLE) uControl |= hwrVZ328UControlRxEnable;
    if (state.TX_ENABLE) uControl |= hwrVZ328UControlTxEnable;
    if (state.RX_CLK_CONT) uControl |= hwrVZ328UControlRxClock1xSync;
    if (state.PARITY_EN) uControl |= hwrVZ328UControlParityEn;
    if (state.ODD_EVEN) uControl |= hwrVZ328UControlParityOdd;
    if (state.STOP_BITS) uControl |= hwrVZ328UControlStopBits2;
    if (state.CHAR8_7) uControl |= hwrVZ328UControlDataBits8;
    //	if (state.GPIO_DELTA_ENABLE)uControl |= hwr328UControlGPIODeltaEn;	// 68328 only
    if (state.OLD_ENABLE) uControl |= hwrVZ328UControlOldDataEn;  // 68VZ328 only
    if (state.CTS_DELTA_ENABLE) uControl |= hwrVZ328UControlCTSDeltaEn;
    if (state.RX_FULL_ENABLE) uControl |= hwrVZ328UControlRxFullEn;
    if (state.RX_HALF_ENABLE) uControl |= hwrVZ328UControlRxHalfEn;
    if (state.RX_RDY_ENABLE) uControl |= hwrVZ328UControlRxRdyEn;
    if (state.TX_EMPTY_ENABLE) uControl |= hwrVZ328UControlTxEmptyEn;
    if (state.TX_HALF_ENABLE) uControl |= hwrVZ328UControlTxHalfEn;
    if (state.TX_AVAIL_ENABLE) uControl |= hwrVZ328UControlTxAvailEn;

    // Baud control register bits
    // These are all values the user sets; we just look at them.

    //	if (state.GPIO_DELTA)		uBaud |= hwr328UBaudGPIODelta;		// 68328 only
    //	if (state.GPIO)				uBaud |= hwr328UBaudGPIOData;		// 68328
    // only 	if (state.GPIO_DIR)			uBaud |= hwr328UBaudGPIODirOut;		//
    // 68328
    // only 	if (state.GPIO_SRC)			uBaud |= hwr328UBaudGPIOSrcBaudGen;	//
    // 68328 only
    if (state.UCLK_DIR) uBaud |= hwrVZ328UBaudUCLKDirOut;  // 68VZ328 only
    if (state.BAUD_SRC) uBaud |= hwrVZ328UBaudBaudSrcUCLK;

    uBaud |= (state.DIVIDE << hwrVZ328UBaudDivideBitOffset) & hwrVZ328UBaudDivider;
    uBaud |= (state.PRESCALER) & hwrVZ328UBaudPrescaler;

    // Receive register bits
    // These are all input bits; we set them, not the user.

    if (state.RX_FIFO_FULL) uReceive |= hwrVZ328UReceiveFIFOFull;
    if (state.RX_FIFO_HALF) uReceive |= hwrVZ328UReceiveFIFOHalf;
    if (state.DATA_READY) uReceive |= hwrVZ328UReceiveDataRdy;
    if (state.OLD_DATA) uReceive |= hwrVZ328UReceiveOldData;  // 68VZ328 only
    if (state.OVRUN) uReceive |= hwrVZ328UReceiveOverrunErr;
    if (state.FRAME_ERROR) uReceive |= hwrVZ328UReceiveFrameErr;
    if (state.BREAK) uReceive |= hwrVZ328UReceiveBreakErr;
    if (state.PARITY_ERROR) uReceive |= hwrVZ328UReceiveParityErr;

    uReceive |= (state.RX_DATA) & hwrVZ328UReceiveData;

    // Transmitter register bits
    // We set everything except TX_DATA; the user sets that
    // value and ONLY that value.

    if (state.TX_FIFO_EMPTY) uTransmit |= hwrVZ328UTransmitFIFOEmpty;
    if (state.TX_FIFO_HALF) uTransmit |= hwrVZ328UTransmitFIFOHalf;
    if (state.TX_AVAIL) uTransmit |= hwrVZ328UTransmitTxAvail;
    if (state.SEND_BREAK) uTransmit |= hwrVZ328UTransmitSendBreak;
    if (state.IGNORE_CTS) uTransmit |= hwrVZ328UTransmitIgnoreCTS;
    if (state.BUSY) uTransmit |= hwrVZ328UTransmitBusy;  // 68VZ328 only
    if (state.CTS_STATUS) uTransmit |= hwrVZ328UTransmitCTSStatus;
    if (state.CTS_DELTA) uTransmit |= hwrVZ328UTransmitCTSDelta;

    uTransmit |= (state.TX_DATA) & hwrVZ328UTransmitData;

    // Misc register bits
    // These are all values the user sets; we just look at them.

    if (state.BAUD_TEST) uMisc |= hwrVZ328UMiscBaudTest;  // 68VZ328 only
    if (state.CLK_SRC) uMisc |= hwrVZ328UMiscClkSrcUCLK;
    if (state.FORCE_PERR) uMisc |= hwrVZ328UMiscForceParityErr;
    if (state.LOOP) uMisc |= hwrVZ328UMiscLoopback;
    if (state.BAUD_RESET) uMisc |= hwrVZ328UMiscBaudReset;  // 68VZ328 only
    if (state.IR_TEST) uMisc |= hwrVZ328UMiscIRTestEn;      // 68VZ328 only
    if (state.RTS_CONT) uMisc |= hwrVZ328UMiscRTSThruFIFO;
    if (state.RTS) uMisc |= hwrVZ328UMiscRTSOut;
    if (state.IRDA_ENABLE) uMisc |= hwrVZ328UMiscIRDAEn;
    if (state.IRDA_LOOP) uMisc |= hwrVZ328UMiscLoopIRDA;
    if (state.RX_POL) uMisc |= hwrVZ328UMiscRXPolarityInv;  // 68VZ328 only
    if (state.TX_POL) uMisc |= hwrVZ328UMiscTXPolarityInv;  // 68VZ328 only

    // Level Marker Interrupt

    uLevel |= (state.TXFIFO_LEVEL_MARKER) << 8;
    uLevel |= (state.RXFIFO_LEVEL_MARKER) << 0;

    if (uartNum == 0) {
        WRITE_REGISTER(uControl, uControl);
        WRITE_REGISTER(uBaud, uBaud);
        WRITE_REGISTER(uReceive, uReceive);
        WRITE_REGISTER(uTransmit, uTransmit);
        WRITE_REGISTER(uMisc, uMisc);
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
//		� EmRegsVZ::GetPort
// ---------------------------------------------------------------------------
// Given an address, return a value indicating what port it is associated with.

int EmRegsVZ::GetPort(emuptr address) {
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
    }

    EmAssert(false);
    return 0;
}

uint32 EmRegsVZ::Tmr1CyclesToNextInterrupt(uint64 systemCycles) {
    if ((READ_REGISTER(intMaskLo) & hwrVZ328IntLoTimer) ||
        !(READ_REGISTER(tmr1Control) & hwrVZ328TmrControlEnable) || timer1TicksPerSecond <= 0)
        return 0xffffffff;

    uint16 tcmp = READ_REGISTER(tmr1Compare);
    uint16 tcn = READ_REGISTER(tmr1Counter);
    uint16 delta = tcmp - tcn;

    double clocksPerSecond = gSession->GetClocksPerSecond();

    uint32 cycles = ceil((double)delta / timer1TicksPerSecond * clocksPerSecond);

    while ((uint32)(((double)(cycles + systemCycles) - tmr1LastProcessedSystemCycles) /
                    clocksPerSecond * timer1TicksPerSecond) < delta)
        cycles++;

    return cycles;
}

uint32 EmRegsVZ::Tmr2CyclesToNextInterrupt(uint64 systemCycles) {
    if ((READ_REGISTER(intMaskLo) & hwrVZ328IntLoTimer2) ||
        !(READ_REGISTER(tmr2Control) & hwrVZ328TmrControlEnable) || timer2TicksPerSecond <= 0)
        return 0xffffffff;

    uint16 tcmp = READ_REGISTER(tmr2Compare);
    uint16 tcn = READ_REGISTER(tmr2Counter);
    uint16 delta = tcmp - tcn;

    double clocksPerSecond = gSession->GetClocksPerSecond();

    uint32 cycles = ceil((double)delta / timer2TicksPerSecond * clocksPerSecond);

    while ((uint32)(((double)(cycles + systemCycles) - tmr2LastProcessedSystemCycles) /
                    clocksPerSecond * timer2TicksPerSecond) < delta)
        cycles++;

    return cycles;
}

uint32 EmRegsVZ::CyclesToNextInterrupt(uint64 systemCycles) {
    return min(Tmr1CyclesToNextInterrupt(systemCycles), Tmr2CyclesToNextInterrupt(systemCycles));
}

void EmRegsVZ::pwmc1Write(emuptr address, int size, uint32 value) {
    EmRegsVZ::StdWrite(address, size, value);

    if (pwmActive && !(value & 0x10)) {
        pwmActive = false;
        DispatchPwmChange();
    }
}

void EmRegsVZ::pwms1Write(emuptr address, int size, uint32 value) {
    EmRegsVZ::StdWrite(address, size, value);

    if (READ_REGISTER(pwmControl) & 0x10) {
        pwmActive = true;
        DispatchPwmChange();
    }
}

void EmRegsVZ::pwmp1Write(emuptr address, int size, uint32 value) {
    EmRegsVZ::StdWrite(address, size, value);

    if (pwmActive) DispatchPwmChange();
}

void EmRegsVZ::DispatchPwmChange() {
    uint16 pwmc1 = READ_REGISTER(pwmControl);
    uint8 pwms1 = READ_REGISTER(pwmSampleLo);
    uint8 pwmp1 = READ_REGISTER(pwmPeriod);

    if (!pwmActive) {
        EmHAL::onPwmChange.Dispatch(-1, -1);

        return;
    }

    uint8 prescaler = (pwmc1 >> 8) & 0x7f;
    uint8 clksel = pwmc1 & 0x03;
    uint32 baseFreq = (pwmc1 & 0x8000) ? 32768 : GetSystemClockFrequency();

    double freq = static_cast<double>(baseFreq) / (prescaler + 1) / (2 << clksel) /
                  min(256u, static_cast<uint32>(pwmp1) + 2);

    double dutyCycle = static_cast<double>(pwms1) / pwmp1;

    if (freq <= 20000) EmHAL::onPwmChange.Dispatch(freq, dutyCycle);
}

bool EmRegsVZNoScreen::CopyLCDFrame(Frame& frame) { return EmHALHandler::CopyLCDFrame(frame); }

uint16 EmRegsVZNoScreen::GetLCD2bitMapping() { return EmHALHandler::GetLCD2bitMapping(); }

void EmRegsVZNoScreen::MarkScreen() { return; }

void EmRegsVZNoScreen::UnmarkScreen() { return; }

void EmRegsVZNoScreen::MarkScreenDirty() { return; }
