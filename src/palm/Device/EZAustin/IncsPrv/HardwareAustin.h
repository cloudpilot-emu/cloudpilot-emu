/******************************************************************************
 *
 * Copyright (c) 1995-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: HardwareAustin.h
 *
 * Description:
 *		Hardware Equates for the EZ (Austin) platform. This header file
 *  contains equates specific to Hardware platform EZ.
 *
 * History:
 *		03/04/99	ben	Created from HardwareEZ.h
 *		07/15/99	kwk	Added hwrDigitizerWidth & hwrDigitizerHeight.
 *
 *****************************************************************************/

#ifdef	NON_PORTABLE									// So app's don't mistakenly include this

#ifndef __HARDWAREAUSTIN_H__
#define __HARDWAREAUSTIN_H__


/***********************************************************************
 *  EZ Rev of the Pilot Hardware
 ***********************************************************************/
#define	hwr68328Base			0xFFFFF000L					// Base address of 68328 

// Define the base of card 0 and the size of each card slot.
#define	hwrCardBase0			0x00000000L					// starts here (just above Dheap)
#define	hwrCardSize				0x20000000L					// Size of each card slot
#define	hwrFlashBase			0x10C00000L					// Base of Flash ROM
//#define	hwrFlashSize			0x00400000L					// Size of Flash ROM
#define	hwrCardOffsetMask		0x1FFFFFFFL					// Used to convert a pointer to a card offset
#define	hwrMinBigROMOffset 	0x00008000L					// Minimum ROM offset to BigROM


// Define the Flash width
#define	hwrROMWidth				2


/************************************************************************
 * Port B Bit settings
 ************************************************************************/
#define	hwrEZPortBLCD_CS					0x01			// (L) LCD Chip Select
#define	hwrEZPortBCS_ADC					0x02			// (L) BurrBrown Chip-select
#define	hwrEZPortBLCDBright				0x08			// (L) LCD Brightness controller SYNC
#define	hwrEZPortBRS232Enable			0x40			// (H) Enable the RS232 Transceiver
#define	hwrEZPortBSoundUnfiltered		0x80			// (-) Unfiltered Sound


/************************************************************************
 * Port C Bit settings
 ************************************************************************/
#define	hwrEZPortCKbdRow0					0x01			// (H) Keyboard Row 0
#define	hwrEZPortCKbdRow1					0x02			// (H) Keyboard Row 1
#define	hwrEZPortCKbdRow2					0x04			// (H) Keyboard Row 2
#define	hwrEZPortCCharging				0x08			// (L) Battery Charging - Low if battery is charging.
#define	hwrEZPortCKbdRows					0x07			// All Keyboard Rows

#define	hwrEZPortCBacklightEnable		0x10			// (H) Enable SED1375 Backlight
#define	hwrEZPortCChargerShutdown		0x20			// (H) Output high to disable the charger
#define	hwrEZPortCEnable5V				0x40			// (H) Enable 5V power supply for screen
#define	hwrEZPortCEnableDTR				0x80			// (H) Assert Serial DTR signal.


/************************************************************************
 * Port D Bit settings
 ************************************************************************/

#define  hwrEZPortDKbdCol0             0x01     // (H) Keyboard Column 0		(aka INT0)
#define  hwrEZPortDKbdCol1             0x02     // (H) Keyboard Column 1		(aka INT1)
#define  hwrEZPortDKbdCol2             0x04     // (H) Keyboard Column 2		(aka INT2)
#define  hwrEZPortDKbdCol3             0x08     // (H) Keyboard Column 3		(aka INT3)
#define  hwrEZPortDKeyBits					0x0F     // All Keyboard Columns

#define	hwrEZPortDDockButton				0x10		// (L) IRQ1 HotSync/Dock Button
#define	hwrEZPortDGoodBattery			0x20		// (H) IRQ2 Good Battery
#define	hwrEZPortDAdapterInstalled		0x40		// (L) IRQ3 Adapter installed
#define	hwrEZPortDPowerFail				0x80		// (L) Power Fail interrupt (IRQ6)


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

