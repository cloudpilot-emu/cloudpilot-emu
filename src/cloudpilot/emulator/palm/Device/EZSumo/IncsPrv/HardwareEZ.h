/******************************************************************************
 *
 * Copyright (c) 1995-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: HardwareEZ.h
 *
 * Description:
 *		Hardware Equates for the EZ (Razor) platform. This header file
 *  contains equates specific to Hardware platform EZ.
 *  It is included only in the modules "ROMHardwareEZ.c" and
 *  "AMXHardwareEZ.c".
 *
 * History:
 *   11/14/97  srj Started life from HardwareTD1.h
 *		3/30/99	JB	Added hwrEZPortDJerryAntenna
 *		3/31/99	JB Added more Jerry pin definitions
 *    6/04/99	srj Added EZ specific stuff from HardwarePrv.h
 *
 *****************************************************************************/

#ifdef	NON_PORTABLE									// So app's don't mistakenly include this

#ifndef __HARDWAREEZ_H__
#define __HARDWAREEZ_H__



/************************************************************************
 * Sumo/Brad/Sparky.  Definitions from HardwarePrv.h.
 ***********************************************************************/
// Define the base of card 0 and the size of each card slot.
#define	hwrCardBase0		0x00000000L					// starts here (just above Dheap)
#define	hwrCardSize			0x20000000L					// Size of each card slot
#define	hwrFlashBase		0x10C00000L					// Base of Flash ROM
//#define	hwrFlashSize		0x00200000L					// Size of Flash ROM
#define	hwrCardOffsetMask	0x1FFFFFFFL					// Used to convert a pointer to a card offset
#define	hwrMinBigROMOffset 0x00008000L				// Minimum ROM offset to BigROM


// Define the Flash width
#define	hwrROMWidth			2

/************************************************************************
 * Jerry EZ cost reduction.  Set the PLD address.
 ***********************************************************************/
#define	hwrJerryPldBase		0x11000000L



/************************************************************************
 * Port B Bit settings
 ************************************************************************/
#define	hwrEZPortBBacklightOn			0x01			// Backlight (Cobra 2)
#define	hwrEZPortBFree1					0x01			// (-) FREE1
#define	hwrEZPortBLCDVccOff				0x02			// (L) LCD Vcc On
#define	hwrEZPortBLCDAdjOn				0x40			// (L) LCD Contrast Adjust On (Brad Rev 0)
#define	hwrEZPortBFree4					0x40			// (-) FREE4
//#define  hwrEZPortBIORail2          	0x40			// (L) I/O Rail Pin 2

//#define 	hwrEZPortBIORail2Bit				6				// Bit number of Rail Pin 2

/************************************************************************
 * Port C Bit settings
 ************************************************************************/
#define	hwrEZPortCLCDEnableOn			0x80			// (H) LCD Enable
/************************************************************************
 * Port D Bit settings
 ************************************************************************/
#define	hwrEZPortDDockButton				0x10		// (L) Dock Button				(aka IRQ1) (edge, negative)
#define  hwrEZPortD232Enable				0x20		// (L) Serial Shutdown on Sumo Rev 1	(aka IRQ2)
#define	hwrEZPortDJerryAntenna			0x20		// Jerry Antenna interrupt (aka IRQ2)
//#define	hwrEZPortDDockIn				0x20		// (L) Dock Input (free on Sumo Rev 0)	(aka IRQ2)
#define	hwrEZPortDFree3					0x40		// (-) FREE3						(aka IRQ3)
//#define	hwrEZPortDIORail1				0x40		// (L) IO Rail 1					(aka IRQ3)
#define	hwrEZPortDPowerFail				0x80		// (L) Power Fail Interrupt	(aka IRQ6) (level, low)
#define  hwrEZPortDKbdCol0             0x01     // (H) Keyboard Column 0		(aka INT0)
#define  hwrEZPortDKbdCol1             0x02     // (H) Keyboard Column 1		(aka INT1)
#define  hwrEZPortDKbdCol2             0x04     // (H) Keyboard Column 2		(aka INT2)
#define  hwrEZPortDKbdCol3             0x08     // (H) Keyboard Column 3		(aka INT3)
#define  hwrEZPortDKeyBits					0x0F     // (H) All Keyboard Columns

