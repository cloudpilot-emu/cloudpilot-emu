/******************************************************************************
 *
 * Copyright (c) 1995-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: M68EZ328Hwr.h
 *
 * Description:
 *		Hardware Equates for the Motorola 68EZ328 Dragonball-EZ chip 
 *
 * History:
 *   	11/13/97  srj	Created from M68328Hwr.h
 *		 3/30/99	 jrb	Added JerryEZ pld defines
 *		 4/ 5/99  jrb	Added JerryEZ pld shadow register bit defines
 *		 5/31/99  SCL	Moved hwr68328Base into this file from <HardwarePrv.h>
 *
 *****************************************************************************/

#ifdef	NON_PORTABLE						// So apps don't include this...


#ifndef	__M68EZ328HWR_H__
#define	__M68EZ328HWR_H__


#include <PalmTypes.h>							// Touchdown includes


//  Define location of the 68328 Dragonball chip
#define	hwr68328Base		0xFFFFF000L					// Base address of 68328 


/***********************************************************************
 * Motorola 68328  registers
 ***********************************************************************/
struct HwrM68EZ328Type {

	UInt8		scr;								// $000: System Control Register
	UInt8											___filler0[0x004-0x001];
	UInt8		chipID;							// $004: Chip ID Register
	UInt8		maskID;							// $005: Mask ID Register
	UInt16	swID;								// $006: Software ID Register
	UInt8											___filler1[0x100-0x008];				 
	
	UInt16	csAGroupBase;					// $100: Chip Select Group A Base Register
	UInt16	csBGroupBase;					// $102: Chip Select Group B Base Register
	UInt16	csCGroupBase;					// $104: Chip Select Group C Base Register
	UInt16	csDGroupBase;					// $106: Chip Select Group D Base Register

	UInt8											___filler6[0x110-0x108];

	UInt16	csASelect;						// $110: Group A Chip Select Register
	UInt16	csBSelect;						// $112: Group B Chip Select Register
	UInt16	csCSelect;						// $114: Group C Chip Select Register
	UInt16	csDSelect;						// $116: Group D Chip Select Register
	
	UInt16	emuCS;						   // $118: EMU Chip Select Register
	
	UInt8											___filler2[0x200-0x11A];		
	
	UInt16	pllControl;						// $200: PLL Control Register
	UInt16	pllFreqSel;						// $202: PLL Frequency Select Register
	UInt16	pllTest;							// $204: PLL Test Register (do not access)
	UInt8											___filler44;
	UInt8		pwrControl;						// $207: Power Control Register
	
	UInt8											___filler3[0x300-0x208];
	
	UInt8		intVector;						// $300: Interrupt Vector Register
	UInt8											___filler4;
	UInt16	intControl;						// $302: Interrupt Control Register
	UInt16	intMaskHi;						// $304: Interrupt Mask Register/HIGH word
	UInt16	intMaskLo;						// $306: Interrupt Mask Register/LOW word
	UInt8											___filler7[0x30c-0x308];
	UInt16	intStatusHi;					// $30C: Interrupt Status Register/HIGH word
	UInt16	intStatusLo;					// $30E: Interrupt Status Register/LOW word
	UInt16	intPendingHi;					// $310: Interrupt Pending Register
	UInt16	intPendingLo;					// $312: Interrupt Pending Register
	
	UInt8 											___filler4a[0x400-0x314];
	
	UInt8		portADir;						// $400: Port A Direction Register
	UInt8		portAData;						// $401: Port A Data Register
	UInt8		portAPullupEn;					// $402: Port A Pullup Enable (similar to Select on DB)
	UInt8											___filler8[5];	
	
	UInt8		portBDir;						// $408: Port B Direction Register
	UInt8		portBData;						// $409: Port B Data Register
	UInt8		portBPullupEn;					// $40A: Port B Pullup Enable
	UInt8		portBSelect;					// $40B: Port B Select Register	
	
	UInt8											___filler9[4];
	
	UInt8		portCDir;						// $410: Port C Direction Register
	UInt8		portCData;						// $411: Port C Data Register
	UInt8		portCPulldnEn;					// $412: Port C Pulldown Enable
	UInt8		portCSelect;					// $413: Port C Select Register	
	
	UInt8											___filler10[4];
	
