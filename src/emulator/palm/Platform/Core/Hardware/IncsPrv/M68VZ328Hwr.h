/*******************************************************************
 * 							 Pilot Software
 *
 *		 Copyright(c) 1995-1999, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------  
 * FileName:
 *		M68VZ328Hwr.h
 *
 * Description:
 *		Hardware Equates for the Motorola 68VZ328 Dragonball-VZ chip 
 *
 * History:
 *   	11/19/99  EBE	Created; integrated from M68EZ328Hwr.h
 *   	11/22/99  EBE	Added Port Defs for J, K, & M
 *   	12/03/99  EBE	Fixed up fillers @ 0x00A & 0x518
 *
 *******************************************************************/  

#ifdef	NON_PORTABLE						// So apps don't include this...


#ifndef	__M68VZ328HWR_H__
#define	__M68VZ328HWR_H__


#include <PalmTypes.h>							// Touchdown includes


//  Define location of the 68VZ328 Dragonball chip
#define	hwr68328Base		0xFFFFF000L					// Base address of 68328 


/***********************************************************************
 * Motorola 68VZ328  registers
 ***********************************************************************/
struct HwrM68VZ328Type {

	UInt8		scr;								// $000: System Control Register
	UInt8											___filler0[0x003-0x001];
	UInt8		pcr;								// $003: Peripheral Control Register
	UInt8		chipID;							// $004: Chip ID Register
	UInt8		maskID;							// $005: Mask ID Register
	UInt16	swID;								// $006: Software ID Register
	UInt16	ioDriveControl;				// $008: I/O Drive Control Register
	UInt8											___filler1[0x100-0x00A];				 
	
	UInt16	csAGroupBase;					// $100: Chip Select Group A Base Register
	UInt16	csBGroupBase;					// $102: Chip Select Group B Base Register
	UInt16	csCGroupBase;					// $104: Chip Select Group C Base Register
	UInt16	csDGroupBase;					// $106: Chip Select Group D Base Register
	
	UInt16	csUGroupBase;					// $108: Chip Select Upper Group Base Register
	
	UInt16	csControl1;						// $10A: Chip Select Control Register
	UInt16	csControl2;						// $10C: Chip Select Control Register
	
	UInt8											___filler2[0x110-0x10E];

	UInt16	csASelect;						// $110: Group A Chip Select Register
	UInt16	csBSelect;						// $112: Group B Chip Select Register
	UInt16	csCSelect;						// $114: Group C Chip Select Register
	UInt16	csDSelect;						// $116: Group D Chip Select Register
	
	UInt16	emuCS;						   // $118: EMU Chip Select Register
	
	UInt8											___filler3[0x150-0x11A];		
	
	UInt16	csControl3;						// $150: Chip Select Control Register

	UInt8											___filler3a[0x200-0x152];		
	
	UInt16	pllControl;						// $200: PLL Control Register
	UInt16	pllFreqSel;						// $202: CGM Frequency Select Register
	
	UInt8											___filler4[0x207-0x204];
	
	UInt8		pwrControl;						// $207: Power Control Register
	
	UInt8											___filler5[0x300-0x208];
	
	UInt8		intVector;						// $300: Interrupt Vector Register
	UInt8											___filler6;
	UInt16	intControl;						// $302: Interrupt Control Register
	UInt16	intMaskHi;						// $304: Interrupt Mask Register/HIGH word
	UInt16	intMaskLo;						// $306: Interrupt Mask Register/LOW word
	UInt8											___filler7[0x30c-0x308];
	UInt16	intStatusHi;					// $30C: Interrupt Status Register/HIGH word
	UInt16	intStatusLo;					// $30E: Interrupt Status Register/LOW word
	UInt16	intPendingHi;					// $310: Interrupt Pending Register/HIGH word
	UInt16	intPendingLo;					// $312: Interrupt Pending Register/LOW word
	UInt16	intLevelControl;				// $314: Interrupt Level Control Register
	
	UInt8 										___filler4a[0x400-0x316];
	
	UInt8		portADir;						// $400: Port A Direction Register
	UInt8		portAData;						// $401: Port A Data Register
	UInt8		portAPullupEn;					// $402: Port A Pullup Enable
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
	
	UInt8											___filler17[0x438-0x434];
		
	UInt8		portJDir;						// $438: Port J Direction Register
	UInt8		portJData;						// $439: Port J Data Register
	UInt8		portJPullupEn;					// $43A: Port J Pull-up Enable
	UInt8		portJSelect;					// $43B: Port J Select Register
	
