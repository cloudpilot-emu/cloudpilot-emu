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

#include "EmPalmOS.h"

#include "Byteswapping.h"
#include "EmBankDRAM.h"    // EmBankDRAM::SetLong
#include "EmBankMapped.h"  // EmBankMapped::SetLong
#include "EmBankROM.h"     // EmBankROM::SetLong
#include "EmBankSRAM.h"    // EmBankSRAM::SetLong
#include "EmCPU68K.h"      // gCPU68K, gStackHigh, etc.
#include "EmCommon.h"
#include "EmLowMem.h"
#include "EmMemory.h"       // CEnableFullAccess
#include "EmPalmStructs.h"  // EmAliasCardHeaderType
#include "EmPatchMgr.h"     // EmPatchMgr
#include "EmSession.h"      // gSession->Reset
#include "Logging.h"
#include "Miscellaneous.h"  // GetSystemCallContext
#include "ROMStubs.h"
#include "UAE.h"  // CHECK_STACK_POINTER_DECREMENT

#if 0                             // CSTODO
    #include "EmErrCodes.h"       // kError_UnimplementedTrap, kError_InvalidLibraryRefNum
    #include "EmSystemState.h"    // EmSystemState
    #include "ErrorHandling.h"    // Errors::ReportInvalidPC
    #include "Platform_NetLib.h"  // Platform_NetLib::Initialize();
    #include "ROMStubs.h"         // IntlSetStrictChecks

#endif

#define LOG_FUNCTION_CALLS 0

static emuptr gBigROMEntry;