	UInt8		portDDir;						// $418: Port D Direction Register
	UInt8		portDData;						// $419: Port D Data Register
	UInt8		portDPullupEn;					// $41A: Port D Pull-up Enable
	UInt8		portDSelect;					// $41B: Port D Select Register
	UInt8		portDPolarity;					// $41C: Port D Polarity Register
	UInt8		portDIntReqEn;					// $41D: Port D Interrupt Request Enable
	UInt8		portDKbdIntEn;					// $41E: Port D Keyboard Interrupt Enable
	UInt8		portDIntEdge;					// $41F: Port D IRQ Edge Register
	
	UInt8		portEDir;						// $420: Port E Direction Register
	UInt8		portEData;						// $421: Port E Data Register
	UInt8		portEPullupEn;					// $422: Port E Pull-up Enable
	UInt8		portESelect;					// $423: Port E Select Register
	
	UInt8											___filler14[4];
	
	UInt8		portFDir;						// $428: Port F Direction Register
	UInt8		portFData;						// $429: Port F Data Register
	UInt8		portFPullupdnEn;				// $42A: Port F Pull-up/down Enable
	UInt8		portFSelect;					// $42B: Port F Select Register
	
	UInt8											___filler16[4];
		
	UInt8		portGDir;						// $430: Port G Direction Register
	UInt8		portGData;						// $431: Port G Data Register
	UInt8		portGPullupEn;					// $432: Port G Pull-up Enable
	UInt8		portGSelect;					// $433: Port G Select Register
	
	UInt8											___filler2000[0x500-0x434];

	UInt16	pwmControl;						// $500: PWM Control Register
	UInt8		pwmSampleHi;    				// $502: PWM Sample - high byte
	UInt8		pwmSampleLo;					// $503: PWM Sample - low byte
	UInt8		pwmPeriod;						// $504: PWM Period
	UInt8		pwmCounter;						// $505: PWM Counter
	
	UInt8											___filler24[0x600-0x506];
	
	UInt16	tmr1Control;					// $600: Timer 1 Control Register
	UInt16	tmr1Prescaler;					// $602: Timer 1 Prescaler Register
	UInt16	tmr1Compare;					// $604: Timer 1 Compare Register
	UInt16	tmr1Capture;					// $606: Timer 1 Capture Register
	UInt16	tmr1Counter;					// $608: Timer 1 Counter Register
	UInt16	tmr1Status;						// $60A: Timer 1 Status Register
	
	UInt8											___filler25[0x800-0x60C];
		
	UInt16	spiMasterData;					// $800: SPI Master Data Register
	UInt16	spiMasterControl;				// $802: SPI Master Control Register
	
	UInt8											___filler27[0x900-0x804];
	
	UInt16	uControl;						// $900: Uart Control Register
	UInt16	uBaud;							// $902: Uart Baud Control Register
	UInt16	uReceive;						// $904: Uart Receive Register
	UInt16	uTransmit;						// $906: Uart Transmit Register
	UInt16	uMisc;							// $908: Uart Miscellaneous Register
	UInt16	uNonIntPresc;					// $90A: Uart IRDA Non-Integer Prescaler
	
	UInt8											___filler28[0xA00-0x90C];
	
	UInt32	lcdStartAddr;					// $A00: Screen Starting Address Register
	UInt8											___filler29;
	UInt8		lcdPageWidth;					// $A05: Virtual Page Width Register
	UInt8											___filler30[2];
	UInt16	lcdScreenWidth;				// $A08: Screen Width Register
	UInt16	lcdScreenHeight;				// $A0A: Screen Height Register
	UInt8											___filler31[0xA18-0xA0C];
	UInt16	lcdCursorXPos;					// $A18: Cursor X Position
	UInt16	lcdCursorYPos;					// $A1A:	Cursor Y Position
	UInt16	lcdCursorWidthHeight;		// $A1C: Cursor Width and Height
	UInt8											___filler32;
	UInt8		lcdBlinkControl;				// $A1F: Blink Control Register
	UInt8		lcdPanelControl;				// $A20: Panel Interface Control Register
	UInt8		lcdPolarity;					// $A21: Polarity Config Register
	UInt8											___filler33;						
	UInt8		lcdACDRate;						// $A23: ACD (M) Rate Control Register
	UInt8											___filler34;
	UInt8		lcdPixelClock;					// $A25: Pixel Clock Divider Register
	UInt8											___filler35;
	UInt8		lcdClockControl;				// $A27: Clocking Control Register
	UInt8											___filler36;
	UInt8		lcdRefreshRateAdj;			// $A29: Refresh Rate Adjustment Register
	UInt8											___filler2003[0xA2D-0xA2A];
	UInt8    lcdPanningOffset;				// $A2D: Panning Offset Register
	
	UInt8											___filler37[0xA31-0xA2E];
	
