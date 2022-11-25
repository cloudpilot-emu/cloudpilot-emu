/******************************************************************************
 *
 * Copyright (c) 1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: HAL.h
 *
 * Description:
 *		General HAL Equates. This header file contains function prototypes for
 *  HAL routines, and is used by both Palm OS and the HAL module.
 *
 * History:
 *		 5/31/99	SCL		Created by Steve Lemke
 *
 *****************************************************************************/

#ifndef PUBLIC_STUFF_STRIPPED
// DOLATER еее Turned this off, because HAL calls *are* portable, right Steve?
#endif // PUBLIC_STUFF_STRIPPED

// #ifdef	NON_PORTABLE	   // So app's don't mistakenly include this

#ifndef __HAL_H__
#define __HAL_H__

#include <PalmOptErrorCheckLevel.h>


/**************************************************************************
 * HAL routine selectors
 ***************************************************************************/
/*
#define	halBootBase	0

typedef enum {
	halBootHwrPreDebugInit = halBootBase,		// 0
	halBootHwrInit,									// 1
	halBootHwrModelSpecificInit,					// 2
	halBootHwrEnableDataWrites,					// 3
	halBootHwrDisableDataWrites,					// 4
	halBootHwrMemReadable,							// 5
	halBootHwrMemWritable,							// 6
	halBootHwrResetNMI,								// 7
	halBootHwrResetPWM,								// 8
	
	halBootKeyBootKeys,								// 9
	
	halBootHwrDisplayCursor,						// 10
	
	halBootDrvOpen,									// 11
	halBootDrvClose,									// 12
	halBootDrvControl,								// 13
	halBootDrvStatus,									// 14
	halBootDrvWriteChar,								// 15
	halBootDrvReadChar,								// 16
	
	halBootLastSelector
	} halBootSelector;
*/

/************************************************************
 * New HAL function prototypes
 *************************************************************/

#if DISABLE_HAL_TRAPS
	#define HAL_CALL(trapNum) 
#else
 	#define HAL_CALL(trapNum) \
		_HAL_API(_CALL)(_HAL_TABLE, trapNum)
#endif

// HwrInit is passed as a parameter to InitStage1 in RomBoot.c
typedef void (*HwrInitProcPtr)(void);


/**************************************************************************
 * Prototypes of functions used only when running on the real hardware
 ***************************************************************************/
#ifdef REMOVE_FOR_EMULATOR
#ifdef __cplusplus
extern "C" {
#endif

	// These routines are "private" prototypes for the HAL code...

	void		HwrPreRAMInit(void); 
	
	void 		HwrInit(void);


#ifdef __cplusplus 
}
#endif
#endif


#endif 	//__HAL_H__

// #endif 	// NON_PORTABLE
