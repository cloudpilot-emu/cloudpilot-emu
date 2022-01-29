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

#include "EmCPU68K.h"

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif

#include <algorithm>  // find

#include "Byteswapping.h"  // Canonical
#include "ChunkHelper.h"
#include "DebugMgr.h"   // gExceptionAddress, gExceptionSize, gExceptionForRead
#include "EmBankROM.h"  // EmBankROM::GetMemoryStart
#include "EmCommon.h"
#include "EmHAL.h"      // EmHAL::GetInterruptLevel
#include "EmMemory.h"   // CEnableFullAccess
#include "EmSession.h"  // HandleInstructionBreak
#include "Logging.h"
#include "MetaMemory.h"
#include "Miscellaneous.h"
#include "Platform.h"
#include "Savestate.h"
#include "SavestateLoader.h"
#include "SavestateStructures.h"
#include "StringData.h"  // kExceptionNames
#include "SuspendContext.h"
#include "SuspendManager.h"
#include "UAE.h"  // cpuop_func, etc.

//#define TRACE_FUNCTION_CALLS

constexpr uint32 SAVESTATE_VERSION = 1;

// Define our own flags for regs.spcflag.  Please do not let these
// overlap with UAE-defined flags (should not fall below 0x0002000)
// and avoid using the high bit just for safety.

#define SPCFLAG_END_OF_CYCLE (0x40000000)

// Data needed by UAE.

int areg_byteinc[] = {1, 1, 1, 1, 1, 1, 1, 2};  // (normally in newcpu.c)
int imm8_table[] = {8, 1, 2, 3, 4, 5, 6, 7};    // (normally in newcpu.c)

int movem_index1[256];  // (normally in newcpu.c)
int movem_index2[256];  // (normally in newcpu.c)
int movem_next[256];    // (normally in newcpu.c)

#ifdef __EMSCRIPTEN__
cpuop_func* cpufunctbl_base;
#else
cpuop_func* cpufunctbl[65536];  // (normally in newcpu.c)
#endif

uint16 last_op_for_exception_3;    /* Opcode of faulting instruction */
emuptr last_addr_for_exception_3;  /* PC at fault time */
emuptr last_fault_for_exception_3; /* Address that generated the exception */

struct regstruct regs;        // (normally in newcpu.c)
struct flag_struct regflags;  // (normally in support.c)

// These variables should strictly be in a sub-system that implements
// the stack overflow checking, etc.  However, for performance reasons,
// we need to expose them to UAE (see the CHECK_STACK_POINTER_ASSIGNMENT,
// et al macros), so define them here.
//
// Similar comments for the CheckKernelStack function.

uae_u32 gStackHigh;
uae_u32 gStackLowWarn;
uae_u32 gStackLow;
uae_u32 gKernelStackOverflowed;

// Definitions of the stack frames used in EmCPU68K::ProcessException.

#include "PalmPack.h"

struct ExceptionStackFrame1 {
    uint16 statusRegister;
    uint32 programCounter;
};

struct ExceptionStackFrame2 {
    uint16 functionCode;
    uint32 accessAddress;
    uint16 instructionRegister;
    uint16 statusRegister;
    uint32 programCounter;
};

#include "PalmPackPop.h"

EmCPU68K* gCPU68K;

// ---------------------------------------------------------------------------
//		� EmCPU68K::Cycle
// ---------------------------------------------------------------------------

// this guy is a macro instead of an inline function so that "counter" can
// be declared as a variable local to the calling function.  The resulting
// code can be more efficient if "counter" can be cached in a register
// instead of being a static or global variable.

#define CYCLE(sleeping)                                                                  \
    {                                                                                    \
        /* Don't do anything if we're in the middle of an ATrap call.  We don't */       \
        /* need interrupts firing or tmr counters incrementing. */                       \
                                                                                         \
        EmAssert(session);                                                               \
        if (!session->IsNested()) {                                                      \
            /* Perform CPU-specific idling. */                                           \
            EmHAL::DispatchCycle(session->GetSystemCycles() + fCurrentCycles, sleeping); \
                                                                                         \
            /* Perform expensive operations. */                                          \
                                                                                         \
            if (sleeping || ((counter++ & 0x7FFF) == 0)) {                               \
                this->CycleSlowly(sleeping);                                             \
            }                                                                            \
        }                                                                                \
    }