	UInt8		lcdFrameRate;					// $A31: Frame Rate Control Modulation Register
	UInt8											___filler2004;
	UInt8		lcdGrayPalette;				// $A33: Gray Palette Mapping Register
	UInt8		lcdReserved;					// $A34: Reserved
	UInt8											___filler2005;
	UInt16	lcdContrastControlPWM;		// $A36: Contrast Control
	
	UInt8											___filler40[0xB00-0xA38];
	
	UInt32	rtcHourMinSec;					// $B00: RTC Hours, Minutes, Seconds Register
	UInt32	rtcAlarm;						// $B04: RTC Alarm Register
	UInt8											___filler2001[0xB0A-0xB08];
	UInt16   rtcWatchDog;               // $B0A: RTC Watchdog Timer
	UInt16	rtcControl;						// $B0C: RTC Control Register
	UInt16	rtcIntStatus;					// $B0E: RTC Interrupt Status Register
	UInt16	rtcIntEnable;					// $B10: RTC Interrupt Enable Register
	UInt16	stopWatch;						// $B12: Stopwatch Minutes
	UInt8											___filler2002[0xB1A-0xB14];
	UInt16	rtcDay;							// $B1A: RTC Day
	UInt16   rtcDayAlarm;					// $B1C: RTC Day Alarm
	
   UInt8											___filler41[0xC00-0xB1E];

   UInt16	dramConfig;                // $C00: DRAM Memory Config Register
   UInt16   dramControl;               // $C02: DRAM Control Register

   UInt8                                ___filler42[0xD00-0xC04];
	
	UInt32	emuAddrCompare;				// $D00: Emulation Address Compare Register   
	UInt32	emuAddrMask;					// $D04: Emulation Address Mask Register
	UInt16	emuControlCompare;			// $D08: Emulation Control Compare Register
	UInt16	emuControlMask;				// $D0A: Emulation Control Mask Register
	UInt16	emuControl;						// $DOC: Emulation Control Register
	UInt16	emuStatus;						// $D0E: Emulation Status Register
};

typedef volatile struct HwrM68EZ328Type*	HwrM68EZ328Ptr;

#define HwrDBallType HwrM68EZ328Type
#define HwrDBallPtr HwrM68EZ328Ptr


/************************************************************************
 * Jerry EZ cost reduction PLD I/O register
 ***********************************************************************/
typedef struct HwrJerryPLDType {

	UInt8 dspOn;			// pld output turn dsp on/off
	UInt8 unused1;
	UInt8	chargeOn;		// pld output turn charger on/off
	UInt8	unused2;
	UInt8 refOn;			// pld output turn 3.0 volt reference on/off
	UInt8	unused3;
	UInt8	pipaBiasEnable;	// pld output turn PA bias on/off
	UInt8	unused4;
	UInt8	dspReset;		// pld output control dsp reset pin up/down
	UInt8	unused5;
	UInt8	ezToDsl;			// pld output comm. pin to dsp
	UInt8 unused6;
	UInt8	rs232Shdn;		// pld output control rs232 interface chip on/off
	UInt8	unused7;
	UInt8	spareOut;		// pld output not used
	UInt8	unused8;
	UInt16	dTo3Sln;			// pld input comm. from dsp
	UInt16	iXtrnl2;			// pld input cradle connector pin
}	HwrJerryPLDType;

typedef	volatile HwrJerryPLDType*	HwrJerryPldPtr;

/************************************************************************
 * Jerry EZ cost reduction.  PLD settings
 ***********************************************************************/
#define hwrJerryPld232Enable				0x80
#define  hwrJerryPld232Disable			0
#define hwrJerryPldChargerOn				0x80
#define  hwrJerryPldChargerOff			0
#define hwrJerryPldPaBiasEnable			0x80
#define  hwrJerryPldPaBiasDisable		0
#define hwrJerryPldRefOn					0x80
#define  hwrJerryPldRefOff					0
#define hwrJerryPldDspSelect				0x80
#define  hwrJerryPldDspDeselect			0
#define hwrJerryPldDspResetAssert		0x80
#define  hwrJerryPldDspResetDeassert	0
#define hwrJerryPldDspPowerOn				0x80
#define  hwrJerryPldDspPowerOff			0

/************************************************************************
 * Jerry EZ cost reduction.  PLD shadow settings
 ***********************************************************************/
#define hwrJerryPldDspOnBit				0x0001
#define hwrJerryPldChargeOnBit			0x0002
#define hwrJerryPldRefOnBit				0x0004
#define hwrJerryPldPaBiasEnableBit		0x0008
#define hwrJerryPldDspResetBit			0x0010
#define hwrJerryPldEzToDslBit				0x0020
#define hwrJerryPldRs232Bit				0x0040

