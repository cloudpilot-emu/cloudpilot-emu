/******************************************************************************
 *
 * Copyright (c) 1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: LibTraps.h
 *
 * Description:
 *      Palm OS Shared Library 'default' traps.
 *
 * History:
 *		7/15/99	Created by Bob Ebert
 *      mm/dd/yy   initials - brief revision comment
 *
 *****************************************************************************/

 #ifndef __LIBTRAPS_H_
 #define __LIBTRAPS_H_

// Include elementary types
#include <PalmTypes.h>

#if CPU_TYPE == CPU_68K
#include <M68KHwr.h>
#endif

//--------------------------------------------------------------------
// Define Library Trap Numbers
//--------------------------------------------------------------------
// Library traps start here and go up by 1's
#define	sysLibTrapBase		0xA800
typedef enum {
	sysLibTrapName = sysLibTrapBase,
	sysLibTrapOpen,
	sysLibTrapClose,
	sysLibTrapSleep,
	sysLibTrapWake,
	sysLibTrapCustom
	} SysLibTrapNumber;



#endif  //__LIBTRAPS_H_
