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

#include "EmBankDRAM.h"    // EmBankDRAM::SetLong
#include "EmBankMapped.h"  // EmBankMapped::SetLong
#include "EmBankROM.h"     // EmBankROM::SetLong
#include "EmBankSRAM.h"    // EmBankSRAM::SetLong
#include "EmCPU68K.h"      // gCPU68K, gStackHigh, etc.
#include "EmCommon.h"
#include "EmMemory.h"       // CEnableFullAccess
#include "EmPalmStructs.h"  // EmAliasCardHeaderType
#include "EmSession.h"      // gSession->Reset
#include "Logging.h"
#include "Miscellaneous.h"  // GetSystemCallContext
#include "UAE.h"            // CHECK_STACK_POINTER_DECREMENT

#if 0                             // CSDUBIOUS
    #include "EmEventPlayback.h"  // EmEventPlayback::Initialize ();
    #include "EmPalmFunction.h"   // ProscribedFunction
    #include "EmPalmFunction.h"   // EmPalmFunctionInit ();
    #include "EmPalmHeap.h"       // EmPalmHeap, GetHeapByPtr
    #include "EmPalmHeap.h"       // EmPalmHeap::Initialize ();
    #include "MetaMemory.h"       // MetaMemory::InRAMOSComponent

#endif

#if 0                             // CSTODO
    #include "EmErrCodes.h"       // kError_UnimplementedTrap, kError_InvalidLibraryRefNum
    #include "EmPatchMgr.h"       // EmPatchMgr
    #include "EmPatchMgr.h"       // EmPatchMgr::Initialize ();
    #include "EmPatchState.h"     // EmPatchState
    #include "ErrorHandling.h"    // Errors::ReportInvalidPC
    #include "Platform_NetLib.h"  // Platform_NetLib::Initialize();
    #include "ROMStubs.h"         // IntlSetStrictChecks

#endif

#define LOG_FUNCTION_CALLS 0

#if 0  // CSDUBIOUS
static StackList gStackList;
static StackRange gBootStack;
static StackRange gKernelStack;
static StackRange gInterruptStack;

static const uint32 CJ_TAGAMX = 0x414D5800;
static const uint32 CJ_TAGFENCE = 0x55555555;

// Subtract 34 bytes from the size of the stack.  That's because
// if an interrupt occurs, 34 bytes are pushed onto the stack before
// the OS switches over to the interrupt stack.
//
//		Exception		 6 bytes		// Old SR and old PC
//		LINK			 4 bytes		// Old A6
//		MOVEM.L			20 bytes		// Old D0, D1, D2, A0, A1
//		MOVE A0, -(SP)	 4 bytes		// Old A0 (again)
//		---------------------------------------------------------
//		Total			34 bytes

static const int kInterruptOverhead = 34;
CSDUBIOUS static emuptr		gStackLowWaterMark = 0;
#endif

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

#if 0  // CSDUBIOUS
	gCPU68K->InstallHookNewPC(HandleNewPC);
    gCPU68K->InstallHookNewSP(HandleNewSP);

    // Add a notification for IntlStrictChecks
    gPrefs->AddNotification(&EmPalmOS::PrefsChanged, kPrefKeyReportStrictIntlChecks);
    gPrefs->AddNotification(&EmPalmOS::PrefsChanged, kPrefKeyReportOverlayErrors);

	Hordes::Initialize();
    EmEventPlayback::Initialize();
	EmPalmHeap::Initialize();
    EmPalmFunctionInit();
#endif

#if 0  // CSTODO
    EmPatchMgr::Initialize();
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
#if 0  // CSDUBIOUS
    gStackList.clear();

    gBootStack = StackRange();
    gKernelStack = StackRange();
    gInterruptStack = StackRange();

    gKernelStackOverflowed = 0;
    Hordes::Reset();
    EmEventPlayback::Reset();
    EmPalmHeap::Reset();
#endif

#if 0  // CSTODO
    EmPatchMgr::Reset();
    Platform_NetLib::Reset();
    EmLowMem::Reset();
#endif

    // If the appropriate modifier key is down, install a temporary breakpoint
    // at the start of the Big ROM.