/************************************************************************
 * Chip ID register for EZ
 ***********************************************************************/
 #define hwrEZ328chipIDEZ		0x45

/************************************************************************
 * Mask ID register for EZ
 ***********************************************************************/
#define hwrEZ328maskID0H31J	0x01
#define hwrEZ328maskID0J75C	0x02
#define hwrEZ328maskID1J75C	0x03
#define hwrEZ328maskID0J83G	0x04
 
/************************************************************************
 * LCD Controller Bits
 ************************************************************************/
// lcdCursorXPos Register
#define	hwrEZ328LcdCursorXPosCtlMask			0xC000
	#define	hwrEZ328LcdCursorXPosCtlTrans	0x0000
	#define	hwrEZ328LcdCursorXPosCtlBlack	0x4000
	#define	hwrEZ328LcdCursorXPosCtlReverse	0x8000
	#define  hwrEZ328LcdCursorXPosCtlWhite   0xC000
#define  hwrEZ328LcdCursorXPosValueMask		0x1FF
#define  hwrEZ328LcdCursorYPosValueMask		0x1FF
	
#define hwrEZ328LcdCursorWidthMask			0x1F
   #define  hwrEZ328LcdCursorWidthOffset  0x8
#define hwrEZ328LcdCursorHeightMask			0x1F

// lcdBlinkControl Register
#define	hwrEZ328LcdBlinkControlEnable		0x80 
#define  hwrEZ328LcdBlinkDivisorMask      0x7F

// lcdPanelControl Register
#define	hwrEZ328LcdPanelControlBusMask		0x0C
	#define	hwrEZ328LcdPanelControlBus1Bit	0x00
	#define	hwrEZ328LcdPanelControlBus2Bit	0x04
	#define	hwrEZ328LcdPanelControlBus4Bit	0x08
#define	hwrEZ328LcdPanelControlGrayScale	0x01
#define	hwrEZ328LcdPanelControl4BitGrayScale	0x02

// lcdPolarity Register
#define	hwrEZ328LcdPolarityShiftClock		0x08
#define	hwrEZ328LcdPolarityFLM					0x04
#define	hwrEZ328LcdPolarityLP					0x02
#define	hwrEZ328LcdPolarityPixel				0x01

// lcdClockControl Register
#define	hwrEZ328LcdClockControlEnable			0x80
#define	hwrEZ328LcdClockControl8BitBus			0x40

#define	hwrEZ328LcdClockControlBurstRateMask	0x0F
#define	hwrEZ328LcdClockControlBurstRate1		0x00
#define	hwrEZ328LcdClockControlBurstRate2		0x01
#define	hwrEZ328LcdClockControlBurstRate3		0x02
#define	hwrEZ328LcdClockControlBurstRate4		0x03
#define	hwrEZ328LcdClockControlBurstRate5		0x04
#define	hwrEZ328LcdClockControlBurstRate6		0x05
#define	hwrEZ328LcdClockControlBurstRate7		0x06
#define	hwrEZ328LcdClockControlBurstRate8		0x07
#define	hwrEZ328LcdClockControlBurstRate9		0x08
#define	hwrEZ328LcdClockControlBurstRate10		0x09
#define	hwrEZ328LcdClockControlBurstRate11		0x0A
#define	hwrEZ328LcdClockControlBurstRate12		0x0B
#define	hwrEZ328LcdClockControlBurstRate13		0x0C
#define	hwrEZ328LcdClockControlBurstRate14		0x0D
#define	hwrEZ328LcdClockControlBurstRate15		0x0E
#define	hwrEZ328LcdClockControlBurstRate16		0x0F

#define  hwrEZ328LcdContrastPWMWidthMask			0x00FF
#define  hwrEZ328LcdContrastEnable					0x0100



/************************************************************************
 * Interrupt Controller Bits
 ************************************************************************/
// intMaskHi bits
// NOTE: Due to a bug in early rev's of the DragonBall, the clear bits for
// IRQ[1236] on those early chips were different than the indicator bits
// (the order was reversed).  So some older code uses the OR of the IRQ1 and
// IRQ6 bits for either, and the OR of the IRQ2 and IRQ3 bits for either.
#define	hwrEZ328IntHiEMU							0x0080
#define	hwrEZ328IntHiSampleTimer				0x0040
#define	hwrEZ328IntHiPen							0x0010
#define	hwrEZ328IntHiIRQ6						0x0008
#define	hwrEZ328IntHiIRQ3						0x0004
#define	hwrEZ328IntHiIRQ2						0x0002
#define	hwrEZ328IntHiIRQ1						0x0001