	UInt8											___filler18[0x440-0x43C];
		
	UInt8		portKDir;						// $440: Port K Direction Register
	UInt8		portKData;						// $441: Port K Data Register
	UInt8		portKPullupdnEn;				// $442: Port K Pull-up/down Enable
	UInt8		portKSelect;					// $443: Port K Select Register
	
	UInt8											___filler19[0x448-0x444];
		
	UInt8		portMDir;						// $448: Port M Direction Register
	UInt8		portMData;						// $449: Port M Data Register
	UInt8		portMPullupdnEn;				// $44A: Port M Pull-up/down Enable
	UInt8		portMSelect;					// $44B: Port M Select Register
	
	UInt8											___filler20[0x500-0x44C];

	UInt16	pwmControl;						// $500: PWM 1 Control Register  (PWM 1 is the same as the single PWM in EZ)
	UInt8		pwmSampleHi;    				// $502: PWM 1 Sample - high byte
	UInt8		pwmSampleLo;					// $503: PWM 1 Sample - low byte
	UInt8		pwmPeriod;						// $504: PWM 1 Period
	UInt8		pwmCounter;						// $505: PWM 1 Counter
	
	UInt8											___filler22[0x510-0x506];

	UInt16	pwm2Control;					// $510: PWM 2 Control Register
	UInt16	pwm2Period;						// $512: PWM 2 Period
	UInt16	pwm2Width;						// $514: PWM 2 Width
	UInt16	pwm2Counter;					// $516: PWM 2 Counter

	UInt8											___filler23[0x600-0x518];
	
	UInt16	tmr1Control;					// $600: Timer 1 Control Register
	UInt16	tmr1Prescaler;					// $602: Timer 1 Prescaler Register
	UInt16	tmr1Compare;					// $604: Timer 1 Compare Register
	UInt16	tmr1Capture;					// $606: Timer 1 Capture Register
	UInt16	tmr1Counter;					// $608: Timer 1 Counter Register
	UInt16	tmr1Status;						// $60A: Timer 1 Status Register
	
	UInt8											___filler24[0x610-0x60C];
		
	UInt16	tmr2Control;					// $610: Timer 2 Control Register
	UInt16	tmr2Prescaler;					// $612: Timer 2 Prescaler Register
	UInt16	tmr2Compare;					// $614: Timer 2 Compare Register
	UInt16	tmr2Capture;					// $616: Timer 2 Capture Register
	UInt16	tmr2Counter;					// $618: Timer 2 Counter Register
	UInt16	tmr2Status;						// $61A: Timer 2 Status Register
	
	UInt8											___filler25[0x700-0x61C];
	
	UInt16	spiRxD;						// $700: SPI Unit 1 Receive Data Register
	UInt16	spiTxD;						// $702: SPI Unit 1 Transmit Data Register
	UInt16	spiCont1;					// $704: SPI Unit 1 Control/Status Register
	UInt16	spiIntCS;					// $706: SPI Unit 1 Interrupt control/Status Register
	UInt16	spiTest;					// $708: SPI Unit 1 Test Register
	UInt16	spiSpc;						// $70A: SPI Unit 1 Sample period counter register
		
	UInt8											___filler26[0x800-0x70C];
	
	UInt16	spiMasterData;					// $800: SPI Unit 2 Data Register (SPI 2 is the same as the single SPI Master in EZ)
	UInt16	spiMasterControl;				// $802: SPI Unit 2 Control/Status Register
	
	UInt8											___filler27[0x900-0x804];
	
	UInt16	uControl;						// $900: Uart 1 Status/Control Register (Uart 1 is the same as the single Uart in EZ)
	UInt16	uBaud;							// $902: Uart 1 Baud Control Register
	UInt16	uReceive;						// $904: Uart 1 Receive Register
	UInt16	uTransmit;						// $906: Uart 1 Transmit Register
	UInt16	uMisc;							// $908: Uart 1 Miscellaneous Register
	UInt16	uNonIntPresc;					// $90A: Uart 1 Non-Integer Prescaler
	
	UInt8											___filler28[0x910-0x90C];
	
	UInt16	u2Control;						// $910: Uart 2 Status/Control Register
	UInt16	u2Baud;							// $912: Uart 2 Baud Control Register
	UInt16	u2Receive;						// $914: Uart 2 Receiver Register
	UInt16	u2Transmit;						// $916: Uart 2 Transmitter Register
	UInt16	u2Misc;							// $918: Uart 2 Miscellaneous Register
	UInt16	u2NonIntPresc;					// $91A: Uart 2 Non-Integer Prescaler
	UInt16	u2FIFOHMark;					// $91C: Uart 2 Half Mark FIFO Register
	
