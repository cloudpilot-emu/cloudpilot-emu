/******************************************************************************
 *
 * Copyright (c) 1994-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: FatalAlert.h
 *
 * Description:
 *        This file defines the system Fatal Alert support.
 *
 * History:
 *		September 12, 1994	Created by Art Lamb
 *
 *****************************************************************************/

#ifndef __FATALALERT_H__
#define __FATALALERT_H__

#include <PalmTypes.h>
#include <CoreTraps.h>

// Value returned by SysFatalAlert
#define fatalReset			0
#define fatalEnterDebugger	1
#define fatalDoNothing		0xFFFFU

#ifdef REMOVE_FOR_EMULATOR
#ifdef __cplusplus
extern "C" {
#endif

UInt16 SysFatalAlert (const Char *msg)
		SYS_TRAP(sysTrapSysFatalAlert);

void SysFatalAlertInit (void)
		SYS_TRAP(sysTrapSysFatalAlertInit);

#ifdef __cplusplus 
}
#endif
#endif

#endif  // __FATALALERT_H__
