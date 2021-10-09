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
#include "EmSystemState.h"
#include "Logging.h"
#include "Miscellaneous.h"
#include "Miscellaneous.h"  // GetSystemCallContext
#include "ROMStubs.h"
#include "UAE.h"  // CHECK_STACK_POINTER_DECREMENT

#define LOG_FUNCTION_CALLS 0

namespace {
    constexpr int MIN_CYCLES_BETWEEN_EVENTS = 10000;
    constexpr int EVENT_QUEUE_SIZE = 20;
}  // namespace

static emuptr gBigROMEntry;

EmThreadSafeQueue<PenEvent> EmPalmOS::penEventQueue{EVENT_QUEUE_SIZE};
EmThreadSafeQueue<KeyboardEvent> EmPalmOS::keyboardEventQueue{EVENT_QUEUE_SIZE};
EmThreadSafeQueue<PenEvent> EmPalmOS::penEventQueueIncoming{EVENT_QUEUE_SIZE};
EmThreadSafeQueue<KeyboardEvent> EmPalmOS::keyboardEventQueueIncoming{EVENT_QUEUE_SIZE};
uint64 EmPalmOS::lastEventPromotedAt{0};

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

    ClearQueues();

    EmPatchMgr::Initialize();
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

    ClearQueues();
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

    ClearQueues();
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

    if (EmMemGet16(oldpc) == 0x4e91 && EmMemGet16(oldpc - 4) == 0x2269 &&
        EmMemGet16(oldpc - 8) == 0x2278) {
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

    CEnableFullAccess munge;

    UInt32 memSemaphoreIDP = EmLowMem_GetGlobal(memSemaphoreID);
    EmAliascj_xsmb<PAS> memSemaphoreID(memSemaphoreIDP);

    if (!gSession->IsNested() && memSemaphoreID.xsmuse == 0) {
        switch (context.fTrapWord) {
            case sysTrapHwrIRQ1Handler:
            case sysTrapHwrIRQ2Handler:
            case sysTrapHwrIRQ3Handler:
            case sysTrapHwrIRQ4Handler:
            case sysTrapHwrIRQ5Handler:
            case sysTrapHwrIRQ6Handler:
                DispatchNextEvent();
        }

        if (gSession->WaitingForSyscall()) {
            gCPU->SetPC(gCPU->GetPC() - pcAdjust);
            gSession->NotifySyscallDispatched();

            return true;
        }
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

void EmPalmOS::QueuePenEvent(PenEvent evt) {
    if (!gSession->IsPowerOn()) return;

    if (penEventQueueIncoming.GetFree() == 0) penEventQueueIncoming.Get();

    penEventQueueIncoming.Put(evt);
}

void EmPalmOS::QueueKeyboardEvent(KeyboardEvent evt) {
    if (!gSession->IsPowerOn()) return;

    if (keyboardEventQueueIncoming.GetFree() == 0) keyboardEventQueueIncoming.Get();

    keyboardEventQueueIncoming.Put(evt);
}

bool EmPalmOS::HasPenEvent() { return penEventQueue.GetUsed() != 0; }

bool EmPalmOS::HasKeyboardEvent() { return keyboardEventQueue.GetUsed() != 0; }

PenEvent EmPalmOS::PeekPenEvent() { return HasPenEvent() ? penEventQueue.Peek() : PenEvent(); }

bool EmPalmOS::DispatchNextEvent() {
    uint64 systemCycles = gSession->GetSystemCycles();

    if (systemCycles - lastEventPromotedAt < MIN_CYCLES_BETWEEN_EVENTS ||
        !gSystemState.IsUIInitialized())
        return false;

    if (DispatchPenEvent() || DispatchKeyboardEvent()) {
        lastEventPromotedAt = systemCycles;
        Wakeup();

        return true;
    }

    return false;
}

bool EmPalmOS::DispatchKeyboardEvent() {
    if (keyboardEventQueueIncoming.GetUsed() == 0) return false;

    if (keyboardEventQueue.GetFree() == 0) keyboardEventQueue.Get();
    keyboardEventQueue.Put(keyboardEventQueueIncoming.Get());

    return true;
}

bool EmPalmOS::DispatchPenEvent() {
    if (penEventQueueIncoming.GetUsed() == 0) return false;

    if (penEventQueue.GetFree() == 0) penEventQueue.Get();
    penEventQueue.Put(penEventQueueIncoming.Get());

    return true;
}

void EmPalmOS::Wakeup() {
    if (gSystemState.OSMajorVersion() >= 4) {
        EvtWakeupWithoutNilEvent();
    } else {
        EvtWakeup();
        EmLowMem::ClearNilEvent();
    }
}

void EmPalmOS::ClearQueues() {
    penEventQueue.Clear();
    keyboardEventQueue.Clear();
    penEventQueueIncoming.Clear();
    keyboardEventQueueIncoming.Clear();

    lastEventPromotedAt = gSession->GetSystemCycles();
}

void EmPalmOS::InjectEvent(CallROMType& callROM) {
    callROM = kExecuteROM;

    // Set the return value (Err) to zero in case we return
    // "true" (saying that we handled the trap).

    m68k_dreg(gRegs, 0) = 0;

    // If the low-memory global "idle" is true, then we're being
    // called from EvtGetEvent or EvtGetPen, in which case we
    // need to check if we need to post some events.

    if (EmLowMem::GetEvtMgrIdle()) {
        // If we're in the middle of calling a Palm OS function ourself,
        // and we are somehow at the point where the system is about to
        // doze, then just return now.  Don't let it doze!  Interrupts are
        // off, and HwrDoze will never return!

        if (gSession->IsNested()) {
            m68k_dreg(gRegs, 0) = 4;
            callROM = kSkipROM;
            return;
        }

        EmAssert(gSession);

        if (HasKeyboardEvent()) {
            KeyboardEvent evt = keyboardEventQueue.Get();

            if (evt.GetKey() != 0) EvtEnqueueKey(evt.GetKey(), 0, 0);
        } else if (HasPenEvent()) {
            PenEvent evt = penEventQueue.Get();
            PointType point;

            if (evt.isPenDown()) {
                point.x = evt.getX();
                point.y = evt.getY();

                TransformPenCoordinates(point.x, point.y);
                // PenScreenToRaw(&point);
            } else {
                point.x = point.y = -1;
            }

            EvtEnqueuePenPoint(&point);
        }
    }
}
