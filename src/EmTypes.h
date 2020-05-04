/* -*- mode: C++; tab-width: 4 -*- */
/* ===================================================================== *\
	Copyright (c) 2000-2001 Palm, Inc. or its subsidiaries.
	All rights reserved.

	This file is part of the Palm OS Emulator.

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.
\* ===================================================================== */

#ifndef EmTypes_h
#define EmTypes_h

// =====================================
// ========== SIMPLE TYPEDEFS ==========
// =====================================

// Standard integral types

	typedef	signed char			int8;
	typedef	unsigned char		uint8;

	typedef signed short		int16;
	typedef unsigned short		uint16;

	typedef signed long			int32;
	typedef unsigned long		uint32;

#ifdef _MSC_VER
	typedef signed __int64		int64;
	typedef unsigned __int64	uint64;
#else
	typedef signed long long	int64;
	typedef unsigned long long	uint64;
#endif

typedef uint32				emuptr;

typedef int 				Bool;

typedef int32				ErrCode;
typedef int32				StrCode;

typedef uint8				ScaleType;
typedef int32				RAMSizeType;
typedef int32				EmCoord;

#define EmMemNULL	((emuptr) 0)
#define EmMemEOM	((emuptr) 0xFFFFFFFF)


// Enumerated types

enum CloseActionType
{
	kSaveAlways,
	kSaveAsk,
	kSaveNever
};


enum LoggingType
{
	kNormalLogging	= 0x01,
	kGremlinLogging	= 0x02,
	kAllLogging		= kNormalLogging | kGremlinLogging
};


enum EmResetType
{
	kResetSys,			// Triggered by calling SysReset
	kResetSoft,			// Same as sticking pin in Reset hole
	kResetHard,			// Same as Soft Reset with Power key down
	kResetDebug,		// Same as Soft Reset with Page Down key down

	kResetTypeMask		= 0x07,
	kResetExtMask		= 0x08,
	kResetNoExt			= 0x08,

	kResetSysNoExt		= kResetNoExt | kResetSys,	// Same as kResetSys with Page Up key down.
	kResetSoftNoExt		= kResetNoExt | kResetSoft,	// Same as kResetSoft with Page Up key down.
	kResetHardNoExt		= kResetNoExt | kResetHard,	// Same as kResetHard with Page Up key down.
	kResetDebugNoExt	= kResetNoExt | kResetDebug	// Same as kResetDebug with Page Up key down.
};


enum EmErrorHandlingOption
{
	kShow,
	kContinue,
	kQuit,
	kSwitch
};


#endif	// EmTypes_h