#if 0  // CSDUBIOUS
    if (Platform::StopOnResetKeyDown()) {
        emuptr romStart = EmBankROM::GetMemoryStart();
        emuptr headerVersion = EmMemGet32(romStart + offsetof(CardHeaderType, hdrVersion));
        long bigROMOffset = 0x03000;

        if (headerVersion > 1) {
            bigROMOffset = EmMemGet32(romStart + offsetof(CardHeaderType, bigROMOffset));
            bigROMOffset &= 0x000FFFFF;  // Allows for 1 Meg offset.
        }

        emuptr resetVector =
            EmMemGet32(romStart + bigROMOffset + offsetof(CardHeaderType, resetVector));

        Debug::SetBreakpoint(dbgTempBPIndex, resetVector, NULL);
    }
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
#if 0  // CSTODO
    EmLowMem::Dispose();
    EmPalmHeap::Dispose();
    Platform_NetLib::Dispose();
    EmPatchMgr::Dispose();
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
            gSession->ScheduleReset(kResetSys);
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

#if 0  // CSTODO

    if (!gSession->IsNested() && gSession->GetBreakOnSysCall()) {
        CEnableFullAccess munge;

        // Check the memory manager semaphore.  If we're stopping on a
        // system call, it's most likely that we want to make some nested
        // Palm OS calls.  Some of those calls may try to acquire the
        // memory manager semaphore.  Therefore, make sure it's available.

        UInt32 memSemaphoreIDP = EmLowMem_GetGlobal(memSemaphoreID);
        EmAliascj_xsmb<PAS> memSemaphoreID(memSemaphoreIDP);

        if (memSemaphoreID.xsmuse == 0) {
            // Check the current task.  We don't want to break on a system
            // call if we're in the background.  If we're breaking on a
            // system call, it's likely because some other part of Poser
            // wants to make OS calls.  Occassionally, those OS calls result
            // in trying to switch to the UI task (see, for example,
            // PrvEditCardOptionsOK).  If we're already in the UI task, then
            // that's OK, but if we're in a background task, an attempted
            // switch to the UI task will fail, due to Poser turning off
            // interrupts when it makes OS calls.

            SysKernelInfoType taskInfo;
            taskInfo.selector = sysKernelInfoSelCurTaskInfo;

            Err err = ::SysKernelInfo(&taskInfo);
            if (err == errNone) {
                if (taskInfo.param.task.tag == 'psys') {
                    gCPU->SetPC(gCPU->GetPC() - pcAdjust);
                    gSession->ScheduleSuspendSysCall();

                    // Return true to say that everything has been handled.

                    return true;
                }

                // If we took a stab at this and didn't find that we were
                // in the UI task, then call EvtWakeup.  If our context is
                // one where an application has called EvtGetEvent with a
                // "sleep forever" timeout, then the current task will be
                // 'AMX ', and will stay that way.  Calling EvtWakeup will
                // force an eventual switch to the UI task.
                //
                // Note that we wake-up the UI thread only if we're in the
                // root AMX thread.  This prevents us from trying to call
                // it when the Palm context is one where a background task
                // is the current task.  It's possible for that task to be
                // sleeping or something (the example we ran into was with
                // a background task calling SysDelay).  EvtWakeup will try
                // to switch to the UI task, but will not return until the
                // UI task switches back to the background task.  That will
                // never happen because interrupts are turned off during
                // all Poser calls into the OS (such as the call to
                // EvtWakeup).  Thus, the only safe places to call
                // EvtWakeup are in the UI or AMX tasks.

                else if (taskInfo.param.task.tag == 0x414d5800) {
                    ::EvtWakeup();
                }
            }
        }
    }

#endif

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
#if 0  // CSDUBIOUS
        if (context.fError == kError_UnimplementedTrap) {
            Errors::ReportErrUnimplementedTrap(context);
        } else if (context.fError == kError_InvalidLibraryRefNum) {
            Errors::ReportErrInvalidRefNum(context);
        }
#endif

        // We should never get here.  context.fError should always equal
        // one of those two error codes, and those two functions we call
        // should never return (they should throw exceptions).

        EmAssert(false);
    }

#if 0  // CSTODO

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
#endif

    return false;
}

#if 0  // CSDUBIOUS