// DANGER: Writing to PortD to (for example) toggle the serial enable bit has potential side effects.
// For example, if we |= or &= the serial enable bit (as PrvSerialEnable used to do) and inadvertently
// write other one-bits to portDData, we may clear pending edge interrupts on those other bits, like
// for example, the keyboard edge interrupt bits.  Though the docs are somewhat ambiguous (surprise!),
// it would APPEAR that this is only the case for the keyboard interrupt bits (INT0-INT3).  This DID
// cause problems when re-enabling the serial port during HwrWake->SerialWake, as it would clear any
// pending keyboard interrupts that woke the device (such as the power key or application buttons).
// SO, when writing to the serial enable bit, we MUST be sure to mask against hwrEZPortDDataMaskBeforeWrite
// before writing to portDData.  This is ALSO VERY IMPORTANT for anyone who might make use of bit 6 (IRQ3)
// as an output pin.  Whenever writing to hwrEZPortDFree3, one should always read portDData first, twiddle
// hwrEZPortDFree3 as appropriate, and with hwrEZPortDDataMaskBeforeWrite and finally write to portDData.
// Maybe in future parts, Moto can spare us a PortDIntStatus register, separate from portDData.
#define  hwrEZPortDDataMaskBeforeWrite	(~(hwrEZPortDKeyBits))	// Don't write ones to these bits!

// OBSOLETE
#define	hwrEZPortDKeyRow0					0x0F		// (H) App keys on row 0
#define	hwrEZPortDKeyRow1					0x3		// (H) PgUp/PgDn on row 1
#define	hwrEZPortDKeyRow2					0x3		// (H) Power and Contrast on row 2
#define  hwrEZPortDKeyOffset           0x0      // bits to shift to put Col 0 in msb

//#define  hwrEZPortDIORail1Bit				6			// Bit number of Rail Pin 1
//#define	hwrEZPortDIntRailBit				3			// Bit number for Rail Pin 1 in Port D interrupt reg.


/************************************************************************
 * Port E Bit settings
 ************************************************************************/
#define	hwrEZPortESpiTxD					0x01		// (L) SPI TXD (Internal)
#define	hwrEZPortESpiRxD					0x02		// (L) SPI RXD (Internal)
#define  hwrEZPortESpiClk					0x04		// SPI Clock
#define	hwrEZPortESpiBits					0x07		// all the SPI bits
#define  hwrEZPortERXD						0x10		// external RXD
#define  hwrEZPortETXD						0x20		// external TXD
#define  hwrEZPortECTS						0x80		// external CTS

/************************************************************************
 * Port F Bit settings
 ************************************************************************/
#define	hwrEZPortFContrastPWM		0x01			// (L) PWM output for LCD contrast (Sumo)
#define	hwrEZPortFLCDAdjOn			0x01			// (L) LCD Contrast Adjust On (Brad Rev 1)
#define 	hwrEZPortFJerryFReset		0x01			// Jerry flash reset
#define	hwrEZPortFPenIO				0x02			// (L) Pen Interrupt,Pen I/O
#define	hwrEZPortF232Enable			0x04			// (L) Shutdown Serial
#define	hwrEZPortFJerryPlatform		0x04			// Jerry-specific HW detect
#define	hwrEZPortFCobra2KbdRow0		0x20			// (L) Keyboard Row 0 (Cobra 2)	
#define	hwrEZPortFCobra2KbdRow1		0x40			// (L) Keyboard Row 1 (Cobra 2)
#define	hwrEZPortFCobra2KbdRow2		0x80			// (L) Keyboard Row 2 (Cobra 2)
#define  hwrEZPortFCobra2KbdRows		0xE0        // (L) All Keyboard Rows (Cobra 2)
#define	hwrEZPortFKbdRow0				0x10			// (L) Keyboard Row 0	(Sumo/Brad)
#define	hwrEZPortFKbdRow1				0x20			// (L) Keyboard Row 1	(Sumo/Brad)	
#define	hwrEZPortFKbdRow2				0x40			// (L) Keyboard Row 2	(Sumo/Brad)	
#define  hwrEZPortFKbdRows				0x70        // (L) All Keyboard Rows	(Sumo/Brad)
#define	hwrEZPortFBacklightOn		0x80			// (L) Backlight on		(Sumo/Brad)
/************************************************************************
 * Port G Bit settings
 ************************************************************************/