// intMaskLo bits
#define	hwrEZ328IntLoInt3						0x0800
#define	hwrEZ328IntLoInt2						0x0400
#define	hwrEZ328IntLoInt1						0x0200
#define	hwrEZ328IntLoInt0						0x0100
#define	hwrEZ328IntLoAllKeys					0x0F00
#define	hwrEZ328IntLoInt0Bit					8
#define	hwrEZ328IntLoPWM						0x0080
#define	hwrEZ328IntLoPWMBit					7
#define	hwrEZ328IntLoKbd						0x0040
#define	hwrEZ328IntLoRTC						0x0010
#define	hwrEZ328IntLoRTCBit					4
#define	hwrEZ328IntLoWDT						0x0008
#define	hwrEZ328IntLoUART						0x0004
#define	hwrEZ328IntLoUARTBit					2
#define	hwrEZ328IntLoTimer					0x0002
#define	hwrEZ328IntLoTimerBit				1
#define	hwrEZ328IntLoSPIM						0x0001

// intControl bits
// NOTE: These are different than that documented in the User's manual 
// for the DragonBall (see erratas).  The DragonBall is the same as the EZ.
#define	hwrEZ328IntCtlEdge1						0x0800	
#define	hwrEZ328IntCtlEdge2						0x0400
#define	hwrEZ328IntCtlEdge3						0x0200
#define	hwrEZ328IntCtlEdge6						0x0100
#define	hwrEZ328IntCtlPol1						0x8000	
#define	hwrEZ328IntCtlPol2						0x4000
#define	hwrEZ328IntCtlPol3						0x2000
#define	hwrEZ328IntCtlPol6						0x1000
#define  hwrEZ328IntCtlPol5						0x0080

/************************************************************************
 * DRAM Controller Bits bits
 * Not all the bits are defined here-- some are pretty bad to muck with
 * in anything but initial setup of the DRAM.
 ************************************************************************/
#define  hwrEZ328dramControlEnable					0x8000
#define  hwrEZ328dramControlSelfRefreshEn			0x4000
#define  hwrEZ328dramControlLightSleep				0x0010
#define  hwrEZ328dramControlLowPowerRefreshEn		0x0004
#define  hwrEZ328dramControlDWEEnable				0x0001


/************************************************************************
 * Timer bits
 ************************************************************************/
#define	hwrEZ328TmrControlUnused						0xFC00
#define  hwrEZ328TmrControlTIOOutput              0x0200
#define	hwrEZ328TmrControlFreeRun						0x0100
#define	hwrEZ328TmrControlCaptureEdgeMask			0x00C0
	#define	hwrEZ328TmrControlCaptureEdgeNone		0x0000
	#define	hwrEZ328TmrControlCaptureEdgeRising	0x0040
	#define	hwrEZ328TmrControlCaptureEdgeFalling	0x0080
	#define	hwrEZ328TmrControlCaptureEdgeBoth		0x00C0
#define	hwrEZ328TmrControlOutputModeToggle			0x0020
#define	hwrEZ328TmrControlEnInterrupt				0x0010
#define	hwrEZ328TmrControlClkSrcMask					0x000E
	#define	hwrEZ328TmrControlClkSrcStop				0x0000
	#define	hwrEZ328TmrControlClkSrcSys				0x0002
	#define	hwrEZ328TmrControlClkSrcSysBy16			0x0004
	#define	hwrEZ328TmrControlClkSrcTIN				0x0006
	#define	hwrEZ328TmrControlClkSrc32KHz			0x0008
#define	hwrEZ328TmrControlEnable						0x0001

#define	hwrEZ328TmrStatusCapture						0x0002
#define	hwrEZ328TmrStatusCaptureBit					1
#define	hwrEZ328TmrStatusCompare						0x0001
#define	hwrEZ328TmrStatusCompareBit					0


/************************************************************************
 * 68EZ328 Serial Port Register Bits
 ************************************************************************/