namespace {
#ifdef TRACE_FUNCTION_CALLS
    void traceFunctionCalls(uint16 opcode, emuptr pc) {
        if (opcode == 0x4e56 && (pc & 0x10000000) == 0x10000000) {
            emuptr addr;
            for (addr = pc; addr - pc < 0x1000; addr += 2) {
                if (EmMemGet16(addr) == 0x4e75) break;
            }

            if (addr - pc >= 0x1000) {
                cout << "no return found at " << hex << pc << endl << flush;
            } else {
                char fname[32];
                int i;

                for (i = 0; i < 32; i++) {
                    fname[i] = EmMemGet8(addr + 3 + i);
                    if (fname[i] == 0 || fname[i] > 127 || fname[i] < 0x20) break;
                }

                if (i < 32 && fname[i] == 0) {
                    cout << "call -> " << hex << (pc & 0x0fffffff) << " : " << fname << endl
                         << flush;
                } else {
                    cout << "call ->" << hex << (pc & 0x0fffffff) << endl << flush;
                }
            }
        }

        if (opcode == 0x4e75 && (pc & 0x10000000) == 0x10000000) {
            char fname[32];
            int i;

            for (i = 0; i < 32; i++) {
                fname[i] = EmMemGet8(pc + 3 + i);
                if (fname[i] == 0 || fname[i] > 127 || fname[i] < 0x20) break;
            }

            if (i < 32 && fname[i] == 0) {
                cout << "rtrn <- " << hex << (pc & 0x0fffffff) << " : " << fname << endl << flush;
            } else {
                cout << "rtrn <- " << hex << (pc & 0x0fffffff) << endl << flush;
            }
        }
    }
#endif
}  // namespace

// ---------------------------------------------------------------------------
//		� EmCPU68K::EmCPU68K
// ---------------------------------------------------------------------------

EmCPU68K::EmCPU68K(EmSession* session)
    : EmCPU(session),
      fLastTraceAddress(EmMemNULL),
      //	fExceptionHandlers (),
      fHookJSR_Ind()
#if REGISTER_HISTORY
      ,
      fRegHistoryIndex(0)
//	, fRegHistory ()
#endif
#if EXCEPTION_HISTORY
      ,
      fExceptionHistoryIndex(0)
//	, fExceptionHistory ()
#endif
{
    this->InitializeUAETables();

    EmAssert(gCPU68K == NULL);
    gCPU68K = this;
}

// ---------------------------------------------------------------------------
//		� EmCPU68K::~EmCPU68K
// ---------------------------------------------------------------------------

EmCPU68K::~EmCPU68K(void) {
    EmAssert(gCPU68K == this);
    gCPU68K = NULL;
}

#pragma mark -

// ---------------------------------------------------------------------------
//		� EmCPU68K::Reset
// ---------------------------------------------------------------------------

void EmCPU68K::Reset(Bool hardwareReset) {
    isSettingUpExceptionFrame = false;
    fLastTraceAddress = EmMemNULL;
#if REGISTER_HISTORY
    fRegHistoryIndex = 0;
#endif

#if EXCEPTION_HISTORY
    fExceptionHistoryIndex = 0;
#endif

    gStackHigh = EmMemEOM;
    gStackLowWarn = EmMemNULL;
    gStackLow = EmMemNULL;
    gKernelStackOverflowed = false;

    if (hardwareReset) {
        // (taken from m68k_reset in newcpu.c)

        // !!! I think that we really need to emulate ROM appearing at NULL.
        // That would break our dependency on EmBankROM.

        emuptr romStart = EmBankROM::GetMemoryStart();
        m68k_areg(regs, 7) = EmMemGet32(romStart);
        m68k_setpc(EmMemGet32(romStart + 4));

        // Note, on the 68K, the t0 and m flags must always be zero.

        regs.prefetch = 0x0000;
        regs.kick_mask = 0xF80000;  // (not a 68K register)
        regs.s = 1;                 // supervisor/user state
        regs.m = 0;                 // master/interrupt state
        regs.stopped = 0;           // (not a 68K register)
        regs.t1 = 0;                // 1 = trace on any instruction
        regs.t0 = 0;                // 1 = trace on change of flow
        SET_ZFLG(0);
        SET_XFLG(0);
        SET_CFLG(0);
        SET_VFLG(0);
        SET_NFLG(0);
        regs.spcflags = 0;  // (not a 68K register)
        regs.intmask = 7;   // disable all interrupts
        regs.vbr = regs.sfc = regs.dfc = 0;
        regs.fpcr = regs.fpsr = regs.fpiar = 0;
    }

    Memory::CheckNewPC(m68k_getpc());
}

// ---------------------------------------------------------------------------
//		� EmCPU68K::Save
// ---------------------------------------------------------------------------

void EmCPU68K::Save(Savestate& savestate) { DoSave(savestate); }

void EmCPU68K::Save(SavestateProbe& savestate) { DoSave(savestate); }

void EmCPU68K::Load(SavestateLoader& loader) {
    Chunk* chunk = loader.GetChunk(ChunkType::cpu68k);
    if (!chunk) {
        logging::printf("error restoring cpu68k: missing savestate");
        loader.NotifyError();

        return;
    }

    if (chunk->Get32() != SAVESTATE_VERSION) {
        logging::printf("error restoring cpu68k: savestate version mismatch");
        loader.NotifyError();

        return;
    }

    regstruct regs;

    LoadChunkHelper helper(*chunk);
    ::DoSaveLoad(helper, regs);
    DoSaveLoad(helper);

    SetRegisters(regs);
}

