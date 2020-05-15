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

#include "EmLowMem.h"

#include "EmCommon.h"
#include "EmMemory.h"        // CEnableFullAccess
#include "EmPalmFunction.h"  // SysTrapIndex

#if 0                          // CSTODO
    #include "EmPatchState.h"  // EmPatchState::OSMajorVersion
#endif

// clang-format off
#include "PalmPack.h"
#define NON_PORTABLE
#include "SysEvtPrv.h"  // SysEvtMgrGlobalsType
#undef NON_PORTABLE
#include "PalmPackPop.h"
// clang-format ons

EmAliasLowMemHdrType<PAS> EmLowMem::fgLowMem(EmMemNULL);

#pragma mark -

// ===========================================================================
//		� EmLowMem
// ===========================================================================

static uint16 gSysDispatchTableSize;

/***********************************************************************
 *
 * FUNCTION:	EmLowMem::Initialize
 *
 * DESCRIPTION: Standard initialization function.  Responsible for
 *				initializing this sub-system when a new session is
 *				created.  Will be followed by at least one call to
 *				Reset or Load.
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

void EmLowMem::Initialize(void) { gSysDispatchTableSize = 0; }

/***********************************************************************
 *
 * FUNCTION:	EmLowMem::Dispose
 *
 * DESCRIPTION: Standard dispose function.	Completely release any
 *				resources acquired or allocated in Initialize and/or
 *				Load.
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

void EmLowMem::Dispose(void) {}

/***********************************************************************
 *
 * FUNCTION:	EmLowMem::GetEvtMgrIdle
 *
 * DESCRIPTION: .
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

uint8 EmLowMem::GetEvtMgrIdle(void) {
    CEnableFullAccess munge;  // Remove blocks on memory access.

    // First, get sysEvtMgrGlobalsP, which is a pointer in low-memory.

    emuptr sysEvtMgrGlobalsP = EmLowMem_GetGlobal(sysEvtMgrGlobalsP);

    /*
    **	Next look at the "idle" boolean in the SysEvtMgrGlobalsPtr
    **	contained in sysEvtMgrGlobalsP:
    **
    **	struct SysEvtMgrGlobalsType
    **	{
    **		...
    **		Boolean 		idle;
    **		...
    **	};
    */

    size_t idleOffset = offsetof(SysEvtMgrGlobalsType, idle);

    // Under Palm OS 1.0, there was an extra 4-byte value at the
    // location currently holding "gremlinsFlag", which is before
    // the "idle" field.  Under that version of the OS, we therefore
    // have to add 4 to get the right offset.

#if 0 // CSTODO
    if (EmPatchState::OSMajorVersion() == 1) {
        idleOffset += sizeof(Int32);
    }
#endif

    uint8 idle = EmMemGet8(sysEvtMgrGlobalsP + idleOffset);

    return idle;
}

/***********************************************************************
 *
 * FUNCTION:	EmLowMem::TrapExists
 *
 * DESCRIPTION: .
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

Bool EmLowMem::TrapExists(uint16 iATrap) {
#if 0
	// No longer cache the dispatch table size.  Handspring changes the
	// size of the dispatch table after the first system call, resulting
	// in Poser caching the old table size, and thus complaining that
	// some functions are not implemented when in fact they are.

	// If we haven't already, get the number of traps in this system.

	if (gSysDispatchTableSize == 0)
	{
		CEnableFullAccess	munge;	// Remove blocks on memory access.

		gSysDispatchTableSize = EmLowMem_GetGlobal (sysDispatchTableSize);
	}

	return (::SysTrapIndex (iATrap) == ::SysTrapIndex (sysTrapHostControl)) ||
			::SysTrapIndex (iATrap) < gSysDispatchTableSize;
#else
    CEnableFullAccess munge;  // Remove blocks on memory access.
    uint16 sysDispatchTableSize = EmLowMem_GetGlobal(sysDispatchTableSize);
    return (::SysTrapIndex(iATrap) == ::SysTrapIndex(sysTrapHostControl)) ||
           ::SysTrapIndex(iATrap) < sysDispatchTableSize;
#endif
}

/***********************************************************************
 *
 * FUNCTION:	EmLowMem::GetTrapAddress
 *
 * DESCRIPTION: .
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

emuptr EmLowMem::GetTrapAddress(uint16 iATrap) {
    emuptr address = EmMemNULL;

    CEnableFullAccess munge;  // Remove blocks on memory access.

    // If the trap number is not in range, just return NULL to signal
    // that normal process of the TRAP $F should occur.

    //	uint16	sysDispatchTableSize = EmLowMem_GetGlobal (sysDispatchTableSize);
    uint16 sysDispatchTableSize = EmMemGet16(256 + 62);

    iATrap = ::SysTrapIndex(iATrap);

    if ((iATrap == ::SysTrapIndex(sysTrapHostControl)) || (iATrap < sysDispatchTableSize)) {
        // Get sysDispatchTableP, which is a pointer in low-memory.

        //		uint32 sysDispatchTableP	= EmLowMem_GetGlobal (sysDispatchTableP);
        uint32 sysDispatchTableP = EmMemGet32(256 + 34);

        // If it's NULL, return NULL.

        if (sysDispatchTableP != EmMemNULL) {
            // Next, get the right guy from its contents.

            address = EmMemGet32(sysDispatchTableP + iATrap * 4);
        }
    }

    return address;
}