/***********************************************************************
 *
 * FUNCTION:	EmPalmOS::CheckStackPointerAssignment
 *
 * DESCRIPTION: !!! This operation slows down Gremlins by about 10%; it
 *				should be sped up or made an option.
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

void EmPalmOS::CheckStackPointerAssignment(void) {
    // If it was a "drastic" change, assume that we're *setting* the
    // stack pointer to a new stack.  Scarf up information about that
    // block of memory and treat that block as a stack.

    // See if we already know about this stack.

    emuptr curA7 = gCPU->GetSP();
    StackList::iterator iter = gStackList.begin();

    while (iter != gStackList.end()) {
        if ((curA7 >= (*iter).fBottom) && (curA7 <= (*iter).fTop)) {
            // If so, switch to it.
            SetCurrentStack(*iter);
            return;
        }

        ++iter;
    }

    // If not, get some information about it and save it off.

    const EmPalmHeap* heap = EmPalmHeap::GetHeapByPtr(curA7);
    if (heap) {
        const EmPalmChunk* chunk = heap->GetChunkBodyContaining(curA7);
        if (chunk) {
            // The second time we switch stacks, we're switching from the
            // boot stack to the kernel stack.  In that case, forget any
            // notion of the boot stack.

            if (gBootStack.fBottom != EmMemNULL) {
                ForgetStack(gBootStack.fBottom);
            }

            // See if this looks like the AMX kernel stack.
            // We detect this by seeing (a) if the stack pointer
            // is being set to a value not too close to the end
            // of the chunk it's in (indicating that the stack is
            // embedded in some other data, as is the AMX stack)
            // and (b) if the values the stack pointer points to
            // are the appropriate tags.

            CEnableFullAccess munge;
            if (curA7 <= (chunk->BodyEnd() - 8) && EmMemGet32(curA7) == CJ_TAGAMX &&
                EmMemGet32(curA7 + 4) == CJ_TAGFENCE) {
                RememberKernelStack();

                // Remember this stack as the "current" stack.

                SetCurrentStack(gKernelStack);
            } else {
                RememberStackChunk(*chunk);

                // Remember this stack as the "current" stack.

                StackRange range(chunk->BodyStart(), chunk->BodyStart() + chunk->BodySize());
                SetCurrentStack(range);
            }
        } else {
            // !!!
        }
    } else {
        if (gStackList.size() == 0) {
            // We're switching from the reset stack to the boot stack.

            RememberBootStack();

            // Remember this stack as the "current" stack.

            SetCurrentStack(gBootStack);
        }
    }
}

/***********************************************************************
 *
 * FUNCTION:	EmPalmOS::CheckStackPointerDecrement
 *
 * DESCRIPTION: !!! This operation slows down Gremlins by about 10%; it
 *				should be sped up or made an option.
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

void EmPalmOS::CheckStackPointerDecrement(void) {
    // If it was an "incremental" change, make sure that A7 is not
    // now outside the stack it used to be in (as indicated by oldA7).

    // Test for "close to the end" (in which case we display a warning), and
    // "past the end" (in which case we print a fatal error).

    if (gKernelStackOverflowed) return;

    emuptr curA7 = gCPU->GetSP();
    Bool warning = curA7 < gStackLowWarn;
    Bool fatal = curA7 < gStackLow;

    if (gStackLowWaterMark > curA7) gStackLowWaterMark = curA7;

    if (warning) {
        // Special hack for the kernel stack overflowing into the
        // interrupt stack.  If there was a stack overflow and the
        // current stack is the kernel stack, then ignore the error.
        // Also, try limiting this hack to older Palm OS's by looking
        // at the stack size.  Newer ROMs will have larger stacks and
        // should not be allowed to have this bug.

        if (fatal && gKernelStack.fTop == gStackHigh &&
            gKernelStack.fTop - gKernelStack.fBottom <= 0x2F4) {
            gKernelStackOverflowed = 1;
            return;
        }

        if (fatal) {
            Errors::ReportErrStackOverflow();
        } else {
            EmAssert(gSession);
            gSession->ScheduleDeferredError(new EmDeferredErrStackFull);
        }
    }
}

/***********************************************************************
 *
 * FUNCTION:	EmPalmOS::CheckKernelStack
 *
 * DESCRIPTION: .
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

void EmPalmOS::CheckKernelStack(void) {
    EmAssert(gKernelStackOverflowed);

    emuptr curA7 = gCPU->GetSP();

    if (curA7 <= gStackHigh && curA7 >= gStackLow) {
        gKernelStackOverflowed = 0;
    }
}

/***********************************************************************
 *
 * FUNCTION:	EmPalmOS::RememberStackChunk
 *
 * DESCRIPTION: .
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

void EmPalmOS::RememberStackChunk(const EmPalmChunk& chunk) {
    // Create a range object for the chunk.

    StackRange range(chunk.BodyStart(), chunk.BodyEnd());

    // Remember it for later.

    RememberStackRange(range);
}

/***********************************************************************
 *
 * FUNCTION:	EmPalmOS::RememberBootStack
 *
 * DESCRIPTION: .
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

void EmPalmOS::RememberBootStack(void) {
    /*
            There's a constant in SystemPrv.h that indicates that the boot
            stack should be 4K long.  However, that 4K is relative to the
            start of the dynamic heap, which is used while the boot stack
            is in effect.  As the system boots up, blocks are allocated and
            de-allocated in the stack range.  We have a check in ForgetStack
            that checks to see if a MemFooFree operation is taking place
            within a stack -- normally a dubious operation -- and it's
            noticing this situation.

            For now, it looks like we only use 0x438 bytes* of the boot
            stack, so I'll allow 0x500.  This should prevent the overlap of
            the boot stack with any allocated chunks.

            *	This was against one particular ROM, but I don't remember
                    which.  Checking a Palm IIIc 4.0 Debug ROM just now, I see
                    it using 0x308 bytes.
    */

    //	const int kBootStackSize = 0x1000;	// Per SystemPrv.h
    const int kBootStackSize = 0x0500;  // Per experimentation

    gBootStack = StackRange(gCPU->GetSP() - kBootStackSize, gCPU->GetSP());

    RememberStackRange(gBootStack);
}