template <typename T>
void EmCPU68K::DoSave(T& savestate) {
    typename T::chunkT* chunk = savestate.GetChunk(ChunkType::cpu68k);
    if (!chunk) return;

    chunk->Put32(SAVESTATE_VERSION);

    regstruct regs;
    GetRegisters(regs);

    SaveChunkHelper helper(*chunk);
    ::DoSaveLoad(helper, regs);
    DoSaveLoad(helper);
}

template <typename T>
void EmCPU68K::DoSaveLoad(T& helper) {
    helper.Do32(fLastTraceAddress);
}

#pragma mark -

// ---------------------------------------------------------------------------
//		� EmCPU68K::Execute
// ---------------------------------------------------------------------------

uint32 EmCPU68K::Execute(uint32 maxCycles) {
    // This function is the bottleneck for all 68K emulation.  It's
    // important that it run as quickly as possible.  To that end,
    // fine tune register allocation as much as we can by hand.

    EmValueChanger<uint32> resetCurrentCycles(fCurrentCycles, 0);

    // Do not run cycleSlowly on each call if single stepping
    int counter = maxCycles ? 0 : 1;

    uint32 cycles;

#define pc (regs.pc)
#define spcflags (regs.spcflags)
#define session (fSession)

    EmAssert(session);

    // -----------------------------------------------------------------------
    // Check for the stopped flag before entering the "execute an opcode"
    // section.  It could be that we last exited the loop while still in stop
    // mode, and we need to wind our way back down to that spot.
    // -----------------------------------------------------------------------

    if (regs.stopped) goto StoppedLoop;

    while (1) {
#if REGISTER_HISTORY
        // -----------------------------------------------------------------------
        // Save the registers for the post-mortem, but don't record the
        // instructions we generate when calling the ROM as a subroutine.  We want
        // those to be as transparent as possible.	In particular, we don't want
        // any functions that we call as part of figuring out why a problem
        // occurred to knock the problem-causing registers off of our array.
        // -----------------------------------------------------------------------

        //		if (!session->IsNested ())
        {
            ++fRegHistoryIndex;
            fRegHistory[fRegHistoryIndex & (kRegHistorySize - 1)] = regs;
        }
#endif
        // -----------------------------------------------------------------------
        // See if we need to halt CPU execution at this location.  We could need
        // to do this for several reasons, including hitting soft breakpoints or
        // needing to execute tailpatches.
        // -----------------------------------------------------------------------

        if (MetaMemory::IsCPUBreak(m68k_getpc())) {
            session->HandleInstructionBreak();
        }

        if (SuspendManager::IsSuspended() && !gSession->IsNested()) break;

        // =======================================================================
        // Execute the opcode.
        // -----------------------------------------------------------------------

        EmOpcode68K opcode;

        opcode = EmMemGet16(pc);
#ifdef TRACE_FUNCTION_CALLS
        traceFunctionCalls(opcode, pc);
#endif

#ifdef __EMSCRIPTEN__
        cycles = ((cpuop_func*)((long)cpufunctbl_base + opcode))(opcode);
#else
        cycles = (cpufunctbl[opcode])(opcode);
#endif
        fCurrentCycles += cycles;
        // =======================================================================

        // Perform periodic tasks.

        CYCLE(false);

    StoppedLoop:

        // -----------------------------------------------------------------------
        // Handle special conditions.  NB: the code reached by calling
        // EmCPU68K::ExecuteSpecial used to be inline in this function.  Moving it
        // out (thus simplifying both EmCPU68K::Execute and EmCPU68K::ExecuteSpecial)
        // sped up the CPU loop by 9%!
        // -----------------------------------------------------------------------

        if (spcflags || regs.stopped) {
            if (this->ExecuteSpecial(maxCycles)) break;
        }

        if (fCurrentCycles >= maxCycles) break;

    }  // while (1)

#undef pc
#undef spcflags
#undef session

    return fCurrentCycles;
}

// ---------------------------------------------------------------------------
//		� EmCPU68K::ExecuteSpecial
// ---------------------------------------------------------------------------