	UInt8											___filler28a[0xA00-0x91E];
	
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
	UInt8		lcdPanelControl;				// $A20: Panel Interface Configuration Register
	UInt8		lcdPolarity;					// $A21: Polarity Config Register
	UInt8											___filler33;						
	UInt8		lcdACDRate;						// $A23: ACD (M) Rate Control Register
	UInt8											___filler34;
	UInt8		lcdPixelClock;					// $A25: Pixel Clock Divider Register
	UInt8											___filler35;
	UInt8		lcdClockControl;				// $A27: Clocking Control Register
//	UInt8											___filler36;
	UInt16	lcdRefreshRateAdj;			// $A28: Refresh Rate Adjustment Register
	UInt8											___filler37;
	UInt8		lcdReserved1;					// $A2B: Reserved
	UInt8											___filler38;
	UInt8    lcdPanningOffset;				// $A2D: Panning Offset Register
	
	UInt8											___filler39[0xA31-0xA2E];
	
	UInt8		lcdFrameRate;					// $A31: Frame Rate Control Modulation Register
	UInt8											___filler2004;
	UInt8		lcdGrayPalette;				// $A33: Gray Palette Mapping Register
	UInt8		lcdReserved2;					// $A34: Reserved
	UInt8											___filler2005;
	UInt16	lcdContrastControlPWM;		// $A36: Contrast Control
	UInt8		lcdRefreshModeControl;		// $A38: Refresh Mode Control Register
	UInt8		lcdDMAControl;					// $A39:	DMA Control Register
	
	UInt8											___filler40[0xB00-0xA3a];
	
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
   UInt16   sdramControl;              // $C04: SDRAM Control Register
   UInt16   sdramPwDn;              	// $C06: SDRAM Power Down Register

   UInt8                                ___filler42[0xD00-0xC08];
	
	UInt32	emuAddrCompare;				// $D00: Emulation Address Compare Register   
	UInt32	emuAddrMask;					// $D04: Emulation Address Mask Register
	UInt16	emuControlCompare;			// $D08: Emulation Control Compare Register
	UInt16	emuControlMask;				// $D0A: Emulation Control Mask Register
	UInt16	emuControl;						// $DOC: Emulation Control Register
	UInt16	emuStatus;						// $D0E: Emulation Status Register
};

typedef volatile struct HwrM68VZ328Type*	HwrM68VZ328Ptr;

#define HwrDBallType HwrM68VZ328Type
#define HwrDBallPtr HwrM68VZ328Ptr


#if 0	// Take this out for Poser; it conflicts with the one in M68EZ328Hwr.h

/************************************************************************
 * Jerry EZ cost reduction PLD I/O register
 ***********************************************************************/