// uControl Register
#define	hwrEZ328UControlUARTEnable			0x8000
#define	hwrEZ328UControlRxEnable				0x4000
#define	hwrEZ328UControlTxEnable				0x2000
#define	hwrEZ328UControlRxClock1xSync		0x1000
#define	hwrEZ328UControlParityEn				0x0800
#define	hwrEZ328UControlParityOdd				0x0400
#define	hwrEZ328UControlStopBits2				0x0200
#define	hwrEZ328UControlDataBits8				0x0100
#define	hwrEZ328UControlOldDataEn				0x0080
#define	hwrEZ328UControlCTSDeltaEn			0x0040
#define	hwrEZ328UControlRxFullEn				0x0020
#define	hwrEZ328UControlRxHalfEn				0x0010
#define	hwrEZ328UControlRxRdyEn				0x0008
#define	hwrEZ328UControlTxEmptyEn				0x0004
#define	hwrEZ328UControlTxHalfEn				0x0002
#define	hwrEZ328UControlTxAvailEn				0x0001

#define	hwrEZ328UControlEnableAll			   (hwrEZ328UControlUARTEnable |  	\
														 hwrEZ328UControlRxEnable |		\
														 hwrEZ328UControlTxEnable)

// uBaud Register
// UCLK output requires PE3 Select to be 0
#define	hwrEZ328UBaudUCLKDirOut				0x2000
#define	hwrEZ328UBaudBaudSrcUCLK				0x0800
#define	hwrEZ328UBaudDivider					0x0700
#define	hwrEZ328UBaudPrescaler					0x003F

#define	hwrEZ328UBaudDivideBitOffset			8


// uReceive Register
#define	hwrEZ328UReceiveFIFOFull				0x8000
#define	hwrEZ328UReceiveFIFOHalf				0x4000
#define	hwrEZ328UReceiveDataRdy				0x2000
#define	hwrEZ328UReceiveDataRdyBit			13
#define	hwrEZ328UReceiveOldData				0x1000
#define	hwrEZ328UReceiveOverrunErr			0x0800
#define	hwrEZ328UReceiveOverrunErrBit		11
#define	hwrEZ328UReceiveFrameErr				0x0400
#define	hwrEZ328UReceiveFrameErrBit			10
#define	hwrEZ328UReceiveBreakErr				0x0200
#define	hwrEZ328UReceiveBreakErrBit			9
#define	hwrEZ328UReceiveParityErr				0x0100
#define	hwrEZ328UReceiveParityErrBit			8
#define	hwrEZ328UReceiveData					0x00FF

#define	hwrEZ328UReceiveErrsMask				(hwrEZ328UReceiveOverrunErr |	\
														 hwrEZ328UReceiveFrameErr |		\
														 hwrEZ328UReceiveBreakErr |		\
														 hwrEZ328UReceiveParityErr)

// uTransmit Register
#define	hwrEZ328UTransmitFIFOEmpty			0x8000
#define	hwrEZ328UTransmitFIFOHalf				0x4000
#define	hwrEZ328UTransmitTxAvail				0x2000
#define	hwrEZ328UTransmitSendBreak			0x1000
#define	hwrEZ328UTransmitIgnoreCTS			0x0800
#define  hwrEZ328UTransmitBusy              0x0400
#define	hwrEZ328UTransmitCTSStatus			0x0200
#define	hwrEZ328UTransmitCTSDelta				0x0100
#define	hwrEZ328UTransmitData					0x00FF

// uMisc Register
#define  hwrEZ328UMiscBaudTest              0x8000
#define	hwrEZ328UMiscClkSrcUCLK				0x4000
#define	hwrEZ328UMiscForceParityErr			0x2000
#define	hwrEZ328UMiscLoopback					0x1000
#define  hwrEZ328UMiscBaudReset             0x0800
#define  hwrEZ328UMiscIRTestEn              0x0400
#define	hwrEZ328UMiscReservedMask				0x0300
#define	hwrEZ328UMiscRTSThruFIFO				0x0080
#define	hwrEZ328UMiscRTSOut						0x0040
#define	hwrEZ328UMiscIRDAEn						0x0020
#define	hwrEZ328UMiscLoopIRDA					0x0010
#define  hwrEZ328UMiscRXPolarityInv         0x0008
#define  hwrEZ328UMiscTXPolarityInv         0x0004
#define	hwrEZ328UMiscUnused						0x0003

// uNonIntPresc Register
#define	hwrEZ328UNonIntPrescSrcIntPresc		0x0000
#define	hwrEZ328UNonIntPrescSrcNonIntPresc	0x8000

#define	hwrEZ328UNonIntPrescSelDiv128			0x0000
#define	hwrEZ328UNonIntPrescSelDiv64			0x0100
#define	hwrEZ328UNonIntPrescSelDiv32			0x0200
#define	hwrEZ328UNonIntPrescSelDiv16			0x0300
#define	hwrEZ328UNonIntPrescSelDiv8			0x0400
#define	hwrEZ328UNonIntPrescSelDiv4			0x0500
#define	hwrEZ328UNonIntPrescSelDiv2			0x0600
#define	hwrEZ328UNonIntPrescSelDiv1			0x0700