/***********************************************************************
 *
 * FUNCTION:	EmPalmOS::RememberKernelStack
 *
 * DESCRIPTION: .
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

void EmPalmOS::RememberKernelStack(void) {
    // Scan down for the bottom of stack marker.

    CEnableFullAccess munge;

    emuptr ksp = gCPU->GetSP();
    while (EmMemGet32(ksp) != CJ_TAGFENCE) ksp -= 2;

    emuptr isp = ksp - 4;
    while (EmMemGet32(isp) != CJ_TAGFENCE) isp -= 2;

    // Add the two stacks.  Add the interrupt stack first, and the
    // kernel stack second.  That way, the last added stack -- the
    // kernel stack -- is remembered as the "current" stack.

    gInterruptStack = StackRange(isp + 4, ksp);
    RememberStackRange(gInterruptStack);

    gKernelStack = StackRange(ksp + 4, gCPU->GetSP());
    RememberStackRange(gKernelStack);
}

/***********************************************************************
 *
 * FUNCTION:	EmPalmOS::RememberStackRange
 *
 * DESCRIPTION: .
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

void EmPalmOS::RememberStackRange(const StackRange& range) {
    #if _DEBUG
    // Make sure this range doesn't overlap any other stack in our list.

    StackList::iterator iter = gStackList.begin();

    while (iter != gStackList.end()) {
        if (range.fBottom >= iter->fBottom && range.fBottom < iter->fTop) EmAssert(false);

        if (range.fTop > iter->fBottom && range.fTop <= iter->fTop) EmAssert(false);

        ++iter;
    }
    #endif

    // Add the range to the list.

    gStackList.push_back(range);
}

/***********************************************************************
 *
 * FUNCTION:	EmPalmOS::SetCurrentStack
 *
 * DESCRIPTION: .
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

void EmPalmOS::SetCurrentStack(const StackRange& range) {
    // Record the low-water mark of the previous stack.

    StackList::iterator iter = gStackList.begin();

    while (iter != gStackList.end()) {
        if (gStackHigh == iter->fTop) {
            iter->fLowWaterMark = gStackLowWaterMark;

            if (gStackHigh == gBootStack.fTop) gBootStack.fLowWaterMark = gStackLowWaterMark;

            if (gStackHigh == gKernelStack.fTop) gKernelStack.fLowWaterMark = gStackLowWaterMark;

            if (gStackHigh == gInterruptStack.fTop)
                gInterruptStack.fLowWaterMark = gStackLowWaterMark;

            break;
        }

        ++iter;
    }

    // Determine the amount to test against when determining if we are
    // close to overflowing the stack.  For applications, let's set this
    // to 50 bytes.  For the kernel stack, set it to zero; some Palm OS's
    // get damn close to the end of the kernel stack (the Palm VII ROM gets
    // within 8 bytes!).

    const int kAppStackSlush = 50;
    const int kKernelStackSlush = 0;

    int stackSlush = kAppStackSlush;

    if (range == gKernelStack) stackSlush = kKernelStackSlush;

    gStackHigh = range.fTop;
    gStackLowWaterMark = range.fLowWaterMark;
    gStackLowWarn = range.fBottom + kInterruptOverhead + stackSlush;
    gStackLow = range.fBottom + kInterruptOverhead;
}

/***********************************************************************
 *
 * FUNCTION:	EmPalmOS::ForgetStack
 *
 * DESCRIPTION: .
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

void EmPalmOS::ForgetStack(emuptr stackBottom) {
    StackList::iterator iter = gStackList.begin();

    while (iter != gStackList.end()) {
        // If the pointer is in the *middle* of a stack, that's not good.

        if (stackBottom > iter->fBottom && stackBottom < iter->fTop) EmAssert(false);

        // If the pointer is to the beginning of the stack, we've found
        // the one we want to remove.

        if (stackBottom == iter->fBottom) {
            gStackList.erase(iter);

            if (stackBottom == gBootStack.fBottom) {
    #define TRACK_BOOT_ALLOCATION 0
    #if TRACK_BOOT_ALLOCATION
                LogAppendMsg(
                    "===== Forgetting Boot Stack, top = 0x%08X, low water mark: 0x%08X =====",
                    gBootStack.fTop, gBootStack.fLowWaterMark);
    #endif

                gBootStack = StackRange();
            }

            return;
        }

        ++iter;
    }
}

/***********************************************************************
 *
 * FUNCTION:	EmPalmOS::ForgetStacksIn
 *
 * DESCRIPTION: .
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

void EmPalmOS::ForgetStacksIn(emuptr start, uint32 range) {
    StackList::iterator iter = gStackList.begin();

    while (iter != gStackList.end()) {
        if ((iter->fBottom >= start) && (iter->fTop <= (start + range))) {
            gStackList.erase(iter);

            if (start == gBootStack.fBottom) {
    #define TRACK_BOOT_ALLOCATION 0
    #if TRACK_BOOT_ALLOCATION
                LogAppendMsg(
                    "===== Forgetting Boot Stack, top = 0x%08X, low water mark: 0x%08X =====",
                    gBootStack.fTop, gBootStack.fLowWaterMark);
    #endif

                gBootStack = StackRange();
            }

            // We munged the collection; start over.

            iter = gStackList.begin();
            continue;
        }

        ++iter;
    }
}

/***********************************************************************
 *
 * FUNCTION:	EmPalmOS::GetBootStack
 *
 * DESCRIPTION: .
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

StackRange EmPalmOS::GetBootStack(void) { return gBootStack; }

/***********************************************************************
 *
 * FUNCTION:	EmPalmOS::IsInStack
 *
 * DESCRIPTION: .
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

Bool EmPalmOS::IsInStack(emuptr addr) { return addr >= gStackLow && addr < gStackHigh; }

/***********************************************************************
 *
 * FUNCTION:	EmPalmOS::IsInStackBlock
 *
 * DESCRIPTION: .
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

Bool EmPalmOS::IsInStackBlock(emuptr addr) { return addr >= gCPU->GetSP() && addr < gStackHigh; }

/***********************************************************************
 *
 * FUNCTION:    EmPalmOS::GenerateStackCrawl
 *
 * DESCRIPTION: Starting with the current PC and A6, generate a list
 *				of active functions.
 *
 * PARAMETERS:  frameList - reference to the collection to receive
 *					the results.
 *
 * RETURNED:    Nothing
 *
 ***********************************************************************/