typedef volatile struct HwrJerryPLDType {

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

typedef	HwrJerryPLDType*	HwrJerryPldPtr;

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
#endif

/************************************************************************
 * Chip ID register for VZ
 ***********************************************************************/
 #define hwrVZ328chipIDVZ		0x45

/************************************************************************
 * Mask ID register for VZ
 ***********************************************************************/
#define hwrVZ328maskID0H31J	0x01
#define hwrVZ328maskID0J75C	0x02
#define hwrVZ328maskID1J75C	0x03
#define hwrVZ328maskID0J83G	0x04
 
/************************************************************************
 * LCD Controller Bits
 ************************************************************************/
// lcdCursorXPos Register
#define	hwrVZ328LcdCursorXPosCtlMask			0xC000
	#define	hwrVZ328LcdCursorXPosCtlTrans	0x0000
	#define	hwrVZ328LcdCursorXPosCtlBlack	0x4000
	#define	hwrVZ328LcdCursorXPosCtlReverse	0x8000
	#define  hwrVZ328LcdCursorXPosCtlWhite   0xC000
#define  hwrVZ328LcdCursorXPosValueMask		0x1FF
#define  hwrVZ328LcdCursorYPosValueMask		0x1FF
	
#define hwrVZ328LcdCursorWidthMask			0x1F
   #define  hwrVZ328LcdCursorWidthOffset  0x8
#define hwrVZ328LcdCursorHeightMask			0x1F

// lcdBlinkControl Register
#define	hwrVZ328LcdBlinkControlEnable		0x80 
#define  hwrVZ328LcdBlinkDivisorMask      0x7F

// lcdPanelControl Register
#define	hwrVZ328LcdPanelControlBusMask		0x0C
	#define	hwrVZ328LcdPanelControlBus1Bit	0x00
	#define	hwrVZ328LcdPanelControlBus2Bit	0x04
	#define	hwrVZ328LcdPanelControlBus4Bit	0x08
#define	hwrVZ328LcdPanelControlGrayScale	0x01
#define	hwrVZ328LcdPanelControl4BitGrayScale	0x02

// lcdPolarity Register
#define	hwrVZ328LcdPolarityShiftClock		0x08
#define	hwrVZ328LcdPolarityFLM					0x04
#define	hwrVZ328LcdPolarityLP					0x02
#define	hwrVZ328LcdPolarityPixel				0x01

// lcdClockControl Register
#define	hwrVZ328LcdClockControlEnable			0x80
#define	hwrVZ328LcdClockControl8BitBus			0x40

#define	hwrVZ328LcdClockControlBurstRateMask	0x0F
#define	hwrVZ328LcdClockControlBurstRate1		0x00
#define	hwrVZ328LcdClockControlBurstRate2		0x01
#define	hwrVZ328LcdClockControlBurstRate3		0x02
#define	hwrVZ328LcdClockControlBurstRate4		0x03
#define	hwrVZ328LcdClockControlBurstRate5		0x04
#define	hwrVZ328LcdClockControlBurstRate6		0x05
#define	hwrVZ328LcdClockControlBurstRate7		0x06
#define	hwrVZ328LcdClockControlBurstRate8		0x07
#define	hwrVZ328LcdClockControlBurstRate9		0x08
#define	hwrVZ328LcdClockControlBurstRate10		0x09
#define	hwrVZ328LcdClockControlBurstRate11		0x0A
#define	hwrVZ328LcdClockControlBurstRate12		0x0B
#define	hwrVZ328LcdClockControlBurstRate13		0x0C
#define	hwrVZ328LcdClockControlBurstRate14		0x0D
#define	hwrVZ328LcdClockControlBurstRate15		0x0E
#define	hwrVZ328LcdClockControlBurstRate16		0x0F

#define  hwrVZ328LcdContrastPWMWidthMask			0x00FF
#define  hwrVZ328LcdContrastEnable					0x0100



/************************************************************************
 * Interrupt Controller Bits
 ************************************************************************/
// intMaskHi bits
// NOTE: Due to a bug in early rev's of the DragonBall, the clear bits for
// IRQ[1236] on those early chips were different than the indicator bits
// (the order was reversed).  So some older code uses the OR of the IRQ1 and
// IRQ6 bits for either, and the OR of the IRQ2 and IRQ3 bits for either.
#define	hwrVZ328IntHiEMU						0x0080
#define	hwrVZ328IntHiSampleTimer				0x0040
#define	hwrVZ328IntHiPen						0x0010
#define	hwrVZ328IntHiIRQ6						0x0008
#define	hwrVZ328IntHiIRQ3						0x0004
#define	hwrVZ328IntHiIRQ2						0x0002
#define	hwrVZ328IntHiIRQ1						0x0001

// intMaskLo bits
#define	hwrVZ328IntLoPWM2						0x2000
#define	hwrVZ328IntLoPWM2Bit					13
#define	hwrVZ328IntLoUART2						0x1000
#define	hwrVZ328IntLoUART2Bit					12
#define	hwrVZ328IntLoInt3						0x0800
#define	hwrVZ328IntLoInt2						0x0400
#define	hwrVZ328IntLoInt1						0x0200
#define	hwrVZ328IntLoInt0						0x0100
#define	hwrVZ328IntLoAllKeys					0x0F00
#define	hwrVZ328IntLoInt0Bit					8
#define	hwrVZ328IntLoPWM						0x0080
#define	hwrVZ328IntLoPWMBit					7
#define	hwrVZ328IntLoKbd						0x0040
#define	hwrVZ328IntLoTimer2						0x0020
#define	hwrVZ328IntLoTimer2Bit					5
#define	hwrVZ328IntLoRTC						0x0010
#define	hwrVZ328IntLoRTCBit					4
#define	hwrVZ328IntLoWDT						0x0008
#define	hwrVZ328IntLoUART						0x0004
#define	hwrVZ328IntLoUARTBit					2
#define	hwrVZ328IntLoTimer					0x0002
#define	hwrVZ328IntLoTimerBit				1
#define	hwrVZ328IntLoSPIM						0x0001

// intControl bits
// NOTE: These are different than that documented in the User's manual 
// for the DragonBall (see erratas).  The DragonBall is the same as the EZ.
#define	hwrVZ328IntCtlEdge1						0x0800	
#define	hwrVZ328IntCtlEdge2						0x0400
#define	hwrVZ328IntCtlEdge3						0x0200
#define	hwrVZ328IntCtlEdge6						0x0100
#define	hwrVZ328IntCtlPol1						0x8000	
#define	hwrVZ328IntCtlPol2						0x4000
#define	hwrVZ328IntCtlPol3						0x2000
#define	hwrVZ328IntCtlPol6						0x1000
#define  hwrVZ328IntCtlPol5						0x0080

/************************************************************************
 * DRAM Controller Bits bits
 * Not all the bits are defined here-- some are pretty bad to muck with
 * in anything but initial setup of the DRAM.
 ************************************************************************/
#define  hwrVZ328dramControlEnable					0x8000
#define  hwrVZ328dramControlSelfRefreshEn			0x4000
#define  hwrVZ328dramControlEDOEn					0x0400
#define  hwrVZ328dramControlLightSleep				0x0010
#define  hwrVZ328dramControlLowPowerRefreshEn	0x0004
#define  hwrVZ328dramControlDWEEnable				0x0001


/************************************************************************
 * SDRAM Controller Bits bits
 * Not all the bits are defined here-- some are pretty bad to muck with
 * in anything but initial setup of the SDRAM.
 ************************************************************************/
// SdramControl bits
#define  hwrVZ328SdramControlEnable					0x8000
#define  hwrVZ328SdramControlContPgModeEn			0x4000
			// rsvd
#define  hwrVZ328SdramControlRefreshEn				0x1000
#define  hwrVZ328SdramControlInitPreChg			0x0800
#define  hwrVZ328SdramControlInitModeReg			0x0400
			// rsvd
			// rsvd
			// rsvd
#define  hwrVZ328SdramControlSColOpt				0x0040
			// BnkAddH[1:0]
			// BnkAddL[1:0]
#define  hwrVZ328SdramControlCASLatency			0x0002
#define  hwrVZ328SdramControlRefrActCmdLatency	0x0001

// SdramPwDn bits
#define  hwrVZ328SdramPwDnAPEn						0x8000
#define  hwrVZ328SdramPwDnPDEn						0x4000
			// rsvd
			// rsvd
			// PDTOUT[3:0]
			// rsvd
			// rsvd
			// rsvd
			// rsvd
			// rsvd
			// rsvd
			// rsvd
			// rsvd


/************************************************************************
 * Timer bits
 ************************************************************************/
#define	hwrVZ328TmrControlUnused						0xFC00
#define  hwrVZ328TmrControlTIOOutput              0x0200
#define	hwrVZ328TmrControlFreeRun						0x0100
#define	hwrVZ328TmrControlCaptureEdgeMask			0x00C0
	#define	hwrVZ328TmrControlCaptureEdgeNone		0x0000
	#define	hwrVZ328TmrControlCaptureEdgeRising	0x0040
	#define	hwrVZ328TmrControlCaptureEdgeFalling	0x0080
	#define	hwrVZ328TmrControlCaptureEdgeBoth		0x00C0
#define	hwrVZ328TmrControlOutputModeToggle			0x0020
#define	hwrVZ328TmrControlEnInterrupt				0x0010
#define	hwrVZ328TmrControlClkSrcMask					0x000E
	#define	hwrVZ328TmrControlClkSrcStop				0x0000
	#define	hwrVZ328TmrControlClkSrcSys				0x0002
	#define	hwrVZ328TmrControlClkSrcSysBy16			0x0004
	#define	hwrVZ328TmrControlClkSrcTIN				0x0006
	#define	hwrVZ328TmrControlClkSrc32KHz			0x0008
#define	hwrVZ328TmrControlEnable						0x0001

#define	hwrVZ328TmrStatusCapture						0x0002
#define	hwrVZ328TmrStatusCaptureBit					1
#define	hwrVZ328TmrStatusCompare						0x0001
#define	hwrVZ328TmrStatusCompareBit					0


/************************************************************************
 * 68VZ328 Serial Port Register Bits
 ************************************************************************/
// uControl Register
#define	hwrVZ328UControlUARTEnable			0x8000
#define	hwrVZ328UControlRxEnable				0x4000
#define	hwrVZ328UControlTxEnable				0x2000
#define	hwrVZ328UControlRxClock1xSync		0x1000
#define	hwrVZ328UControlParityEn				0x0800
#define	hwrVZ328UControlParityOdd				0x0400
#define	hwrVZ328UControlStopBits2				0x0200
#define	hwrVZ328UControlDataBits8				0x0100
#define	hwrVZ328UControlOldDataEn				0x0080
#define	hwrVZ328UControlCTSDeltaEn			0x0040
#define	hwrVZ328UControlRxFullEn				0x0020
#define	hwrVZ328UControlRxHalfEn				0x0010
#define	hwrVZ328UControlRxRdyEn				0x0008
#define	hwrVZ328UControlTxEmptyEn				0x0004
#define	hwrVZ328UControlTxHalfEn				0x0002
#define	hwrVZ328UControlTxAvailEn				0x0001

#define	hwrVZ328UControlEnableAll			   (hwrVZ328UControlUARTEnable |  	\
														 hwrVZ328UControlRxEnable |		\
														 hwrVZ328UControlTxEnable)