#define	hwrEZ328UNonIntPrescStepMask			0x00FF
#define	hwrEZ328UNonIntPrescSelOffset			8

// see EZ docs for explanation of the following.  This provides the correct
// pulse width for IRDA, and is just a statement in the docs. The values don't
// appear to make sense, but I just push the buttons.

// This has been deprecated in favor of a prv function in the serial driver and
// the SerialMgr (depending on whether you have Bond or not) that calculates
// the value of this register based on the clock frequency you're running on.
// There was a bug when running EZ at 20MHz that caused the IrDA pulsewidth to
// get too short.

#if 0
#define 	hwrEZ328UNonIntPrescIRDAMode			(hwrEZ328UNonIntPrescSrcIntPresc \
															| hwrEZ328UNonIntPrescSelDiv32 \
															| (hwrEZ328UNonIntPrescStepMask & 0x20))
#endif

/************************************************************************
 * PWM Bits
 ************************************************************************/
// PWM register bits of interest
#define	hwrEZ328PWMControlEnable				0x0010
#define	hwrEZ328PWMControlEnableBit			4
#define	hwrEZ328PWMControlEnableIRQ			0x0040
#define	hwrEZ328PWMIRQStatus					0x0080
// PWM divider chain output selectors
#define	hwrEZ328PWMControlDivMask				0x0003
#define	hwrEZ328PWMControlDivBy2				0x0
#define	hwrEZ328PWMControlDivBy4				0x1
#define	hwrEZ328PWMControlDivBy8				0x2
#define	hwrEZ328PWMControlDivBy16				0x3
// PWM Repeat Counts
#define	hwrEZ328PWMRepeatSample1x				0x0
#define	hwrEZ328PWMRepeatSample2x				0x4
#define	hwrEZ328PWMRepeatSample4x				0x8
#define	hwrEZ328PWMRepeatSample8x				0xC
// PWM hardware constraints
#define	hwrEZ328PWMFifoSizeInBytes				4
#define	hwrEZ328PWMPrescalerMask				0x7F00
#define	hwrEZ328PWMPrescalerOffset				8

/************************************************************************
 * PLL Bits
 * note that these do not follow the '328 spec-- however, they seem to 
 * follow the 'EZ328 spec, so maybe Motorola is behind in their tech pubs
 * department?? (Naah.)
 ************************************************************************/
#define	hwrEZ328PLLControlDisable				0x0008
#define	hwrEZ328PLLControlClkEn				0x0010

#define	hwrEZ328PLLControlSysDMADiv2			0x0000
#define	hwrEZ328PLLControlSysDMADiv4			0x0100
#define	hwrEZ328PLLControlSysDMADiv8			0x0200
#define	hwrEZ328PLLControlSysDMADiv16		0x0300
#define	hwrEZ328PLLControlSysDMADiv1			0x0400

#define	hwrEZ328PLLControlPixDMADiv2			0x0000  
#define	hwrEZ328PLLControlPixDMADiv4			0x0800
#define	hwrEZ328PLLControlPixDMADiv8			0x1000
#define	hwrEZ328PLLControlPixDMADiv16		0x1800
#define	hwrEZ328PLLControlPixDMADiv1			0x2000

#define  hwrEZ328PLLControlDMAVCODiv2       0x0000
#define  hwrEZ328PLLControlDMAVCODiv4       0x0020
#define  hwrEZ328PLLControlDMAVCODiv8       0x0040
#define  hwrEZ328PLLControlDMAVCODiv16      0x0060
#define  hwrEZ328PLLControlDMAVCODiv1       0x0080

#define  hwrEZ328PLLFreqSelCLK32				0x8000

/************************************************************************
 * Real Time Clock (RTC) Bits
 ************************************************************************/

// RTC Control Register (RTCCTL) bits
#define hwrEZ328RTCControlRTCEnable				0x80
#define hwrEZ328RTCControlRefSelMask				0x20
#define hwrEZ328RTCControlRefSel38400				0x20		// 38.4 kHz reference frequency
#define hwrEZ328RTCControlRefSel32768				0x00		// 32.768 kHz reference frequency