/***********************************************************************
 *
 * FUNCTION:	EmPalmOS::Initialize
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

void EmPalmOS::Initialize(void) {
    EmAssert(gCPU68K);

    gCPU68K->InstallHookException(kException_SysCall, HandleTrap15);
    gCPU68K->InstallHookJSR_Ind(HandleJSR_Ind);

    gBigROMEntry = EmMemNULL;

    EmPatchMgr::Initialize();

#if 0  // CSTODO
    Platform_NetLib::Initialize();
#endif
}

/***********************************************************************
 *
 * FUNCTION:	EmPalmOS::Reset
 *
 * DESCRIPTION:	Standard reset function.  Sets the sub-system to a
 *				default state.  This occurs not only on a Reset (as
 *				from the menu item), but also when the sub-system
 *				is first initialized (Reset is called after Initialize)
 *				as well as when the system is re-loaded from an
 *				insufficient session file.
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

void EmPalmOS::Reset(void) {
    EmPatchMgr::Reset();

#if 0  // CSTODO
    Platform_NetLib::Reset();
    EmLowMem::Reset();
#endif
}

/***********************************************************************
 *
 * FUNCTION:	EmPalmOS::Save
 *
 * DESCRIPTION:	Standard save function.  Saves any sub-system state to
 *				the given session file.
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

void EmPalmOS::Save(SessionFile& f) {
#if 0  // CSTODO
    Hordes::Save(f);
    EmEventPlayback::Save(f);
    EmPatchMgr::Save(f);
    Platform_NetLib::Save(f);
    EmPalmHeap::Save(f);
    EmLowMem::Save(f);

    const long kCurrentVersion = 2;

    Chunk chunk;
    EmStreamChunk s(chunk);

    s << kCurrentVersion;

    s << gStackList;
    s << gBootStack;
    s << gKernelStack;
    s << gInterruptStack;

    s << gStackHigh;
    s << gStackLowWaterMark;
    s << gStackLowWarn;
    s << gStackLow;

    s << gKernelStackOverflowed;

    f.WriteStackInfo(chunk);
#endif
}

/***********************************************************************
 *
 * FUNCTION:	EmPalmOS::Load
 *
 * DESCRIPTION:	Standard load function.  Loads any sub-system state
 *				from the given session file.
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

void EmPalmOS::Load(SessionFile& f) {
#if 0  // CSTODO
    Hordes::Load(f);
    EmEventPlayback::Load(f);
    EmPatchMgr::Load(f);
    Platform_NetLib::Load(f);
    EmPalmHeap::Load(f);
    EmLowMem::Load(f);

    Chunk chunk;
    if (f.ReadStackInfo(chunk)) {
        long version;
        EmStreamChunk s(chunk);

        s >> version;

        if (version >= 1) {
            s >> gStackList;
            s >> gBootStack;
            s >> gKernelStack;
            s >> gInterruptStack;

            s >> gStackHigh;
            s >> gStackLowWaterMark;
            s >> gStackLowWarn;
            s >> gStackLow;
        }

        if (version >= 2) {
            s >> gKernelStackOverflowed;
        }
    } else {
        f.SetCanReload(false);  // Need to reboot
    }
#endif
}

/***********************************************************************
 *
 * FUNCTION:	EmPalmOS::Dispose
 *
 * DESCRIPTION:	Standard dispose function.  Completely release any
 *				resources acquired or allocated in Initialize and/or
 *				Load.
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

void EmPalmOS::Dispose(void) {
    EmPatchMgr::Dispose();

#if 0  // CSTODO
    EmLowMem::Dispose();
    EmPalmHeap::Dispose();
    Platform_NetLib::Dispose();
    EmEventPlayback::Dispose();
    Hordes::Dispose();
#endif
}

/***********************************************************************
 *
 * FUNCTION:	EmPalmOS::HandleTrap15
 *
 * DESCRIPTION: Handle a trap. Traps are of the format TRAP $F / $Axxx.
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

Bool EmPalmOS::HandleTrap15(ExceptionNumber) { return EmPalmOS::HandleSystemCall(true); }

/***********************************************************************
 *
 * FUNCTION:	EmPalmOS::HandleJSR_Ind
 *
 * DESCRIPTION: Check for SYS_TRAP_FAST calls.
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

Bool EmPalmOS::HandleJSR_Ind(emuptr oldpc, emuptr dest) {
    Bool handledIt = false;  // Default to calling ROM.

    //	inline asm SysTrapFast(Int trapNum)
    //	{
    //			MOVE.L	struct(LowMemType.fixed.globals.sysDispatchTableP), A1
    //			MOVE.L	((trapNum-sysTrapBase)*4)(A1), A1
    //			JSR 	(A1)	// call it
    //	}
    //
    //	#define SYS_TRAP_FAST(trapNum)
    //		FIVEWORD_INLINE(
    //			0x2278, 0x0122,
    //			0x2269, (trapNum-sysTrapBase)*4,
    //			0x4e91)

    uint8* realMem = EmMemGetRealAddress(oldpc);
    if (EmMemDoGet16(realMem) == 0x4e91 && EmMemDoGet16(realMem - 4) == 0x2269 &&
        EmMemDoGet16(realMem - 8) == 0x2278) {
        handledIt = EmPalmOS::HandleSystemCall(false);
    } else {
        if (gBigROMEntry == EmMemNULL) {
            emuptr base = EmBankROM::GetMemoryStart();

            // Check every romDelta (4K) for up to maxBigROMOffset
            //(256K) till we find the "Big" ROM

            const UInt32 romDelta = 4 * 1024L;           // BigROM starts on a 4K boundary
            const UInt32 maxBigROMOffset = 256 * 1024L;  // Give up looking past here
            UInt16 loops = maxBigROMOffset / romDelta;   // How many loops to do
            emuptr bP = base + romDelta;

            while (loops--) {
                // Ignore older card headers that might have been
                // programmed in at a lower address (hdrVersion < 3)

                EmAliasCardHeaderType<PAS> cardHdr(bP);

                UInt32 signature = cardHdr.signature;
                UInt16 hdrVersion = cardHdr.hdrVersion;

                if (signature == sysCardSignature && hdrVersion >= 3) {
                    gBigROMEntry = cardHdr.resetVector;
                    break;
                }

                bP += romDelta;
            }

            // See if we found it (may have hdrVersion < 3)

            if (gBigROMEntry == EmMemNULL) {
                EmAliasCardHeaderType<PAS> smallCardHdr(base);
                UInt32 bigROMOffset;

                if (smallCardHdr.hdrVersion == 2) {
                    bigROMOffset = smallCardHdr.bigROMOffset & 0x000FFFFF;
                } else {
                    bigROMOffset = 0x3000;
                }

                EmAliasCardHeaderType<PAS> bigCardHdr(base + bigROMOffset);
                gBigROMEntry = bigCardHdr.resetVector;
            }
        }

        if (dest == gBigROMEntry) {
            EmAssert(gSession);
            gSession->ScheduleReset(EmSession::ResetType::sys);
        }
    }

    return handledIt;
}

/***********************************************************************
 *
 * FUNCTION:	EmPalmOS::HandleSystemCall
 *
 * DESCRIPTION: .
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

Bool EmPalmOS::HandleSystemCall(Bool fromTrap) {
    // ======================================================================
    //	First things first: if we need to break execution on the next call
    //	to a system function, make sure that happens.
    // ======================================================================

    EmAssert(gSession);
    EmAssert(gCPU68K);

    // If the system call is being made by a TRAP $F, the PC has already
    // been bumped past the opcode.  If being made with a JSR via the
    // SYS_TRAP_FAST macro, the PC has not been adjusted.  Determine a
    // "pcAdjust" value that allows us to get back to the start of the
    // instruction that got us here.

    int pcAdjust = fromTrap ? 2 : 0;

    if (!gSession->IsNested() && gSession->WaitingForSyscall()) {
        CEnableFullAccess munge;

        // Check the memory manager semaphore.  If we're stopping on a
        // system call, it's most likely that we want to make some nested
        // Palm OS calls.  Some of those calls may try to acquire the
        // memory manager semaphore.  Therefore, make sure it's available.

        UInt32 memSemaphoreIDP = EmLowMem_GetGlobal(memSemaphoreID);
        EmAliascj_xsmb<PAS> memSemaphoreID(memSemaphoreIDP);

        if (memSemaphoreID.xsmuse == 0) {
            SysKernelInfoType taskInfo;
            taskInfo.selector = sysKernelInfoSelCurTaskInfo;

            Err err = ::SysKernelInfo(&taskInfo);

            if (err == errNone &&
                (taskInfo.param.task.tag == 'psys' || taskInfo.param.task.tag == 0x414d5800)) {
                gCPU->SetPC(gCPU->GetPC() - pcAdjust);
                gSession->NotifySyscallDispatched();

                return true;
            }
        }
    }

    // ======================================================================
    //	Determine what ROM function is about to be called, and determine
    //	the method by which it is being called.
    // ======================================================================

    SystemCallContext context;
    Bool gotFunction = GetSystemCallContext(gCPU->GetPC() - pcAdjust, context);

    // ======================================================================
    //	Validate the address for the ROM function we're about to call.
    // ======================================================================

    if (!gotFunction) {
        // We should never get here.  context.fError should always equal
        // one of those two error codes, and those two functions we call
        // should never return (they should throw exceptions).

        EmAssert(false);
    }

    // ======================================================================
    // If this trap is patched, let the patch handler handle the patch.
    // ======================================================================

    CallROMType result = EmPatchMgr::HandleSystemCall(context);

    // ======================================================================
    //	If we completely handled the function in head and tail patches, tell
    //	the profiler that we exited the function and get out of here.
    // ======================================================================

    if (result == kSkipROM) {
        gCPU->SetPC(context.fNextPC);

        // Return true to say that everything has been handled.
        return true;
    }

    return false;
}