void EmPalmOS::GenerateStackCrawl(EmStackFrameList& frameList) {
    // Make sure we have access to all of memory, in case A6 is bogus
    // or used for some purpose other than frame links.

    CEnableFullAccess munge;

    // Clear out the stack crawl list.

    frameList.clear();

    // Push the initial stack frame onto the stack.  This consists
    // of the current PC and A6 values.

    EmStackFrame frame;

    frame.fAddressInFunction = m68k_getpc();
    frame.fA6 = m68k_areg(regs, 6);

    frameList.push_back(frame);

    // If A6 is odd or not in the current stack, stop the stack crawl.

    if (::IsOdd(frame.fA6) || !EmPalmOS::IsInStack(frame.fA6)) return;

    while (1) {
        emuptr oldA6 = frame.fA6;

        // Get the previous A6 and function from the stack.

        frame.fAddressInFunction = EmMemGet32(oldA6 + 4);
        frame.fA6 = EmMemGet32(oldA6);

        // If A6 is odd or not in the current stack, stop the stack crawl.

        if (::IsOdd(frame.fA6) || !EmPalmOS::IsInStack(frame.fA6)) return;

        // If the new A6 is not greater than the old A6, stop
        // the stack crawl.

        if (frame.fA6 <= oldA6) return;

        // If the return address is not valid, let's take a chance
        // that what's on the stack is <A6> <SR> <return address>.
        // We'd see this sequence if an exception has occurred and
        // and exception frame is on the stack.

        if (!EmMemCheckAddress(frame.fAddressInFunction, 2)) {
            frame.fAddressInFunction = EmMemGet32(oldA6 + 6);

            // If the return address still doesn't look valid,
            // stop the stack crawl.

            if (!EmMemCheckAddress(frame.fAddressInFunction, 2)) {
                return;
            }
        }

        // Everything looks OK, push this information onto our stack.

        frameList.push_back(frame);
    }
}