#define hwrEZPortGDTACK					0x01			// (L) DTACK
#define hwrEZPortFree5					0x02			// (-) FREE5
#define  hwrEZPortGIDDetect			0x04        // (L) ID select (drives kbd)
#define	hwrEZPortGLiIonVoltReadDis	0x08			// (L) Enable A/D Voltage Divider
#define	hwrEZPortGJerry12Venable	0x08			// Jerry 12Volt enable for flash
#define	hwrEZPortGIRShutdown			0x10			// (H) Shutdown IRDA
#define	hwrEZPortGADCOff				0x20			// (L) A/D Select


/************************************************************************
 * SPIM Base Configuration
 ************************************************************************/
#define	hwrEZSPIMBaseControl		(hwrEZ328SPIMControlRateDiv16 |		\
												 hwrEZ328SPIMControlIntEnable |		\
												 hwrEZ328SPIMControlInvPolarity |	\
												 hwrEZ328SPIMControlOppPhase  |  	\
												 16-1)


/************************************************************************
 * Macros for turning write-protection on/off
 * These are used in-line for MemSemaphoreReserve and MemSemaphoreRelease
 *  for performance reasons.
 ************************************************************************/
#define	hwrWriteProtectOff()														\
	{																						\
	HwrDBallPtr baseP = (HwrDBallPtr)hwr68328Base;							\
																							\
	/* Clr read-only bit in chip select register for storage area */	\
	baseP->csDSelect &= ~(0x2000);												\
																							\
	}

#ifndef	BUILD_OPTION_WP_OFF
#define	hwrWriteProtectOn()														\
	{																						\
	HwrDBallPtr baseP = (HwrDBallPtr)hwr68328Base;							\
																							\
	/* Set read-only bit in chip select register for storage area */	\
	baseP->csDSelect |= (0x2000);													\
																							\
	}
#else
#define	hwrWriteProtectOn()	
#endif


/************************************************************************
 *Initial values for the PLL. We set the system clock to be VCO/1 so
 *  the system clock speed becomes:
 *
 *  32768 * ((hwrEZFreqSelP + 1) * 14 + hwrEZFreqSelQ + 1)
 *   --------------------------------------------------------
 *                          VCO divider
 ************************************************************************/
// NEW ASIC versions run at 16.580608 Mhz
#define	hwrEZPLLControl			(hwrEZ328PLLControlClkEn | 		\
											 hwrEZ328PLLControlSysDMADiv1 | 	\
											 hwrEZ328PLLControlPixDMADiv1 |  \
											 hwrEZ328PLLControlDMAVCODiv1)
// The Frequency needs to be lowered on the older DragonBall-EZ processors.
//
// The values are:
// 15MHz:      		Allows processor to operate without screen noise within the voltage
//             		range specified for the Power Supply (3.3V +- 100mV)
//             		P = 0x1F,  Q=0x9
// 13.5 MHz:   		Brings processor into voltage spec (runs as low as 3.0V without noise)
//             		P = 0x1C,  Q=0x7
//
// The following are the OK values for 0J83C processors (id 4)
// 16.580608 is the spec'd frequency for the part. HK admits that 20MHz works
// 16.580608MHz:	P = 0x23,	Q=0x01
// 20.0 MHz:   		P = 0x2A,	Q=0x9

#define	hwrEZFreqSelPQ_13_5MHz	(0x1C | (0x07 << 8))

#define	hwrEZFreqSelPQ_14_75MHz	(0x1F | (0x01 << 8))

#define	hwrEZFreqSelPQ_15MHz	(0x1F | (0x09 << 8))

#define	hwrEZFreqSelPQ_16MHz	(0x23 | (0x01 << 8))

#define	hwrEZFreqSelPQ_20MHz	(0x2A | (0x09 << 8))


// Time needed to get to full battery voltage on EZ
// this is used only in case of a constant-voltage charger, and Razor doesn't use that
// right now.  See PrvSystemTimerProc() for more gory details.
#if 0
#define  hwrEZSecToFullCharge		60
#endif

// Bit definitions for Miscflags that are needed for the EZSumo HAL
//#define	hwrEZSubIDCobra2Mask 0x6
#define	hwrMiscFlagExtNewKeyboard		0x00000006		// This value tells us we have Cobra2 keyboard
#define	hwrMiscFlagExtNewKeyboardMask	0x00000006		// Mask for the Cobra2 bits
#define	hwrMiscFlagExt20MHzSpeed		0x00000001		// Set on Cobra 2 for 20MHz operation

#endif 	//__HARDWAREEZ_H__

#endif 	// NON_PORTABLE
