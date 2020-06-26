/******************************************************************************
 *
 * Copyright (c) 1996-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: Localize.h
 *
 * Description:
 *		Functions to localize data.
 *
 * History:
 *   	8/28/96  Roger - Initial version
 *
 *****************************************************************************/

#ifndef __LOCALIZE_H__
#define __LOCALIZE_H__


// Include elementary types
#include <PalmTypes.h>					// Basic types
#include <CoreTraps.h>				// Trap Numbers.

// The number format (thousands separator and decimal point).  This defines
// how numbers are formatted and not neccessarily currency numbers (i.e. Switzerland).
typedef enum {
	nfCommaPeriod,
	nfPeriodComma,
	nfSpaceComma,
	nfApostrophePeriod,
	nfApostropheComma
	} NumberFormatType;

	

#ifdef REMOVE_FOR_EMULATOR
#ifdef __cplusplus
extern "C" {
#endif


void 		LocGetNumberSeparators(NumberFormatType numberFormat, 
				Char *thousandSeparator, Char *decimalSeparator)
							SYS_TRAP(sysTrapLocGetNumberSeparators);



#ifdef __cplusplus 
}
#endif
#endif


#endif	// __LOCALIZE_H__