// ---------------------------------------------------------------------------
//		� EmPalmOS::PrefsChanged
// ---------------------------------------------------------------------------
// Respond to a preference change.

static void PrvRespondToPrefsChange(PrefKeyType prefKey) {
    if (EmPatchState::UIInitialized()) {
        if (::PrefKeysEqual(prefKey, kPrefKeyReportStrictIntlChecks) &&
            EmPatchMgr::IntlMgrAvailable()) {
            Preference<Bool> pref(kPrefKeyReportStrictIntlChecks, false);
            ::IntlSetStrictChecks(*pref);
        }

        if (::PrefKeysEqual(prefKey, kPrefKeyReportOverlayErrors)) {
            Preference<Bool> pref(kPrefKeyReportOverlayErrors, false);
            (void)::FtrSet(omFtrCreator, omFtrShowErrorsFlag, *pref);
        }
    }
}

void EmPalmOS::PrefsChanged(PrefKeyType prefKey, void*) {
    if (gSession) {
    #if HAS_OMNI_THREAD
        if (gSession->InCPUThread()) {
            ::PrvRespondToPrefsChange(prefKey);
        } else
    #endif
        {
            EmSessionStopper stopper(gSession, kStopOnSysCall);

            ::PrvRespondToPrefsChange(prefKey);
        }
    }
}

    #pragma mark -


/***********************************************************************
 *
 * FUNCTION:	EmPalmOS::HandleNewPC
 *
 * DESCRIPTION: The PC is about to be changed via a JSR, BSR, RTS, or
 *				RTE.  Check that the new address appears to be valid.
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

void EmPalmOS::HandleNewPC(emuptr dest) {
    // If the address is odd, then it's bad.

    if ((dest & 1) != 0) Errors::ReportErrInvalidPC(dest, kOddAddress);

    //	if (!EmPatchState::UIInitialized ())
    //		return;

    // Ensure that the PC is in ROM or RAM.

    EmMemPutFunc longSetter = EmMemGetBank(dest).lput;

    if (longSetter == EmBankROM::SetLong || longSetter == EmBankFlash::SetLong) {
        if (!EmBankROM::ValidAddress(dest, 2)) {
            Errors::ReportErrInvalidPC(dest, kNotInCodeSegment);
        }
    } else if (longSetter == EmBankSRAM::SetLong) {
    } else if (longSetter == EmBankDRAM::SetLong) {
        if (dest < (256 + 693 * 4) /*MetaMemory::GetSysGlobalsEnd ()*/) {
            Errors::ReportErrInvalidPC(dest, kNotInCodeSegment);
        }
    } else if (longSetter == EmBankMapped::SetLong) {
        if (!EmBankMapped::ValidAddress(dest, 2)) {
            Errors::ReportErrInvalidPC(dest, kUnmappedAddress);
        }
    } else {
        Errors::ReportErrInvalidPC(dest, kUnmappedAddress);
    }
}

/***********************************************************************
 *
 * FUNCTION:	EmPalmOS::HandleNewSP
 *
 * DESCRIPTION: !!! This operation slows down Gremlins by about 10%; it
 *				should be sped up or made an option.
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

void EmPalmOS::HandleNewSP(EmStackChangeType type) {
    if (type == kStackPointerChanged) {
        CheckStackPointerAssignment();
    } else if (type == kStackPointerDecremented) {
        CheckStackPointerDecrement();
    } else if (type == kStackPointerKernelStackHack) {
        CheckKernelStack();
    }
}

    #pragma mark -


EmStream& operator<<(EmStream& s, const StackRange& range) {
    s << range.fBottom;
    s << range.fTop;
    s << range.fLowWaterMark;

    return s;
}

EmStream& operator>>(EmStream& s, StackRange& range) {
    s >> range.fBottom;
    s >> range.fTop;
    s >> range.fLowWaterMark;

    return s;
}

#endif