// uBaud Register
// UCLK output requires PE3 Select to be 0
#define	hwrVZ328UBaudUCLKDirOut				0x2000
#define	hwrVZ328UBaudBaudSrcUCLK				0x0800
#define	hwrVZ328UBaudDivider					0x0700
#define	hwrVZ328UBaudPrescaler					0x003F

#define	hwrVZ328UBaudDivideBitOffset			8


// uReceive Register
#define	hwrVZ328UReceiveFIFOFull				0x8000
#define	hwrVZ328UReceiveFIFOHalf				0x4000
#define	hwrVZ328UReceiveDataRdy				0x2000
#define	hwrVZ328UReceiveDataRdyBit			13
#define	hwrVZ328UReceiveOldData				0x1000
#define	hwrVZ328UReceiveOverrunErr			0x0800
#define	hwrVZ328UReceiveOverrunErrBit		11
#define	hwrVZ328UReceiveFrameErr				0x0400
#define	hwrVZ328UReceiveFrameErrBit			10
#define	hwrVZ328UReceiveBreakErr				0x0200
#define	hwrVZ328UReceiveBreakErrBit			9
#define	hwrVZ328UReceiveParityErr				0x0100
#define	hwrVZ328UReceiveParityErrBit			8
#define	hwrVZ328UReceiveData					0x00FF