Bool EmCPU68K::ExecuteSpecial(uint32 maxCycles) {
    EmAssert(fSession);

    if (fSession->IsNested()) {
        return this->CheckForBreak();
    }

    // Check for Reset first.  If this is set, don't do anything else.

    if ((regs.spcflags & SPCFLAG_END_OF_CYCLE)) {
        if (fSession->ExecuteSpecial(true)) return true;
    }

    // Execute UAE spcflags-handling code (from do_specialties in newcpu.c).

    // If we're tracing, execute the trace vector.
    //
    // The check for SPCFLAG_STOP was added in Poser.  It's needed
    // if we're re-entering ExecuteStopped loop on the Mac after
    // exiting in order to handle events.

    if ((regs.spcflags & SPCFLAG_DOTRACE) && !(regs.stopped)) {
        this->ProcessException(kException_Trace);
    }

    if (regs.stopped && this->ExecuteStoppedLoop(maxCycles)) return true;

    // Do trace-mode stuff (do_trace from newcpu.c does more,
    // but it's only needed for CPU_LEVEL > 0)

    if (regs.spcflags & SPCFLAG_TRACE) {
        if (regs.t1) {
            fLastTraceAddress = m68k_getpc();
            regs.spcflags &= ~SPCFLAG_TRACE;
            regs.spcflags |= SPCFLAG_DOTRACE;
        }
    }

    if ((regs.spcflags & SPCFLAG_DOINT) && !gKernelStackOverflowed &&
        !SuspendManager::IsSuspended()) {
        int32 interruptLevel = EmHAL::GetInterruptLevel();
        regs.spcflags &= ~SPCFLAG_DOINT;  // was ~(SPCFLAG_INT | SPCFLAG_DOINT) in Greg and Craig,
                                          // but the latest UAE has this
        if ((interruptLevel != -1) && (interruptLevel > regs.intmask)) {
            this->ProcessInterrupt(interruptLevel);
            m68k_setstopped(0);
        }
    }

    if (regs.spcflags & SPCFLAG_INT) {
        regs.spcflags &= ~SPCFLAG_INT;
        regs.spcflags |= SPCFLAG_DOINT;
    }

    if ((regs.spcflags & SPCFLAG_END_OF_CYCLE)) {
        regs.spcflags &= ~SPCFLAG_END_OF_CYCLE;

        if (fSession->ExecuteSpecial(false)) return true;
    }

    return this->CheckForBreak();
}

// ---------------------------------------------------------------------------
//		� EmCPU68K::ExecuteStoppedLoop
// ---------------------------------------------------------------------------

Bool EmCPU68K::ExecuteStoppedLoop(uint32 maxCycles) {
    EmSession* session = fSession;

    EmAssert(session);
    EmAssert(regs.intmask < 7);
    EmAssert(!SuspendManager::IsSuspended());

    // Do not run cycleSlowly on each call if single stepping
    int counter = maxCycles ? 0 : 1;

    // While the CPU is stopped (because a STOP instruction was
    // executed) do some idle tasks.

    do {
        uint32 cyclesToNextInterrupt =
            EmHAL::CyclesToNextInterrupt(session->GetSystemCycles() + fCurrentCycles);
        fCurrentCycles += ((gSession->IsPowerOn() && cyclesToNextInterrupt > 0 &&
                            cyclesToNextInterrupt != 0xffffffff)
                               ? cyclesToNextInterrupt
                               : (maxCycles > 0 ? maxCycles : 1));

        CYCLE(true);

        // Process an interrupt (see if it's time to wake up).

        if (regs.spcflags & (SPCFLAG_INT | SPCFLAG_DOINT)) {
            int32 interruptLevel = EmHAL::GetInterruptLevel();

            regs.spcflags &= ~(SPCFLAG_INT | SPCFLAG_DOINT);

            if ((interruptLevel != -1) && (interruptLevel > regs.intmask)) {
                this->ProcessInterrupt(interruptLevel);
                m68k_setstopped(0);
            }
        }

        if (regs.stopped && gSession->IsPowerOn())
            logging::printf("WARNING: CPU failed to wake up after %u cycles",
                            cyclesToNextInterrupt);

        if (this->CheckForBreak() || (fCurrentCycles >= maxCycles)) {
            return true;
        }
    } while (regs.stopped);

    return false;
}

// ---------------------------------------------------------------------------
//		� EmCPU68K::CycleSlowly
// ---------------------------------------------------------------------------

void EmCPU68K::CycleSlowly(Bool sleeping) { EmHAL::CycleSlowly(sleeping); }

// ---------------------------------------------------------------------------
//		� EmCPU68K::CheckAfterCycle
// ---------------------------------------------------------------------------

void EmCPU68K::CheckAfterCycle(void) { regs.spcflags |= SPCFLAG_END_OF_CYCLE; }

// ---------------------------------------------------------------------------
//		� EmCPU68K::GetPC
// ---------------------------------------------------------------------------

emuptr EmCPU68K::GetPC(void) {
    //	return this->GetRegister (e68KRegID_PC);
    return m68k_getpc();
}

// ---------------------------------------------------------------------------
//		� EmCPU68K::GetSP
// ---------------------------------------------------------------------------

emuptr EmCPU68K::GetSP(void) {
    //	return this->GetRegister (e68KRegID_SSP);
    return m68k_areg(regs, 7);
}

// ---------------------------------------------------------------------------
//		� EmCPU68K::GetRegister
// ---------------------------------------------------------------------------

