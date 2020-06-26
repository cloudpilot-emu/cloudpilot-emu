/******************************************************************************
 *
 * Copyright (c) 1995-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: Hardware.h
 *
 * Description:
 *		General Hardware Equates. This header file
 *  contains general hardware configuration equates that are not dependent on
 *  the version of the hardware we're running on. Equates specific to
 *  a certain component like the 68328, etc. are contained in separate
 *  header files.
 *
 * History:
 *		 4/27/95	RM		Created by Ron Marianetti
 *		 6/12/98	jhl	Updated definitions for hwrNVPrefSet/Get
 *		 6/17/98	jhl	added 'mbtx' ROM token def
 *		 8/17/98	SCL	Synchronized HwrMiscFlags between Sumo/Eleven
 *		 9/14/98	jfs	Added PrvKeyBootKeys def (see ROMBoot.c for info)
 *		10/06/98	jfs	Added software-only hardware ID and features detection defs
 *		10/07/98	jfs	Cleaned up hardware ID bit defs and made unused combos obvious
 *		10/07/98	Bob	Up max depth support to 8 bits per pixel
 *		01/18/99	kwk	Added Sumo TSM button heights. 
 *		05/27/99	lyl	Added hwrROMTokenBoot7 and hwrROMTokenBoot8
 *		06/14/99	SCL	Split out most pieces to separate Hwr*.h files
 *		07/14/99	kwk	Addded HwrGetSilkscreenID.
 *
 *****************************************************************************/

#ifdef	NON_PORTABLE	   // So app's don't mistakenly include this

#ifndef __HARDWARE_H__
#define __HARDWARE_H__

#include <CoreTraps.h>
#include <HAL.h>				// NOTE: needs to be reflected in Makefiles


// Define # of card slots
#if EMULATION_LEVEL == EMULATION_NONE
#define	hwrNumCardSlots	1								// 1 card slot
#else
#define	hwrNumCardSlots	2								// 2 card slots for emulation
#endif


// HwrLCDGetDepth requests
#define	hwrLCDGetCurrentDepth			0				// Get depth display is currently set to

// HwrGetSilkscreenID results
// DOLATER kwk - should these go here or in UICommon.h?
// DOLATER kwk - does Austin have a different layout?
#define	silkscreenOriginal				10000
#define	silkscreenPalmV					10001			// Screen is slightly narrower at bottom.

#define	silkscreenCustom					19000			// First id for custom silkscreens

/**************************************************************************
 * Prototypes of functions used only when running on the real hardware
 ***************************************************************************/
#ifdef REMOVE_FOR_EMULATOR
#ifdef __cplusplus
extern "C" {
#endif

	// HAL routines installed into system traps from ROMHardwareXXX.c:
	void		HwrPreDebugInit(UInt32 cardHeaderAddr)
						HAL_CALL(sysTrapHwrPreDebugInit);
	
	void		HwrResetNMI(void)
						HAL_CALL(sysTrapHwrResetNMI);
	
	void		HwrResetPWM(void)
						HAL_CALL(sysTrapHwrResetPWM);
	
	UInt32	HwrMemWritable(void *addr)
						HAL_CALL(sysTrapHwrMemWritable);
	
	UInt32	HwrMemReadable(void *addr)
						HAL_CALL(sysTrapHwrMemReadable);
	
	Boolean	HwrEnableDataWrites(void)
						HAL_CALL(sysTrapHwrEnableDataWrites);
	
	void		HwrDisableDataWrites(void)
						HAL_CALL(sysTrapHwrDisableDataWrites);


	// HAL routines NOT installed into system traps from ROMHardwareXXX.c:
	void		PrvShutDownPLL(void);


	// HAL routines installed into system traps from ROMHiHardwareXXX.c:
	void		HwrDoze(Boolean onlyNMI)
						HAL_CALL(sysTrapHwrDoze);
	
	Err		HwrSetSystemClock(UInt32 *freqP)
						HAL_CALL(sysTrapHwrSetSystemClock);
							
	Err		HwrSetCPUDutyCycle(UInt16 *dutyP)
						HAL_CALL(sysTrapHwrSetCPUDutyCycle);
							
	void 		HwrGetRAMMapping(void * /*CardInfoPtr*/ cardInfoP, UInt16 *numBlocksP, 
						UInt32 *physBlockInfoP, UInt32 *logBlockInfoP)
						HAL_CALL(sysTrapHwrGetRAMMapping);
				
	void		HwrSleep(Boolean untilReset, Boolean emergency)
						HAL_CALL(sysTrapHwrSleep);
	
	Err		HwrWake(void)
						HAL_CALL(sysTrapHwrWake);
	
	void		HwrPostDebugInit(void)
						HAL_CALL(sysTrapHwrPostDebugInit);
	
	void		HwrIdentifyFeatures(void)
						HAL_CALL(sysTrapHwrIdentifyFeatures);
	
	void		HwrModelSpecificInit(void)
						HAL_CALL(sysTrapHwrModelSpecificInit);

	void		HwrModelInitStage2(void)
						HAL_CALL(sysTrapHwrModelInitStage2);


	// Other routines:

	UInt16	HwrGetSilkscreenID(void)
						HAL_CALL(sysTrapHwrGetSilkscreenID);

	void		HwrWakeupIntHandler(void);

/*	
	void		PrvExecuteROMToken1(void);


	void		HwrRadioShutdown(void);
*/							

#ifdef __cplusplus 
}
#endif
#endif


/************************************************************
 * Assembly Function Prototypes
 *************************************************************/
#define	_HwrWake		\
				ASM_SYS_TRAP(sysTrapHwrWake)


#endif 	//__HARDWARE_H__

#endif 	// NON_PORTABLE