#define	hwrVZ328UReceiveErrsMask				(hwrVZ328UReceiveOverrunErr |	\
														 hwrVZ328UReceiveFrameErr |		\
														 hwrVZ328UReceiveBreakErr |		\
														 hwrVZ328UReceiveParityErr)

// uTransmit Register
#define	hwrVZ328UTransmitFIFOEmpty			0x8000
#define	hwrVZ328UTransmitFIFOHalf				0x4000
#define	hwrVZ328UTransmitTxAvail				0x2000
#define	hwrVZ328UTransmitSendBreak			0x1000
#define	hwrVZ328UTransmitIgnoreCTS			0x0800
#define  hwrVZ328UTransmitBusy              0x0400
#define	hwrVZ328UTransmitCTSStatus			0x0200
#define	hwrVZ328UTransmitCTSDelta				0x0100
#define	hwrVZ328UTransmitData					0x00FF

// uMisc Register
#define  hwrVZ328UMiscBaudTest              0x8000
#define	hwrVZ328UMiscClkSrcUCLK				0x4000
#define	hwrVZ328UMiscForceParityErr			0x2000
#define	hwrVZ328UMiscLoopback					0x1000
#define  hwrVZ328UMiscBaudReset             0x0800
#define  hwrVZ328UMiscIRTestEn              0x0400
#define	hwrVZ328UMiscReservedMask				0x0300
#define	hwrVZ328UMiscRTSThruFIFO				0x0080
#define	hwrVZ328UMiscRTSOut						0x0040
#define	hwrVZ328UMiscIRDAEn						0x0020
#define	hwrVZ328UMiscLoopIRDA					0x0010
#define  hwrVZ328UMiscRXPolarityInv         0x0008
#define  hwrVZ328UMiscTXPolarityInv         0x0004
#define	hwrVZ328UMiscUnused						0x0003

// uNonIntPresc Register
#define	hwrVZ328UNonIntPrescSrcIntPresc		0x0000
#define	hwrVZ328UNonIntPrescSrcNonIntPresc	0x8000

#define	hwrVZ328UNonIntPrescSelDiv128			0x0000
#define	hwrVZ328UNonIntPrescSelDiv64			0x0100
#define	hwrVZ328UNonIntPrescSelDiv32			0x0200
#define	hwrVZ328UNonIntPrescSelDiv16			0x0300
#define	hwrVZ328UNonIntPrescSelDiv8			0x0400
#define	hwrVZ328UNonIntPrescSelDiv4			0x0500
#define	hwrVZ328UNonIntPrescSelDiv2			0x0600
#define	hwrVZ328UNonIntPrescSelDiv1			0x0700