uint32 EmCPU68K::GetRegister(int index) {
    uint32 result = 0;

    switch (index) {
        case e68KRegID_D0:
            result = m68k_dreg(regs, 0);
            break;
        case e68KRegID_D1:
            result = m68k_dreg(regs, 1);
            break;
        case e68KRegID_D2:
            result = m68k_dreg(regs, 2);
            break;
        case e68KRegID_D3:
            result = m68k_dreg(regs, 3);
            break;
        case e68KRegID_D4:
            result = m68k_dreg(regs, 4);
            break;
        case e68KRegID_D5:
            result = m68k_dreg(regs, 5);
            break;
        case e68KRegID_D6:
            result = m68k_dreg(regs, 6);
            break;
        case e68KRegID_D7:
            result = m68k_dreg(regs, 7);
            break;
        case e68KRegID_A0:
            result = m68k_areg(regs, 0);
            break;
        case e68KRegID_A1:
            result = m68k_areg(regs, 1);
            break;
        case e68KRegID_A2:
            result = m68k_areg(regs, 2);
            break;
        case e68KRegID_A3:
            result = m68k_areg(regs, 3);
            break;
        case e68KRegID_A4:
            result = m68k_areg(regs, 4);
            break;
        case e68KRegID_A5:
            result = m68k_areg(regs, 5);
            break;
        case e68KRegID_A6:
            result = m68k_areg(regs, 6);
            break;
        case e68KRegID_USP:
            result = m68k_areg(regs, 7);
            break;
        case e68KRegID_SSP:
            result = m68k_areg(regs, 7);
            break;
        case e68KRegID_PC:
            result = m68k_getpc();
            break;
        case e68KRegID_SR:
            this->UpdateSRFromRegisters();
            result = regs.sr;
            break;

        default:
            EmAssert(false);
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmCPU68K::SetPC
// ---------------------------------------------------------------------------

void EmCPU68K::SetPC(emuptr newPC) { this->SetRegister(e68KRegID_PC, newPC); }

// ---------------------------------------------------------------------------
//		� EmCPU68K::SetSP
// ---------------------------------------------------------------------------

void EmCPU68K::SetSP(emuptr newPC) { this->SetRegister(e68KRegID_SSP, newPC); }

// ---------------------------------------------------------------------------
//		� EmCPU68K::SetRegister
// ---------------------------------------------------------------------------

void EmCPU68K::SetRegister(int index, uint32 val) {
    switch (index) {
        case e68KRegID_D0:
            m68k_dreg(regs, 0) = val;
            break;
        case e68KRegID_D1:
            m68k_dreg(regs, 1) = val;
            break;
        case e68KRegID_D2:
            m68k_dreg(regs, 2) = val;
            break;
        case e68KRegID_D3:
            m68k_dreg(regs, 3) = val;
            break;
        case e68KRegID_D4:
            m68k_dreg(regs, 4) = val;
            break;
        case e68KRegID_D5:
            m68k_dreg(regs, 5) = val;
            break;
        case e68KRegID_D6:
            m68k_dreg(regs, 6) = val;
            break;
        case e68KRegID_D7:
            m68k_dreg(regs, 7) = val;
            break;
        case e68KRegID_A0:
            m68k_areg(regs, 0) = val;
            break;
        case e68KRegID_A1:
            m68k_areg(regs, 1) = val;
            break;
        case e68KRegID_A2:
            m68k_areg(regs, 2) = val;
            break;
        case e68KRegID_A3:
            m68k_areg(regs, 3) = val;
            break;
        case e68KRegID_A4:
            m68k_areg(regs, 4) = val;
            break;
        case e68KRegID_A5:
            m68k_areg(regs, 5) = val;
            break;
        case e68KRegID_A6:
            m68k_areg(regs, 6) = val;
            break;
        case e68KRegID_USP:
            m68k_areg(regs, 7) = val;
            break;
        case e68KRegID_SSP:
            m68k_areg(regs, 7) = val;
            break;
        case e68KRegID_PC:
            m68k_setpc(val);
            break;
        case e68KRegID_SR:
            regs.sr = val;
            this->UpdateRegistersFromSR();
            break;

        default:
            EmAssert(false);
    }
}

// ---------------------------------------------------------------------------
//		� EmCPU68K::Stopped
// ---------------------------------------------------------------------------
// Return whether or not the CPU itself is halted.  This is seperate from
// whether or not the session (that is, the thread emulating the CPU) is
// halted.

Bool EmCPU68K::Stopped(void) { return regs.stopped; }

// ---------------------------------------------------------------------------
//		� EmCPU68K::CheckForBreak
// ---------------------------------------------------------------------------
// Check to see if the conditions tell us to break from the CPU Execute loop.

Bool EmCPU68K::CheckForBreak(void) {
    EmAssert(fSession);
    return fSession->CheckForBreak();
}

#pragma mark -

// ---------------------------------------------------------------------------
//		� EmCPU68K::ProcessInterrupt
// ---------------------------------------------------------------------------

void EmCPU68K::ProcessInterrupt(int32 interrupt) {
    this->ProcessException((ExceptionNumber)(EmHAL::GetInterruptBase() + interrupt));

    regs.intmask = interrupt;
    regs.spcflags |= SPCFLAG_INT;  // Check for higher-level interrupts
}

// ---------------------------------------------------------------------------
//		� EmCPU68K::ProcessException
// ---------------------------------------------------------------------------

void EmCPU68K::ProcessException(ExceptionNumber exception) {
    if (isSettingUpExceptionFrame) {
        if (!gSession->IsNested()) gSession->ScheduleReset(EmSession::ResetType::soft);

        return;
    }

    // Make sure the Status Register is up-to-date.
    this->UpdateSRFromRegisters();

#if EXCEPTION_HISTORY
    // Save the exception for the post-mortem

    fExceptionHistoryIndex++;

    long index = fExceptionHistoryIndex & (kExceptionHistorySize - 1);
    fExceptionHistory[index].name = kExceptionNames[exception];

    fExceptionHistory[index].pc = m68k_getpc();
    fExceptionHistory[index].sp = m68k_areg(regs, 7);
#endif

    // Let any custom exception handler have a go at it.  If it returns
    // true, it handled it completely, and we don't have anything else to do.
    //
    // By "handled it completely", we could mean one of two things.  (1)
    // that Poser completely replaced the system function and the ROM
    // function was skipped, or (2) Poser dealt with the trap dispatch
    // process, setting the PC to the function entry point.
    //
    // If profiling is on, (2) never occurs because we want to profile the
    // trap dispatcher itself.  Therefore, only (1) is possible.

    Bool handled = false;
    Hook68KExceptionList& fns = fExceptionHandlers[exception];
    Hook68KExceptionList::iterator iter = fns.begin();
    while (iter != fns.end()) {
        if ((*iter)(exception)) {
            handled = true;
        }

        ++iter;
    }

    if (handled) {
        return;
    }

    EmAssert(!SuspendManager::IsSuspended() || !SuspendManager::GetContext().RequiresStackAccess());

    // The following is vaguely modelled after Exception() from newcpu.c
    // (The call to MakeSR appears at the start of this method).

    // If not in supervisor mode, set the usp, restore A7 from the isp,
    // and transfer to supervisor mode.

    if (!regs.s) {
        regs.usp = m68k_areg(regs, 7);
        m68k_areg(regs, 7) = regs.isp;
        regs.s = 1;
    }

    // Set up the stack frame.
    // !!! If we're handling a trace exception, I think that fLastTraceAddress
    // comes into play here instead of m68k_getpc.
    // !!! Manage this with EmPalmStructs...
    {
        EmValueChanger trackHandleException(isSettingUpExceptionFrame, true);

        if (exception == kException_BusErr || exception == kException_AddressErr) {
            COMPILE_TIME_ASSERT(sizeof(ExceptionStackFrame2) == 14);
            m68k_areg(regs, 7) -= sizeof(ExceptionStackFrame2);
            CHECK_STACK_POINTER_DECREMENT();

            emuptr frame = m68k_areg(regs, 7);

            // Eh...Palm OS doesn't use these 3 anyway...
            EmMemPut16(frame + offsetof(ExceptionStackFrame2, functionCode), 0);
            EmMemPut32(frame + offsetof(ExceptionStackFrame2, accessAddress), 0);
            EmMemPut16(frame + offsetof(ExceptionStackFrame2, instructionRegister), 0);

            EmMemPut16(frame + offsetof(ExceptionStackFrame2, statusRegister), regs.sr);
            EmMemPut32(frame + offsetof(ExceptionStackFrame2, programCounter), m68k_getpc());
        } else {
            COMPILE_TIME_ASSERT(sizeof(ExceptionStackFrame1) == 6);
            m68k_areg(regs, 7) -= sizeof(ExceptionStackFrame1);
            CHECK_STACK_POINTER_DECREMENT();

            emuptr frame = m68k_areg(regs, 7);

            EmMemPut16(frame + offsetof(ExceptionStackFrame1, statusRegister), regs.sr);
            EmMemPut32(frame + offsetof(ExceptionStackFrame1, programCounter), m68k_getpc());
        }
    }

    emuptr newpc;
    {
        CEnableFullAccess munge;  // Remove blocks on memory access.

        // Get the exception handler address.
        newpc = EmMemGet32(regs.vbr + 4 * exception);
    }

    // Check the exception handler address and jam it into the PC.

    m68k_setpc(newpc);

    // Turn tracing off.

    regs.t1 = regs.t0 = regs.m = 0;
    regs.spcflags &= ~(SPCFLAG_TRACE | SPCFLAG_DOTRACE);
}

// ---------------------------------------------------------------------------
//		� EmCPU68K::ProcessIllegalInstruction
// ---------------------------------------------------------------------------

void EmCPU68K::ProcessIllegalInstruction(EmOpcode68K opcode) {
    // This function is loosely based on op_illg in newcpu.c

    // Process an FTrap.

    if ((opcode & 0xF000) == 0xF000) {
        this->ProcessException(kException_FTrap);
    }

    // Process an ATrap.

    else if ((opcode & 0xF000) == 0xA000) {
        this->ProcessException(kException_ATrap);
    }

    // Process all other opcodes.

    else {
        this->ProcessException(kException_IllegalInstr);
    }
}

// ---------------------------------------------------------------------------
//		� EmCPU68K::ProcessJSR_Ind
// ---------------------------------------------------------------------------

int EmCPU68K::ProcessJSR_Ind(emuptr oldPC, emuptr dest) {
    int handled = false;

    Hook68KJSR_IndList::iterator iter = fHookJSR_Ind.begin();
    while (iter != fHookJSR_Ind.end()) {
        if ((*iter)(oldPC, dest)) {
            handled = true;
        }

        ++iter;
    }

    return handled;
}

#pragma mark -

// ---------------------------------------------------------------------------
//		� EmCPU68K::InstallHookException
//		� EmCPU68K::InstallHookJSR
//		� EmCPU68K::InstallHookJSR_Ind
//		� EmCPU68K::InstallHookLINK
//		� EmCPU68K::InstallHookRTE
//		� EmCPU68K::InstallHookRTS
//		� EmCPU68K::InstallHookNewPC
//		� EmCPU68K::InstallHookNewSP
// ---------------------------------------------------------------------------

void EmCPU68K::InstallHookException(ExceptionNumber exceptionNumber, Hook68KException fn) {
    fExceptionHandlers[exceptionNumber].push_back(fn);
}

void EmCPU68K::InstallHookJSR_Ind(Hook68KJSR_Ind fn) { fHookJSR_Ind.push_back(fn); }

// ---------------------------------------------------------------------------
//		� EmCPU68K::RemoveHookException
//		� EmCPU68K::RemoveHookJSR
//		� EmCPU68K::RemoveHookJSR_Ind
//		� EmCPU68K::RemoveHookLINK
//		� EmCPU68K::RemoveHookRTE
//		� EmCPU68K::RemoveHookRTS
//		� EmCPU68K::RemoveHookNewPC
//		� EmCPU68K::RemoveHookNewSP
// ---------------------------------------------------------------------------

void EmCPU68K::RemoveHookException(ExceptionNumber exceptionNumber, Hook68KException fn) {
    Hook68KExceptionList::iterator iter = find(fExceptionHandlers[exceptionNumber].begin(),
                                               fExceptionHandlers[exceptionNumber].end(), fn);

    if (iter != fExceptionHandlers[exceptionNumber].end()) {
        fExceptionHandlers[exceptionNumber].erase(iter);
    }
}

void EmCPU68K::RemoveHookJSR_Ind(Hook68KJSR_Ind fn) {
    Hook68KJSR_IndList::iterator iter = find(fHookJSR_Ind.begin(), fHookJSR_Ind.end(), fn);

    if (iter != fHookJSR_Ind.end()) {
        fHookJSR_Ind.erase(iter);
    }
}

#pragma mark -

// ---------------------------------------------------------------------------
//		� EmCPU68K::GetRegisters
// ---------------------------------------------------------------------------

void EmCPU68K::GetRegisters(regstruct& registers) {
    this->UpdateSRFromRegisters();

    registers = regs;
    registers.pc = m68k_getpc();
}

// ---------------------------------------------------------------------------
//		� EmCPU68K::SetRegisters
// ---------------------------------------------------------------------------

void EmCPU68K::SetRegisters(regstruct& registers) {
    regs = registers;
    this->UpdateRegistersFromSR();

    m68k_setpc(registers.pc);
}

// ---------------------------------------------------------------------------
//		� EmCPU68K::UpdateSRFromRegisters
// ---------------------------------------------------------------------------
// Create a 16-bit status register value from the broken out fields.  In
// general, we keep the various fields separate for speed of access.  However,
// there are times when we need the packed 16-bit field.
//
// This function is called any time the 16-bit representation of the SR is
// needed:
//
//		on EORSR
//		on ORSR
//		on ANDSR
//		on MVSR2
//		on MV2SR
//		on RTR
//		when processing exceptions (EmCPU68K::ProcessException)
//
//		SystemPacket::GetRegs
//		EmCPU68K::GetRegisters

void EmCPU68K::UpdateSRFromRegisters(void) {
    // (taken from MakeSR in newcpu.c)

    regs.sr =
        ((regs.t1 << 15) | (regs.t0 << 14) | (regs.s << 13) | (regs.m << 12) | (regs.intmask << 8) |
         (GET_XFLG << 4) | (GET_NFLG << 3) | (GET_ZFLG << 2) | (GET_VFLG << 1) | GET_CFLG);
}

// ---------------------------------------------------------------------------
//		� EmCPU68K::UpdateRegistersFromSR
// ---------------------------------------------------------------------------
// Break out all of the fields from the 16-bit status register into their own
// separate variables.
//
// This function is called any time the SR has been update and needs to be
// re-expanded:
//
//		on EORSR
//		on ORSR
//		on ANDSR
//		on MV2SR
//		on STOP
//		on RTE
//		on RTR
//
//		SystemPacket::SetRegs
//		EmCPU68K::SetRegisters

void EmCPU68K::UpdateRegistersFromSR(void) {
    // (taken from MakeFromSR in newcpu.c)

    //	int oldm = regs.m;
    int olds = regs.s;

    regs.t1 = (regs.sr >> 15) & 1;
    regs.t0 = (regs.sr >> 14) & 1;
    regs.s = (regs.sr >> 13) & 1;
    regs.m = (regs.sr >> 12) & 1;
    regs.intmask = (regs.sr >> 8) & 7;

    SET_XFLG((regs.sr >> 4) & 1);
    SET_NFLG((regs.sr >> 3) & 1);
    SET_ZFLG((regs.sr >> 2) & 1);
    SET_VFLG((regs.sr >> 1) & 1);
    SET_CFLG(regs.sr & 1);

    if (olds != regs.s) {
        if (olds) {
            regs.isp = m68k_areg(regs, 7);
            m68k_areg(regs, 7) = regs.usp;
        } else {
            regs.usp = m68k_areg(regs, 7);
            m68k_areg(regs, 7) = regs.isp;
        }
    }

    regs.spcflags |= SPCFLAG_INT;

    if (regs.t1 || regs.t0) {
        regs.spcflags |= SPCFLAG_TRACE;
    } else {
        regs.spcflags &= ~(SPCFLAG_TRACE | SPCFLAG_DOTRACE);
    }
}

// ---------------------------------------------------------------------------
//		� EmCPU68K::BusError
// ---------------------------------------------------------------------------

void EmCPU68K::BusError(emuptr address, long size, Bool forRead) {
    gExceptionAddress = address;
    gExceptionSize = size;
    gExceptionForRead = forRead;

    this->ProcessException(kException_BusErr);
    return;
}

// ---------------------------------------------------------------------------
//		� EmCPU68K::AddressError
// ---------------------------------------------------------------------------

void EmCPU68K::AddressError(emuptr address, long size, Bool forRead) {
    gExceptionAddress = address;
    gExceptionSize = size;
    gExceptionForRead = forRead;

    this->ProcessException(kException_AddressErr);
    return;
}

#pragma mark -

// ---------------------------------------------------------------------------
//		� EmCPU68K::InitializeUAETables
// ---------------------------------------------------------------------------

void EmCPU68K::InitializeUAETables(void) {
    static int initialized;

    // All of the stuff in this function needs to be done only once;
    // it doesn't need to be executed every time we create a new CPU.

    if (initialized) return;

    initialized = true;

    // Initialize some CPU-related tables
    // (This initialization code is taken from init_m68k in newcpu.c)

    int i, j;

    for (i = 0; i < 256; i++) {
        for (j = 0; j < 8; j++) {
            if (i & (1 << j)) {
                break;
            }
        }

        movem_index1[i] = j;
        movem_index2[i] = 7 - j;
        movem_next[i] = i & (~(1 << j));
    }

    read_table68k();
    do_merges();

    // The rest of this code is based on build_cpufunctbl in newcpu.c.

#ifdef __EMSCRIPTEN__
    cpuop_func** cpufunctbl =
        (cpuop_func**)malloc(0xffff * sizeof(cpuop_func*));  // (normally in newcpu.c)
#endif

    unsigned long opcode;
    struct cputbl* tbl = op_smalltbl_3;

    for (opcode = 0; opcode < 65536; opcode++) {
        cpufunctbl[opcode] = op_illg;
    }

    for (i = 0; tbl[i].handler != NULL; i++) {
        if (!tbl[i].specific) {
            cpufunctbl[tbl[i].opcode] = tbl[i].handler;
#if HAS_PROFILING
            perftbl[tbl[i].opcode] = tbl[i].perf;
#endif
        }
    }

    for (opcode = 0; opcode < 65536; opcode++) {
        cpuop_func* f;

        if (table68k[opcode].mnemo == i_ILLG || table68k[opcode].clev > 0) {
            continue;
        }

        if (table68k[opcode].handler != -1) {
            f = cpufunctbl[table68k[opcode].handler];
            if (f == op_illg) {
                abort();
            }

            cpufunctbl[opcode] = f;
#if HAS_PROFILING
            perftbl[opcode] = perftbl[table68k[opcode].handler];
#endif
        }
    }

    for (i = 0; tbl[i].handler != NULL; i++) {
        if (tbl[i].specific) {
            cpufunctbl[tbl[i].opcode] = tbl[i].handler;
#if HAS_PROFILING
            perftbl[tbl[i].opcode] = tbl[i].perf;
#endif
        }
    }

#ifdef __EMSCRIPTEN__
    cpufunctbl_base = (cpuop_func*)EM_ASM_INT(
        {
            wasmTable.grow(0x10000);

            for (let i = 0; i <= 0xffff; i++)
                wasmTable.set(wasmTable.length - 0xffff - 1 + i,
                              wasmTable.get(HEAPU32[($0 >>> 2) + i]));

            return wasmTable.length - 0xffff - 1;
        },
        cpufunctbl);

    free(cpufunctbl);
#endif

    // (hey readcpu doesn't free this guy!)

    Platform::DisposeMemory(table68k);
}