// RTC Interrupt Enable Register (RTCIENR) bits
#define hwrEZ328RTCIntEnableSample7            0x8000
#define hwrEZ328RTCIntEnableSample6            0x4000
#define hwrEZ328RTCIntEnableSample5            0x2000
#define hwrEZ328RTCIntEnableSample4            0x1000
#define hwrEZ328RTCIntEnableSample3            0x0800
#define hwrEZ328RTCIntEnableSample2            0x0400
#define hwrEZ328RTCIntEnableSample1            0x0200
#define hwrEZ328RTCIntEnableSample0            0x0100
#define hwrEZ328RTCIntEnableHour               0x0020
#define hwrEZ328RTCIntEnableSec						0x0010
#define hwrEZ328RTCIntEnable24Hr					0x0008
#define hwrEZ328RTCIntEnableAlarm					0x0004
#define hwrEZ328RTCIntEnableMinute					0x0002
#define hwrEZ328RTCIntEnableStopWatch				0x0001

// RTC Interrupt Status Register bits
#define hwrEZ328RTCIntStatusSample7            0x8000
#define hwrEZ328RTCIntStatusSample6            0x4000
#define hwrEZ328RTCIntStatusSample5            0x2000
#define hwrEZ328RTCIntStatusSample4            0x1000
#define hwrEZ328RTCIntStatusSample3            0x0800
#define hwrEZ328RTCIntStatusSample2            0x0400
#define hwrEZ328RTCIntStatusSample1            0x0200
#define hwrEZ328RTCIntStatusSample0            0x0100
#define hwrEZ328RTCIntStatusHour               0x0020
#define hwrEZ328RTCIntStatusSec						0x0010
#define hwrEZ328RTCIntStatus24Hr					0x0008
#define hwrEZ328RTCIntStatusAlarm					0x0004
#define hwrEZ328RTCIntStatusMinute					0x0002
#define hwrEZ328RTCIntStatusStopWatch				0x0001

// RTC Alarm register (RTCALRM) masks and offsets
#define hwrEZ328RTCAlarmSecondsMask				0x0000003fL
#define hwrEZ328RTCAlarmSecondsOffset				0
#define hwrEZ328RTCAlarmMinutesMask				0x003f0000L
#define hwrEZ328RTCAlarmMinutesOffset				16
#define hwrEZ328RTCAlarmHoursMask					0x1f000000L
#define hwrEZ328RTCAlarmHoursOffset				24

// RTC hours/minutes/seconds register (RTCHMS) masks and offsets
#define hwrEZ328RTCHourMinSecSecondsMask			0x0000003fL
#define hwrEZ328RTCHourMinSecSecondsOffset		0
#define hwrEZ328RTCHourMinSecMinutesMask			0x003f0000L
#define hwrEZ328RTCHourMinSecMinutesOffset		16
#define hwrEZ328RTCHourMinSecHoursMask			0x1f000000L
#define hwrEZ328RTCHourMinSecHoursOffset			24

// RTC Day register masks/offsets
#define hwrEZ328RTCDayDayMask                  0x01FF
#define hwrEZ328RTCDayDayOffset                0

// RTC Watchdog Timer masks/offsets
#define hwrEZ328RTCWatchdogEnable              0x0001
#define hwrEZ328RTCWatchdogIntSel              0x0002
#define hwrEZ328RTCWatchdogIntStatus           0x0080
#define hwrEZ328RTCWatchdogCountMask           0x0300
#define hwrEZ328RTCWatchdogCountOffset         8

/************************************************************************
 * SPI Master bits
 ************************************************************************/
#define	hwrEZ328SPIMControlRateMask			0xE000
	#define	hwrEZ328SPIMControlRateDiv4			0x0000
	#define	hwrEZ328SPIMControlRateDiv8			0x2000
	#define	hwrEZ328SPIMControlRateDiv16			0x4000
	#define	hwrEZ328SPIMControlRateDiv32			0x6000
	#define	hwrEZ328SPIMControlRateDiv64			0x8000
	#define	hwrEZ328SPIMControlRateDiv128		0xA000
	#define	hwrEZ328SPIMControlRateDiv256		0xC000
	#define	hwrEZ328SPIMControlRateDiv512		0xE000
#define	hwrEZ328SPIMControlEnable				0x0200
#define	hwrEZ328SPIMControlExchange			0x0100
#define	hwrEZ328SPIMControlIntStatus			0x0080
#define	hwrEZ328SPIMControlIntEnable			0x0040
#define	hwrEZ328SPIMControlOppPhase			0x0020
#define	hwrEZ328SPIMControlInvPolarity		0x0010
#define	hwrEZ328SPIMControlBitsMask			0x000F


#endif // __M68EZ328_H__

#endif // NON_PORTABLE
