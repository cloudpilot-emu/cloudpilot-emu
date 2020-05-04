/******************************************************************************
 *
 * Copyright (c) 1995-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: HardwareTD1.h
 *
 * Description:
 *		Hardware Equates for the TD1 platform. This header file
 *  contains equates specific to Hardware platform TD1.
 *  It is included only in the modules "ROMHardwareTD1.c" and
 *  "AMXHardwareTD1.c".
 *
 * History:
 *		6/13/95  RM		Created by Ron Marianetti
 *		6/ 8/98  jhl	Added defines for PF3 & PE6 for Jerry DVT
 *		6/ 8/98  jhl	Added defines for PF3 & PE6 for Jerry DVT
 *		6/ 8/98  jhl	Added defines for PF3 & PE6 for Jerry DVT
 *		5/31/99	SCL	Moved TD1-specific stuff into this file from <HardwarePrv.h>
 *
 *****************************************************************************/

#ifdef	NON_PORTABLE									// So app's don't mistakenly include this

#ifndef __HARDWARETD1_H__
#define __HARDWARETD1_H__



/***********************************************************************
 *  First Rev of the Pilot Hardware
 ***********************************************************************/

// Define the base of card 0 and the size of each card slot.
#define	hwrCardBase0		0x10000000L					// starts here
#define	hwrCardSize			0x10000000L					// Size of each card slot
#define	hwrFlashBase		0x10C00000L					// Base of Flash ROM
#define	hwrFlashSize		0x00200000L					// Size of Flash ROM
#define	hwrCardOffsetMask	0x0FFFFFFFL					// Used to convert a pointer to a card offset
#define	hwrMinBigROMOffset 0x00008000L				// Minimum ROM offset to BigROM


// Define the Flash width
#define	hwrROMWidth			2

//  Define location and setup for the 68681 Duart
#define	hwrDuartBase		0x10E00000				// Base address of DUART

#define	hwrDuartClock		3686400					// in Hertz (3.6864 MHz)
 
// Setting of ACR register except for baud rate select 
// Timer mode - Crystal clock /1
#define	hwrDuartACRReg		0x60					



/************************************************************************
 * Dragonball Port D Bit settings
 ************************************************************************/
#define	hwrTD1PortDNoExtPower		0x80			// (L) External DC input

/************************************************************************
 * Dragonball Port E Bit settings
 ************************************************************************/
// used in Jerry except EVT
#define	hwrTD1PortEPLLLock			0x40			// (IH) Jerry - Indicates PLL lock

#define	hwrTD1PortENoBacklight		0x80			// (H) high if no backlight present

/************************************************************************
 * Dragonball Port F Bit settings
 ************************************************************************/
// following 4 used in Jerry except EVT
#define	hwrTD1PortFDSPOn				0x01			// (H) Jerry - DSP Power On
#define	hwrTD1PortFChrgOn				0x02			// (H) Jerry - NiCad Charge On
#define	hwrTD1PortF3vRefOn			0x04			// (H) Jerry - 3v Reference On
#define	hwrTD1PortFPABiasOn			0x08			// (H) Jerry - PA Bias On (and NiCd sense)

#define	hwrTD1PortFPanelYPOff		0x01			// (L) Vcc to Y Panel
#define	hwrTD1PortFPanelYMOn			0x02			// (H) Gnd to Y Panel
#define	hwrTD1PortFPanelXPOff		0x04			// (L) Vcc to X Panel
#define	hwrTD1PortFPanelXMOn			0x08			// (H) Gnd to X Panel
#define	hwrTD1PortFLCDEnableOn		0x10			// (H) LCD Display on	
#define	hwrTD1PortFLCDVccOff			0x20			// (L) LCD Vcc		
#define	hwrTD1PortFLCDVeeOn			0x40			// (H) LCD Vee
#define	hwrTD1PortFADCSOff			0x80			// (L) A/D Chip Select


// Configurations of the panel control bits
#define	hwrTD1PortFPanelMask			0x0F			// Panel control bits

// Configure for lowest power
#define	hwrTD1PortFPanelCfgOff			(hwrTD1PortFPanelYPOff | 		\
												 	 hwrTD1PortFPanelXPOff)

// Configure to generate pen interrupt
#define	hwrTD1PortFPanelCfgPenIRQ		(hwrTD1PortFPanelXMOn | 		\
								 				 	 hwrTD1PortFPanelYPOff | 		\
												 	 hwrTD1PortFPanelXPOff)

// Configure to Measure Y Position
#define	hwrTD1PortFPanelCfgYMeas		(hwrTD1PortFPanelXMOn | 		\
								 				 	 hwrTD1PortFPanelYPOff)

// Configure to Measure X Position
#define	hwrTD1PortFPanelCfgXMeas		(hwrTD1PortFPanelXPOff | 		\
								 				 	 hwrTD1PortFPanelYMOn)