#define  hwrEZPortDKeyOffset           0x0      // bits to shift to put Col 0 in lsb

/************************************************************************
 * Port E Bit settings
 ************************************************************************/
#define	hwrEZPortESpiTxD					0x01		// (L) SPI TXD (Internal)
#define	hwrEZPortESpiRxD					0x02		// (L) SPI RXD (Internal)
#define  hwrEZPortESpiClk					0x04		// SPI Clock
#define	hwrEZPortESpiBits					0x07		// all the SPI bits
#define  hwrEZPortERXD						0x10		// external RXD
#define  hwrEZPortETXD						0x20		// external TXD
#define	hwrEZPortERTS						0x40		// external RTS
#define  hwrEZPortECTS						0x80		// external CTS


/************************************************************************
 * Port F Bit settings
 ************************************************************************/

#define	hwrEZPortFLCDPowered				0x01		// (H) Input asserted when LCD is at full power.
#define	hwrEZPortFPenIO					0x02		// (H) Pen Interrupt, Pen IO (IRQ5)
#define	hwrEZPortFBusClock				0x04		// (H) Bus Clock
#define	hwrEZPortFA20						0x08		// (H) A20

#define	hwrEZPortFA21						0x10		// (H) A21
#define	hwrEZPortFVideoClkEnable		0x20		// (H) Enable Video clock.
#define	hwrEZPortFBatteryEnable			0x40		// (H) Battery Enable
#define	hwrEZPortFIXTRNL2					0x80		// (L) IXTRNL2 line, used to ID the device connected to the sync port.


/************************************************************************
 * Port G Bit settings
 ************************************************************************/

#define	hwrEZPortGDTACK						0x01		// (-) DTACK
#define	hwrEZPortGA0							0x02		// (-) A0
#define  hwrEZPortGIDDetect					0x04     // (L) ID select (drives kbd)
#define	hwrEZPortGHiZ							0x08		// (L) Used in ICT to tristate dragonball pins
#define	hwrEZPortGIRShutdown					0x10		// (H) Shutdown IRDA
#define	hwrEZPortGEmuBrk						0x20		// (-) EMUBRK


/************************************************************************
 * SPIM Base Configuration
 ************************************************************************/
#define	hwrEZSPIMBaseControl			(hwrEZ328SPIMControlRateDiv16 |		\
												 hwrEZ328SPIMControlIntEnable |		\
												 hwrEZ328SPIMControlInvPolarity |	\
												 hwrEZ328SPIMControlOppPhase  |  	\
												 16-1)



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

#define	hwrEZFreqSelPQ_15MHz		(0x1F | (0x09 << 8))

#define	hwrEZFreqSelPQ_16MHz		(0x23 | (0x01 << 8))

#define	hwrEZFreqSelPQ_20MHz		(0x2A | (0x09 << 8))


/************************************************************************
 * Display constants specific to Austin product.
 ************************************************************************/

#define	hwrDisplayBootDepth			8				// Boot time bits per pixel (system default)	

#define 	hwrDisplayWidth				160			// Physical Screen Width
#define 	hwrDisplayHeight				160			// Physical Screen Height
#define	hwrDisplayMaxDepth			8				// Maximum bits per pixel hardware supports

#define	hwrDigitizerWidth				hwrDisplayWidth	// Same as width of display
#define	hwrDigitizerHeight			220			// Height of digitizer area

// Screen refresh rate
#define	hwrDisplayPeriod				13				// frame period in ms.

// Brightness Control Adjuster special values
#define	hwrDisplayBrightAdjLowPwr	0x2000		// Sets special low power state for brightness adjust.


#endif 	// __HARDWAREAUSTIN_H__

#endif 	// NON_PORTABLE
