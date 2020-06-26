//******************************************************************
// Copyright (c) 1995-1999 Palm Computing, Inc. or its subsidiaries.
// All rights reserved.
//
// 							Pilot Software
//
//-------------------------------------------------------------------
// FileName:
//		HardwareVZ.h
//
// Description:
//		Hardware Equates for the VZ (Palm V) platform. This header file
//  contains equates specific to Hardware platform VZ.
//  It is included only in the modules "ROMHardwareVZ.c" and
//  "AMXHardwareVZ.c".
//
// History:
//
//			Name		Date			Description
//			----		----			------------
//			agk		2/1/00		Change name of port bits to match Battra's spec
//
//*******************************************************************

#ifdef	NON_PORTABLE		// So app's don't mistakenly include this

#ifndef __HARDWAREVZ_H__
#define __HARDWAREVZ_H__



/************************************************************************
 * Palm V/Palm IIIx/Palm IIIe.  Definitions from HardwarePrv.h.
 ***********************************************************************/
// Define the base of card 0 and the size of each card slot.
#define	hwrCardBase0		0x00000000L					// starts here (just above Dheap)
#define	hwrCardSize			0x20000000L					// Size of each card slot
#define	hwrFlashBase		0x10C00000L					// Base of Flash ROM
#define	hwrFlashSize		0x00200000L					// Size of Flash ROM
#define	hwrCardOffsetMask	0x1FFFFFFFL					// Used to convert a pointer to a card offset
#define	hwrMinBigROMOffset 0x00008000L				// Minimum ROM offset to BigROM


// Define the Flash width
#define	hwrROMWidth			2

//#define	USE_VZTRN_UART1_FOR_DEBUG		1		

/************************************************************************
 * Port B Bit settings
 ************************************************************************/

#define	hwrVZPortBUSB_CS_N				0x01		// (L) USB Chip Select #
#define	hwrVZPortBSDWE_N					0x02		// (L) SDRAM Control - SDWE#
#define	hwrVZPortBSDRAS_N					0x04		// (L) SDRAM Control - SDRAS#
#define	hwrVZPortBSDCAS_N					0x08		// (L) SDRAM Control - SDCAS#

#define	hwrVZPortBSDCS_N					0x10		// (L) SDRAM Control - SDCS#
#define	hwrVZPortBRadio_IN_SVC			0x20		// (L) Radio in-service signal
#define  hwrVZPortBALARM_LED				0x40		// (L) Alarm LED
#define	hwrVZPortBSoundUnfiltered		0x80		// (P) Unfiltered Sound	PWM

/************************************************************************
 * Port C Bit settings
 ************************************************************************/

#define	hwrVZPortCLCDData0				0x01		// (L) Mono LCD Data Bit 0
#define	hwrVZPortCLCDData1				0x02		// (L) Mono LCD Data Bit 1
#define	hwrVZPortCLCDData2				0x04		// (L) Mono LCD Data Bit 2
#define	hwrVZPortCLCDData3				0x08		// (L) Mono LCD Data Bit 3

#define	hwrVZPortCLCDFramePulse			0x10		// (L) Mono LCD Frame Pulse
#define	hwrVZPortCLCDLinePulse			0x20		// (L) Mono LCD Line Pulse
#define	hwrVZPortCLCDClock				0x40		// (L) Mono LCD Clock
#define	hwrVZPortCLCDOn					0x80		// (L) Enable the LCD controller

/************************************************************************
 * Port D Bit settings
 ************************************************************************/

#define  hwrVZPortDKbdCol0             0x01     // (H) Keyboard Column 0		(aka INT0)
#define  hwrVZPortDKbdCol1             0x02     // (H) Keyboard Column 1		(aka INT1)
#define  hwrVZPortDKbdCol2             0x04     // (H) Keyboard Column 2		(aka INT2)
#define  hwrVZPortDKbdCol3             0x08     // (H) Keyboard Column 3		(aka INT3)

#define	hwrVZPortDDockButton				0x10		// (L) HotSync/Dock Button		(aka IRQ1)
#define	hwrVZPortDRadio_RING_IN			0x20		// (L) Radio Ring Indicate		(aka IRQ2)
#define	hwrVZPortDUSB_INT_N				0x40		// (L) USB Interrupt				(aka IRQ3)
#define	hwrVZPortDPowerFail				0x80		// (L) Power Fail interrupt 	(aka IRQ6)

#define  hwrVZPortDKeyBits					0x0F     // All Keyboard Columns
#define  hwrVZPortDKeyOffset           0x0      // bits to shift to put Col 0 in lsb