/************************************************************************
 * Dragonball Port G Bit Assignments
 ************************************************************************/
#define	hwrTD1PortGSerialOn			0x08			// (H) Serial Driver Enable
#define	hwrTD1PortGBattOff			0x10			// (L) Battery Voltage Measure
#define	hwrTD1PortGRefreshOut		0x20			// (L) Timer 1 output to signal refresh
#define	hwrTD1PortGSelfRefOff		0x40			// (L) PSRAM Self Refresh mode
#define	hwrTD1PortGSelfRefOffBit	6				//  Bit # for above
#define	hwrTD1PortGBacklightOn		0x80			// (H) Turn on backlight

/************************************************************************
 * Dragonball Port J Bit Assignments
 ************************************************************************/
#define	hwrTD1PortJDSPReset			0x08			// (H) Jerry - DSP Reset
#define	hwrTD1PortJIrOn				0x10			// (L) IrDA Driver Enable
#define	hwrTD1PortJLedOn				0x20			// (H) LED On
// following 2 used in EVT Jerry only
#define	hwrTD1PortJChrgOn				0x40			// (H) Jerry - NiCad Charge On
#define	hwrTD1PortJDSPOn				0x80			// (H) Jerry - DSP Power On

/************************************************************************
 * Dragonball Port M Bit Assignments
 ************************************************************************/
#define	hwrTD1PortMVccFail			0x04			// (L) Input indicates 3.3v is too low
#define	hwrTD1PortMCardIRQ			0x08			// (L) IRQ3 input from card
#define	hwrTD1PortMUnused4			0x10			// Unused (IRQ2)
#define	hwrTD1PortMDockButton		0x20			// (L) Sync button from Serial Doc
#define	hwrTD1PortMPenIO				0x40			// Pen input/output
#define	hwrTD1PortMDockIn				0x80			// (L) Gen. Purpose input from Serial Doc



/************************************************************************
 * SPIM Base Configuration
 ************************************************************************/
#define	hwrTD1SPIMBaseControl		(hwr328SPIMControlRateDiv16 |		\
												 hwr328SPIMControlIntEnable |		\
												 hwr328SPIMControlInvPolarity |	\
												 hwr328SPIMControlOppPhase  |  	\
												 16-1)


#if 0
				// We can't use these macros in Palm OS any more.
				// This code is now access through the HAL routines
				// HwrEnableDataWrites and HwrDisableDataWrites.

/************************************************************************
 * Macros for turning write-protection on/off
 * These are used in-line for MemSemaphoreReserve and MemSemaphoreRelease
 *  for performance reasons.
 ************************************************************************/
#define	hwrWriteProtectOff()														\
	{																						\
	HwrDBallPtr baseP = (HwrDBallPtr)hwr68328Base;							\
	/* UInt16			mask; */														\
																							\
	/* DOLATER.. take this out....	*/											\
	/* mask = baseP->csAGroupMask;	*/											\
																							\
	/* Clr read-only bit in chip select register for storage area */	\
	baseP->csASelect1 &= ~(0x0008);												\
	baseP->csASelect3 &= ~(0x0008);												\
																							\
	/* DOLATER.. take this out... */												\
	/*baseP->csAGroupMask = mask;	*/												\
	}

#ifndef	BUILD_OPTION_WP_OFF
#define	hwrWriteProtectOn()														\
	{																						\
	HwrDBallPtr baseP = (HwrDBallPtr)hwr68328Base;							\
	/* UInt16			mask; */														\
																							\
	/* DOLATER.. take this out....	*/											\
	/* mask = baseP->csAGroupMask;	*/											\
																							\
	/* Set read-only bit in chip select register for storage area */	\
	baseP->csASelect1 |= (0x0008);												\
	baseP->csASelect3 |= (0x0008);												\
																							\
	/* DOLATER.. take this out... */												\
	/*baseP->csAGroupMask = mask;	*/												\
	}
#else
#define	hwrWriteProtectOn()	
#endif

#endif


/************************************************************************
 *Initial values for the PLL. We set the system clock to be VCO/2 so
 *  the system clock speed becomes:
 *
 *  32768 * ((hwrTD1FreqSelP + 1) * 14 + hwrTD1FreqSelQ + 1)
 *   --------------------------------------------------------
 *                            2
 ************************************************************************/
// NEW ASIC versions run at 16.580608 Mhz
#define	hwrTD1PLLControl			(hwr328PLLControlClkEn | 			\
											 hwr328PLLControlSysVCODiv1 | 	\
											 hwr328PLLControlPixVCODiv1)
#define	hwrTD1FreqSelQ				0x01
#define	hwrTD1FreqSelP				0x23
#define	hwrTD1Frequency			(32768*((hwrTD1FreqSelP+1)*14+hwrTD1FreqSelQ+1)/1)


 
#endif 	//__HARDWARETD1_H__

#endif 	// NON_PORTABLE