#define	hwrVZ328UNonIntPrescStepMask			0x00FF
#define	hwrVZ328UNonIntPrescSelOffset			8

// see VZ docs for explanation of the following.  This provides the correct
// pulse width for IRDA, and is just a statement in the docs. The values don't
// appear to make sense, but I just push the buttons.

// This has been deprecated in favor of a prv function in the serial driver and
// the SerialMgr (depending on whether you have Bond or not) that calculates
// the value of this register based on the clock frequency you're running on.
// There was a bug when running EZ at 20MHz that caused the IrDA pulsewidth to
// get too short.

#if 0
#define 	hwrVZ328UNonIntPrescIRDAMode			(hwrVZ328UNonIntPrescSrcIntPresc \
															| hwrVZ328UNonIntPrescSelDiv32 \
															| (hwrVZ328UNonIntPrescStepMask & 0x20))
#endif

/************************************************************************
 * PWM Bits
 ************************************************************************/
// PWM register bits of interest
#define	hwrVZ328PWMControlEnable				0x0010
#define	hwrVZ328PWMControlEnableBit			4
#define	hwrVZ328PWMControlEnableIRQ			0x0040
#define	hwrVZ328PWMIRQStatus					0x0080
// PWM divider chain output selectors
#define	hwrVZ328PWMControlDivMask				0x0003
#define	hwrVZ328PWMControlDivBy2				0x0
#define	hwrVZ328PWMControlDivBy4				0x1
#define	hwrVZ328PWMControlDivBy8				0x2
#define	hwrVZ328PWMControlDivBy16				0x3
// PWM Repeat Counts
#define	hwrVZ328PWMRepeatSample1x				0x0
#define	hwrVZ328PWMRepeatSample2x				0x4
#define	hwrVZ328PWMRepeatSample4x				0x8
#define	hwrVZ328PWMRepeatSample8x				0xC
// PWM hardware constraints
#define	hwrVZ328PWMFifoSizeInBytes				4
#define	hwrVZ328PWMPrescalerMask				0x7F00
#define	hwrVZ328PWMPrescalerOffset				8

/************************************************************************
 * PLL Bits
 ************************************************************************/
#define	hwrVZ328PLLControlDisable				0x0008
#define	hwrVZ328PLLControlClkDisable			0x0010

#define	hwrVZ328PLLControlSysDMADiv2			0x0000
#define	hwrVZ328PLLControlSysDMADiv4			0x0100
#define	hwrVZ328PLLControlSysDMADiv8			0x0200
#define	hwrVZ328PLLControlSysDMADiv16			0x0300
#define	hwrVZ328PLLControlSysDMADiv1			0x0400

#define	hwrVZ328PLLControlPixDMADiv2			0x0000  
#define	hwrVZ328PLLControlPixDMADiv4			0x0800
#define	hwrVZ328PLLControlPixDMADiv8			0x1000
#define	hwrVZ328PLLControlPixDMADiv16			0x1800
#define	hwrVZ328PLLControlPixDMADiv1			0x2000

#define	hwrVZ328PLLControlPreSc1Div2			0x0080
#define	hwrVZ328PLLControlPreSc1Div1			0x0000

#define	hwrVZ328PLLControlPreSc2Div2			0x0020
#define	hwrVZ328PLLControlPreSc2Div1			0x0000

#define	hwrCZ328PLLControl32Periods			0x0000
#define	hwrCZ328PLLControl48Periods			0x0001
#define	hwrCZ328PLLControl64Periods			0x0002
#define	hwrCZ328PLLControl96Periods			0x0003

#define  hwrVZ328PLLFreqSelCLK32					0x8000

/************************************************************************
 * Real Time Clock (RTC) Bits
 ************************************************************************/

// RTC Control Register (RTCCTL) bits
#define hwrVZ328RTCControlRTCEnable				0x80
#define hwrVZ328RTCControlRefSelMask				0x20
#define hwrVZ328RTCControlRefSel38400				0x20		// 38.4 kHz reference frequency
#define hwrVZ328RTCControlRefSel32768				0x00		// 32.768 kHz reference frequency