// DANGER: Writing to PortD to (for example) toggle the serial enable bit has potential side effects.
// For example, if we |= or &= the serial enable bit (as PrvSerialEnable used to do) and inadvertently
// write other one-bits to portDData, we may clear pending edge interrupts on those other bits, like
// for example, the keyboard edge interrupt bits.  Though the docs are somewhat ambiguous,
// it would APPEAR that this is only the case for the keyboard interrupt bits (INT0-INT3).  This DID
// cause problems when re-enabling the serial port during HwrWake->SerialWake, as it would clear any
// pending keyboard interrupts that woke the device (such as the power key or application buttons).
// SO, when writing to the serial enable bit, we MUST be sure to mask against hwrVZPortDDataMaskBeforeWrite
// before writing to portDData.  This is ALSO VERY IMPORTANT for anyone who might make use of bit 6 (IRQ3)
// as an output pin.  Whenever writing to hwrVZPortDFree3, one should always read portDData first, twiddle
// hwrVZPortDFree3 as appropriate, and with hwrVZPortDDataMaskBeforeWrite and finally write to portDData.
// Maybe in future parts, Moto can spare us a PortDIntStatus register, separate from portDData.
#define  hwrVZPortDDataMaskBeforeWrite	(~(hwrVZPortDKeyBits))	// Don't write ones to these bits!

/************************************************************************
 * Port E Bit settings
 ************************************************************************/

#define	hwrVZPortESpiTxD					0x01		// (L) SPI TXD
#define	hwrVZPortESpiRxD					0x02		// (L) SPI RXD
#define  hwrVZPortESpiClk					0x04		// (L) SPI Clock
#define	hwrVZPortERadio_READY			0x08		// (L) Radio ready (XDR on radio)

#define  hwrVZPortERadio_RXD				0x10		// (H) RXD1# (used by Radio)
#define  hwrVZPortERadio_TXD				0x20		// (H) TXD1# (used by Radio)
#define	hwrVZPortETXD						hwrVZPortERadio_TXD	// keep the old name for now

#ifndef	USE_VZTRN_UART1_FOR_DEBUG					
#define	hwrVZPortERadio_RTS				0x40		// (L) Radio RTS
#define  hwrVZPortERadio_CTS				0x80		// (L) Radio CTS
#else		// USE_VZTRN_UART1_FOR_DEBUG					
#define	hwrVZPortERTS						0x40		// (L) RTS1#
#define  hwrVZPortECTS						0x80		// (L) CTS1#
#endif	// USE_VZTRN_UART1_FOR_DEBUG					

#define	hwrVZPortESpiBits					0x07		// all the SPI bits

/************************************************************************
 * Port F Bit settings
 ************************************************************************/
// Spitfire
#define	hwrVZPortFLCD_CONTRAST			0x01		// (P) LCD Contrast
#define	hwrVZPortFPenIO					0x02		// (L) Pen Interrupt, Pen IO (IRQ5)
#define	hwrVZPortFFree2					0x04		// (-) Free (CLKO)
#define	hwrVZPortFA20						0x08		// (P) A20

#define	hwrVZPortFA21						0x10		// (P) A21
#define	hwrVZPortFA22						0x20		// (-) A22
#define	hwrVZPortFRadio_DTR				0x40		// (L) DTR to radio (XER on radio)
#define	hwrVZPortFROM_CS					0x80		// (L) Mass ROM chip select

/************************************************************************
 * Port G Bit settings
 ************************************************************************/

#define	hwrVZPortGLCD_VCC_ON_N			0x01		// (L) LCD VCC control
#define	hwrVZPortGEL_ON					0x02		// (L) EL_ON
#define  hwrVZPortGFID_DETECT_N			0x04		// (-) ID_DETECT#
#define	hwrVZPortG232_SHDN_N				0x08		// (L) 232_SHDN#		// Obsolete! (2/10/00)
#define	hwrVZPortG232a_SHDN_N			0x08		// (L) 232_SHDN#

#define	hwrVZPortGUSB_SPND				0x10		// (H) USB_SPND
#define	hwrVZPortGADC_CS_N				0x20		// (H) ADC_CS#
#define	hwrVZPortGUnused6					0x40		// (-) Unused6
#define	hwrVZPortGUnused7					0x80		// (-) Unused7

/************************************************************************
 * Port J Bit settings
 ************************************************************************/

#define	hwrVZPortJRadio_ANT3				0x01		// (L) Radio RSSI 3
#define	hwrVZPortJRadio_ANT2				0x02		// (L) Radio RSSI 2
#define	hwrVZPortJRadio_ANT1				0x04		// (L) Radio RSSI 1
#define	hwrVZPortJRadio_PWR_STATE		0x08		// (L) Radio Power State

#define  hwrVZPortJRXD						0x10		// (L) RXD2#
#define  hwrVZPortJTXD						0x20		// (L) TXD2#
#define	hwrVZPortJRTS						0x40		// (L) RTS2#
#define  hwrVZPortJCTS						0x80		// (L) CTS2#

/************************************************************************
 * Port K Bit settings
 ************************************************************************/

	// Move KbdRowX to upper nibble so RW#, LDS#, & UDS# are available
	// to Logic Analyzer to allow for instruction disassembly.
	// Note: This requires a modified PCB.					
#define	hwrVZPortKRadio_PWR_KEY			0x01		// (L) Radio power on/off
#define	hwrVZPortKFree1					0x02		// (-) FREE1 (RW#)
#define	hwrVZPortKEncharge				0x04		// (L) Charging Enable
#define	hwrVZPortKFree3					0x08		// (-) Free 3

#define	hwrVZPortKRadio_CD				0x10		// (L) Radio Carrier Detect
#define	hwrVZPortKKbdRow0					0x20		// (H) Keyboard Row 0
#define	hwrVZPortKKbdRow1					0x40		// (H) Keyboard Row 1
#define	hwrVZPortKKbdRow2					0x80		// (H) Keyboard Row 2

#define	hwrVZPortKKbdRows					0xE0		//	All Keyboard Rows


/************************************************************************
 * Port M Bit settings
 ************************************************************************/

#define	hwrVZPortMSDClk					0x01		// (-) SDClk
#define	hwrVZPortMSDCE						0x02		// (-) SDCE
#define	hwrVZPortMSDDQMH					0x04		// (-) SDDQMH
#define	hwrVZPortMSDDQML					0x08		// (-) SDDQML

#define	hwrVZPortMSDA10					0x10		// (-) SDA10
// TEMPORARY - Tornado board uses this pin to enable either IrDA or UART2 VT (2/10/00)
//						0: IrDA is on
//						1: RS-232 Voltage Translator is on
#define	hwrVZPortMIR_SD					0x20		// (L) Infrared Shut-down (UART 2)
#define	hwrVZPortM232b_SHDN_N			0x20		// (L) Voltage Translator shutdown (UART 2)
#define	hwrVZPortMUnused6					0x40		// (-) Unused6
#define	hwrVZPortMUnused7					0x80		// (-) Unused7

/************************************************************************
 * SPIM Base Configuration
 ************************************************************************/
#define	hwrVZSPIMBaseControl		(hwrVZ328SPIMControlRateDiv16 |		\
												 hwrVZ328SPIMControlIntEnable |		\
												 hwrVZ328SPIMControlInvPolarity |	\
												 hwrVZ328SPIMControlOppPhase  |  	\
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
 *  32768 * 2 * ((hwrVZFreqSelP + 1) * 14 + hwrVZFreqSelQ + 1)
 *   --------------------------------------------------------
 *                          VCO divider
 ************************************************************************/
// Dragonball VZ runs at 33.161216 Mhz
#define	hwrVZPLLControl			(hwrVZ328PLLControlSysDMADiv1 | 	\
											 hwrVZ328PLLControlPixDMADiv4 |  \
											 hwrVZ328PLLControlPreSc1Div1 |  \
											 hwrVZ328PLLControlPreSc2Div1)
											 
// The values are:
// 33.16MHz:      	P = 0x23,  Q=0x01
// 26.01MHz:   		P = 0x1B,  Q=0x04
//
#define	hwrVZFreqSelPQ_33_16MHz	(0x23 | (0x01 << 8))

#define	hwrVZFreqSelPQ_26_01MHz	(0x1B | (0x04 << 8))


/***** Oboselete!  Should be removed eventually - agk 03/01/00 ***/
#define	hwrVZPortERadioXDR	hwrVZPortERadio_READY
#define	hwrVZPortERadioRXD	hwrVZPortERadio_RXD
#define	hwrVZPortERadioTXD	hwrVZPortERadio_TXD
#define	hwrVZPortERadioRTS	hwrVZPortERadio_RTS
#define	hwrVZPortERadioCTS	hwrVZPortERadio_CTS




#endif 	//__HARDWAREVZ_H__

#endif 	// NON_PORTABLE