// RTC Interrupt Enable Register (RTCIENR) bits
#define hwrVZ328RTCIntEnableSample7            0x8000
#define hwrVZ328RTCIntEnableSample6            0x4000
#define hwrVZ328RTCIntEnableSample5            0x2000
#define hwrVZ328RTCIntEnableSample4            0x1000
#define hwrVZ328RTCIntEnableSample3            0x0800
#define hwrVZ328RTCIntEnableSample2            0x0400
#define hwrVZ328RTCIntEnableSample1            0x0200
#define hwrVZ328RTCIntEnableSample0            0x0100
#define hwrVZ328RTCIntEnableHour               0x0020
#define hwrVZ328RTCIntEnableSec						0x0010
#define hwrVZ328RTCIntEnable24Hr					0x0008
#define hwrVZ328RTCIntEnableAlarm					0x0004
#define hwrVZ328RTCIntEnableMinute					0x0002
#define hwrVZ328RTCIntEnableStopWatch				0x0001

// RTC Interrupt Status Register bits
#define hwrVZ328RTCIntStatusSample7            0x8000
#define hwrVZ328RTCIntStatusSample6            0x4000
#define hwrVZ328RTCIntStatusSample5            0x2000
#define hwrVZ328RTCIntStatusSample4            0x1000
#define hwrVZ328RTCIntStatusSample3            0x0800
#define hwrVZ328RTCIntStatusSample2            0x0400
#define hwrVZ328RTCIntStatusSample1            0x0200
#define hwrVZ328RTCIntStatusSample0            0x0100
#define hwrVZ328RTCIntStatusHour               0x0020
#define hwrVZ328RTCIntStatusSec						0x0010
#define hwrVZ328RTCIntStatus24Hr					0x0008
#define hwrVZ328RTCIntStatusAlarm					0x0004
#define hwrVZ328RTCIntStatusMinute					0x0002
#define hwrVZ328RTCIntStatusStopWatch				0x0001

// RTC Alarm register (RTCALRM) masks and offsets
#define hwrVZ328RTCAlarmSecondsMask				0x0000003fL
#define hwrVZ328RTCAlarmSecondsOffset				0
#define hwrVZ328RTCAlarmMinutesMask				0x003f0000L
#define hwrVZ328RTCAlarmMinutesOffset				16
#define hwrVZ328RTCAlarmHoursMask					0x1f000000L
#define hwrVZ328RTCAlarmHoursOffset				24

// RTC hours/minutes/seconds register (RTCHMS) masks and offsets
#define hwrVZ328RTCHourMinSecSecondsMask			0x0000003fL
#define hwrVZ328RTCHourMinSecSecondsOffset		0
#define hwrVZ328RTCHourMinSecMinutesMask			0x003f0000L
#define hwrVZ328RTCHourMinSecMinutesOffset		16
#define hwrVZ328RTCHourMinSecHoursMask			0x1f000000L
#define hwrVZ328RTCHourMinSecHoursOffset			24

// RTC Day register masks/offsets
#define hwrVZ328RTCDayDayMask                  0x01FF
#define hwrVZ328RTCDayDayOffset                0

// RTC Watchdog Timer masks/offsets
#define hwrVZ328RTCWatchdogEnable              0x0001
#define hwrVZ328RTCWatchdogIntSel              0x0002
#define hwrVZ328RTCWatchdogIntStatus           0x0080
#define hwrVZ328RTCWatchdogCountMask           0x0300
#define hwrVZ328RTCWatchdogCountOffset         8

/************************************************************************
 * SPI Master bits
 ************************************************************************/
#define	hwrVZ328SPIMControlRateMask			0xE000
	#define	hwrVZ328SPIMControlRateDiv4			0x0000
	#define	hwrVZ328SPIMControlRateDiv8			0x2000
	#define	hwrVZ328SPIMControlRateDiv16			0x4000
	#define	hwrVZ328SPIMControlRateDiv32			0x6000
	#define	hwrVZ328SPIMControlRateDiv64			0x8000
	#define	hwrVZ328SPIMControlRateDiv128		0xA000
	#define	hwrVZ328SPIMControlRateDiv256		0xC000
	#define	hwrVZ328SPIMControlRateDiv512		0xE000
#define	hwrVZ328SPIMControlEnable				0x0200
#define	hwrVZ328SPIMControlExchange			0x0100
#define	hwrVZ328SPIMControlIntStatus			0x0080
#define	hwrVZ328SPIMControlIntEnable			0x0040
#define	hwrVZ328SPIMControlOppPhase			0x0020
#define	hwrVZ328SPIMControlInvPolarity		0x0010
#define	hwrVZ328SPIMControlBitsMask			0x000F


#endif // __M68VZ328_H__

#endif // NON_PORTABLE











