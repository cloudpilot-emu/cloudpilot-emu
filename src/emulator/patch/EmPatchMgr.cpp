/* -*- mode: C++; tab-width: 4 -*- */
/* ===================================================================== *\
        Copyright (c) 1998-2001 Palm, Inc. or its subsidiaries.
        Copyright (c) 2001 PocketPyro, Inc.
        All rights reserved.

        This file is part of the Palm OS Emulator.

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.
\* ===================================================================== */

#include "EmPatchMgr.h"

#include "ChunkHelper.h"
#include "EmCommon.h"
#include "EmHAL.h"  // EmHAL::GetLineDriverState
#include "EmLowMem.h"  // EmLowMem::GetEvtMgrIdle, EmLowMem::TrapExists, EmLowMem_SetGlobal, EmLowMem_GetGlobal
#include "EmPalmFunction.h"  // IsSystemTrap
#include "EmPatchModule.h"
#include "EmPatchModuleHtal.h"
#include "EmPatchModuleSys.h"
#include "EmSession.h"  // GetDevice
#include "KeyboardEvent.h"
#include "Logging.h"     // LogEvtAddEventToQueue, etc.
#include "MetaMemory.h"  // MetaMemory mark functions
#include "Miscellaneous.h"
#include "PenEvent.h"
#include "ROMStubs.h"  // FtrSet, FtrUnregister, EvtWakeup, ...
#include "Savestate.h"
#include "SavestateLoader.h"
#include "SavestateProbe.h"
#include "UAE.h"  // gRegs, m68k_dreg, etc.

// ======================================================================
//	Private globals and constants
// ======================================================================

// Table of currently Patched shared libraries
//
static PatchedLibIndex gPatchedLibs;

// Table of currently installed tail patches
//
static TailPatchIndex gInstalledTailpatches;

// ======================================================================
//	Private functions
// ======================================================================

// #define LOG_SYSCALLS
#ifdef LOG_SYSCALLS
    #include "DecodeSyscalls.h"
#endif

namespace {
    constexpr uint32 SAVESTATE_VERSION = 1;
    constexpr int MAX_INSTALLED_TAILPATCHES = 16;

    template <typename T>
    void DoSaveLoad(T& helper, SystemCallContext& ctx) {
        helper.Do32(ctx.fPC)
            .Do32(ctx.fNextPC)
            .Do(typename T::Pack16() << ctx.fTrapWord << ctx.fTrapIndex)
            .Do32(ctx.fExtra)
            .Do(typename T::BoolPack() << ctx.fViaTrap << ctx.fViaJsrA1);
    }

    template <typename T>
    void DoSaveLoad(T& helper, TailpatchType& patch) {
        helper.Do32(patch.fCount);

        DoSaveLoad(helper, patch.fContext);
    }

    bool executingPatch = false;
}  // namespace

EmPatchModule* EmPatchMgr::patchModuleSys = nullptr;
EmPatchModule* EmPatchMgr::patchModuleHtal = nullptr;

/***********************************************************************
 *
 * FUNCTION:	EmPatchMgr::Initialize
 *
 * DESCRIPTION: Standard initialization function.  Responsible for
 *				initializing this sub-system when a new session is
 *				created.  Will be followed by at least one call to
 *				Reset or Load.
 *
 * PARAMETERS:	none
 *
 * RETURNED:	nothing
 *
 ***********************************************************************/

void EmPatchMgr::Initialize(void) {
    EmAssert(gSession);

    if (!patchModuleSys) {
        patchModuleSys = new EmPatchModuleSys();
    }

    if (!patchModuleHtal) {
        patchModuleHtal = new EmPatchModuleHtal();
    }

    executingPatch = false;
}

/***********************************************************************
 *
 * FUNCTION:	EmPatchMgr::Reset
 *
 * DESCRIPTION:	Standard reset function.  Sets the sub-system to a
 *				default state.	This occurs not only on a Reset (as
 *				from the menu item), but also when the sub-system
 *				is first initialized (Reset is called after Initialize)
 *				as well as when the system is re-loaded from an
 *				insufficient session file.
 *
 * PARAMETERS:	none
 *
 * RETURNED:	nothing
 *
 ***********************************************************************/

void EmPatchMgr::Reset(void) {
    gInstalledTailpatches.clear();

    // Clear the installed lib patches (for "loaded" libraries)
    //
    gPatchedLibs.clear();

    executingPatch = false;
}

/***********************************************************************
 *
 * FUNCTION:	EmPatchMgr::Save
 *
 * DESCRIPTION:	Standard save function.  Saves any sub-system state to
 *				the given session file.
 *
 * PARAMETERS:	none
 *
 * RETURNED:	nothing
 *
 ***********************************************************************/

template <typename T>
void EmPatchMgr::Save(T& savestate) {
    typename T::chunkT* chunk = savestate.GetChunk(ChunkType::patchMgr);
    if (!chunk) return;

    chunk->Put32(SAVESTATE_VERSION);

    if (gInstalledTailpatches.size() > MAX_INSTALLED_TAILPATCHES) {
        logging::printf("failed to save session in EmPatchMgr: too many installed tailpatches");

        savestate.NotifyError();
        return;
    }

    chunk->Put32(gInstalledTailpatches.size());
    SaveChunkHelper helper(*chunk);

    for (auto& tailpatch : gInstalledTailpatches) DoSaveLoad(helper, tailpatch);

    if constexpr (T::chunkT::isProbe) {
        TailpatchType filler;
        filler.fContext.fViaJsrA1 = false;
        filler.fContext.fViaTrap = false;

        for (size_t i = 0; i < MAX_INSTALLED_TAILPATCHES - gInstalledTailpatches.size(); i++)
            DoSaveLoad(helper, filler);
    }
}

template void EmPatchMgr::Save<Savestate>(Savestate& savestate);
template void EmPatchMgr::Save<SavestateProbe>(SavestateProbe& savestate);

void EmPatchMgr::Load(SavestateLoader& loader) {
    Chunk* chunk = loader.GetChunk(ChunkType::patchMgr);
    if (!chunk) return;

    if (chunk->Get32() != SAVESTATE_VERSION) {
        logging::printf("error restoring PatchMgr: savestate version mismatch");
        loader.NotifyError();

        return;
    }

    gPatchedLibs.clear();
    gInstalledTailpatches.clear();
    RemoveInstructionBreaks();

    executingPatch = false;

    uint32 tailpatchCount = chunk->Get32();
    LoadChunkHelper helper(*chunk);

    for (size_t i = 0; i < tailpatchCount; i++) {
        TailpatchType patch;
        DoSaveLoad(helper, patch);

        gInstalledTailpatches.push_back(patch);
    }

    InstallInstructionBreaks();
}

/***********************************************************************
 *
 * FUNCTION:	EmPatchMgr::Dispose
 *
 * DESCRIPTION:	Standard dispose function.	Completely release any
 *				resources acquired or allocated in Initialize and/or
 *				Load.
 *
 * PARAMETERS:	none
 *
 * RETURNED:	nothing
 *
 ***********************************************************************/

void EmPatchMgr::Dispose(void) {
    gInstalledTailpatches.clear();
    gPatchedLibs.clear();

    if (patchModuleSys) {
        delete patchModuleSys;
        patchModuleSys = nullptr;
    }

    if (patchModuleHtal) {
        delete patchModuleHtal;
        patchModuleHtal = nullptr;
    }
}

/***********************************************************************
 *
 * FUNCTION:	EmPatchMgr::PostLoad
 *
 * DESCRIPTION:	Do some stuff that is normally taken care of during the
 *				process of resetting the device (autoloading
 *				applications, setting the device date, installing the
 *				HotSync user-name, and setting the 'gdbS' feature).
 *
 * PARAMETERS:	none
 *
 * RETURNED:	nothing
 *
 ***********************************************************************/

void EmPatchMgr::PostLoad(void) {
#if 0  // CSTODO
    if (EmSystemState::UIInitialized()) {
        // If we're listening on a socket, install the 'gdbS' feature.	The
        // existance of this feature causes programs written with the prc tools
        // to enter the debugger when they're launched.

        if (Debug::ConnectedToTCPDebugger()) {
            FtrSet('gdbS', 0, 0x12BEEF34);
        } else {
            FtrUnregister('gdbS', 0);
        }

        // Reconfirm the strict intl checks setting, whether on or off.

        Preference<Bool> intlPref(kPrefKeyReportStrictIntlChecks);

        if (EmPatchMgr::IntlMgrAvailable()) {
            ::IntlSetStrictChecks(*intlPref);
        }

        // Reconfirm the overlay checks setting, whether on or off.

        Preference<Bool> overlayPref(kPrefKeyReportOverlayErrors);
        (void)::FtrSet(omFtrCreator, omFtrShowErrorsFlag, *overlayPref);

        // Install the HotSync user-name.

        // Actually, let's not do that.  From Scott Maxwell:
        //
        //	Would it be possible to save the HotSync user name with each session? This
        //	would be very convenient for working on multiple projects because each
        //	session could have a different user name.
        //
        // To which I said:
        //	I think that what you're seeing is Poser (re-)establishing the user preference
        //	from the Properties/Preferences dialog box after the session is reloaded.  I
        //	could see this way of working as being valuable, too, so I'm not sure which way
        //	to go: keep things the way they are or change them.
        //
        // To which he said:
        //
        //	How about having the preferences dialog grab the name from the Palm RAM?
        //	That way you could easily maintain it per session.
        //
        // Sounds good to me...

        //		Preference<string>	userNamePref (kPrefKeyUserName);
        //		::SetHotSyncUserName (userNamePref->c_str ());

        CEnableFullAccess munge;

        if (EmLowMem::TrapExists(sysTrapDlkGetSyncInfo)) {
            char userName[dlkUserNameBufSize];
            Err err = ::DlkGetSyncInfo(NULL, NULL, NULL, userName, NULL, NULL);
            if (!err) {
                Preference<string> userNamePref(kPrefKeyUserName);
                userNamePref = string(userName);
            }
        }

        // Auto-load any files in the Autoload[Foo] directories.

        ::PrvAutoload();

        // Install the current date.

        ::PrvSetCurrentDate();

        // Wake up any current application so that they can respond
        // to events we pump in at EvtGetEvent time.

        ::EvtWakeup();
    }

    // Re-open any needed transports.  This could probably be done
    // at the time the session file is loaded, but we put it here
    // with the rest of the (deferred) post-load activities for
    // consistancy.

    for (EmUARTDeviceType ii = kUARTBegin; ii < kUARTEnd; ++ii) {
        if (EmHAL::GetLineDriverState(ii)) {
            EmTransport* transport = gEmuPrefs->GetTransportForDevice(ii);

            if (transport) {
                transport->Open();
            }
        }
    }
#endif
}

/***********************************************************************
 *
 * FUNCTION:	EmPatchMgr::GetLibPatchTable
 *
 * DESCRIPTION:	.
 *
 * PARAMETERS:	none
 *
 * RETURNED:	nothing
 *
 ***********************************************************************/

EmPatchModule* EmPatchMgr::GetLibPatchTable(uint16 refNum) {
    if (refNum >= gPatchedLibs.size()) {
        gPatchedLibs.resize(refNum + 1);
    }

    InstalledLibPatchEntry& libPtchEntry = gPatchedLibs[refNum];

    if (libPtchEntry.IsDirty() == true) {
        string libName = ::GetLibraryName(refNum);

        EmPatchModule* patchModuleIP = NULL;

        // CSTODO

        libPtchEntry.SetPatchTableP(patchModuleIP);
        libPtchEntry.SetDirty(false);
    }

    return libPtchEntry.GetPatchTableP();
}

/***********************************************************************
 *
 * FUNCTION:	EmPatchMgr::HandleSystemCall
 *
 * DESCRIPTION:	If this is a trap we could possibly have head- or
 *				tail-patched, handle those cases.
 *
 * PARAMETERS:	none
 *
 * RETURNED:	nothing
 *
 ***********************************************************************/

CallROMType EmPatchMgr::HandleSystemCall(const SystemCallContext& context) {
    EmAssert(gSession);

#if 0  // CSTODO
    if (gSession->GetNeedPostLoad()) {
        gSession->SetNeedPostLoad(false);
        EmPatchMgr::PostLoad();
    }
#endif

    HeadpatchProc hp;
    TailpatchProc tp;
    EmPatchMgr::GetPatches(context, hp, tp);

#ifdef LOG_SYSCALLS
    cout << trapWordToString(context.fTrapWord) << endl << flush;
#endif

    CallROMType handled = EmPatchMgr::HandlePatches(context, hp, tp);

    return handled;
}

/***********************************************************************
 *
 * FUNCTION:	EmPatchMgr::GetPatches
 *
 * DESCRIPTION:
 *
 * PARAMETERS:	none
 *
 * RETURNED:	nothing
 *
 ***********************************************************************/

void EmPatchMgr::GetPatches(const SystemCallContext& context, HeadpatchProc& hp,
                            TailpatchProc& tp) {
    EmPatchModule* patchModule = nullptr;

    // If this is in the system function range, check our table of
    // system function patches.

    if (::IsSystemTrap(context.fTrapWord)) {
        patchModule = patchModuleSys;
    } else if (context.fExtra ==
               EmPatchModuleHtal::kMagicRefNum)  // See comments in HtalLibSendReply.
    {
        patchModule = patchModuleHtal;
    }

    // Otherwise, see if this is a call to a patched library
    else {
        patchModule = GetLibPatchTable(context.fExtra);
    }

    // Now that we've got the right patch table for this module, see if
    // that patch table has head- or tailpatches for this function.

    if (patchModule) {
        patchModule->GetHeadpatch(context.fTrapIndex, hp);
        patchModule->GetTailpatch(context.fTrapIndex, tp);
    } else {
        hp = NULL;
        tp = NULL;
    }
}

/***********************************************************************
 *
 * FUNCTION:	EmPatchMgr::HandlePatches
 *
 * DESCRIPTION:
 *
 * PARAMETERS:	none
 *
 * RETURNED:	nothing
 *
 ***********************************************************************/

CallROMType EmPatchMgr::HandlePatches(const SystemCallContext& context, HeadpatchProc hp,
                                      TailpatchProc tp) {
    CallROMType handled = kExecuteROM;

    // First, see if we have a SysHeadpatch for this function. If so, call
    // it. If it returns true, then that means that the head patch
    // completely handled the function.

    // !!! May have to mess with PC here in case patches do something
    // to enter the debugger.

    if (hp) {
        handled = CallHeadpatch(hp);
    }

    // Next, see if there's a SysTailpatch function for this trap. If
    // so, install the TRAP that will cause us to regain control
    // after the trap function has executed.

    if (tp) {
        if (handled == kExecuteROM) {
            SetupForTailpatch(tp, context);
        } else {
            CallTailpatch(tp);
        }
    }

    return handled;
}

/***********************************************************************
 *
 * FUNCTION:	EmPatchMgr::HandleInstructionBreak
 *
 * DESCRIPTION:	Handle a tail patch, if any is registered for this
 *				memory location.
 *
 * PARAMETERS:	none
 *
 * RETURNED:	nothing
 *
 ***********************************************************************/

void EmPatchMgr::HandleInstructionBreak(void) {
    // Get the address of the tailpatch to call.  May return NULL if
    // there is no tailpatch for this memory location.

    TailpatchProc tp = RecoverFromTailpatch(gCPU->GetPC());

    // Call the tailpatch handler for the trap that just returned.

    CallTailpatch(tp);
}

/***********************************************************************
 *
 * FUNCTION:	EmPatchMgr::InstallInstructionBreaks
 *
 * DESCRIPTION:	Set the MetaMemory bit that tells the CPU loop to stop
 *				when we get to the desired locations.
 *
 * PARAMETERS:	none
 *
 * RETURNED:	nothing
 *
 ***********************************************************************/

void EmPatchMgr::InstallInstructionBreaks(void) {
    TailPatchIndex::iterator iter = gInstalledTailpatches.begin();

    while (iter != gInstalledTailpatches.end()) {
        MetaMemory::MarkInstructionBreak(iter->fContext.fNextPC);
        ++iter;
    }
}

/***********************************************************************
 *
 * FUNCTION:	EmPatchMgr::RemoveInstructionBreaks
 *
 * DESCRIPTION:	Clear the MetaMemory bit that tells the CPU loop to stop
 *				when we get to the desired locations.
 *
 * PARAMETERS:	none
 *
 * RETURNED:	nothing
 *
 ***********************************************************************/

void EmPatchMgr::RemoveInstructionBreaks(void) {
    TailPatchIndex::iterator iter = gInstalledTailpatches.begin();

    while (iter != gInstalledTailpatches.end()) {
        MetaMemory::UnmarkInstructionBreak(iter->fContext.fNextPC);
        ++iter;
    }
}

/***********************************************************************
 *
 * FUNCTION:	EmPatchMgr::SetupForTailpatch
 *
 * DESCRIPTION:	Set up the pending TRAP $F call to be tailpatched.
 *
 * PARAMETERS:	none
 *
 * RETURNED:	nothing
 *
 ***********************************************************************/

void EmPatchMgr::SetupForTailpatch(TailpatchProc tp, const SystemCallContext& context) {
    // See if this function is already tailpatched.  If so, merely increment
    // the use-count field.

    TailPatchIndex::iterator iter = gInstalledTailpatches.begin();

    while (iter != gInstalledTailpatches.end()) {
        if (iter->fContext.fNextPC == context.fNextPC) {
            ++(iter->fCount);
            return;
        }

        ++iter;
    }

    // This function is not already tailpatched, so add a new entry
    // for the the PC/opcode we want to save.

    EmAssert(gSession);
    RemoveInstructionBreaks();

    TailpatchType newTailpatch;

    newTailpatch.fContext = context;
    newTailpatch.fCount = 1;
    newTailpatch.fTailpatch = tp;

    gInstalledTailpatches.push_back(newTailpatch);

    InstallInstructionBreaks();
}

/***********************************************************************
 *
 * FUNCTION:	EmPatchMgr::RecoverFromTailpatch
 *
 * DESCRIPTION:	.
 *
 * PARAMETERS:	none
 *
 * RETURNED:	nothing
 *
 ***********************************************************************/

TailpatchProc EmPatchMgr::RecoverFromTailpatch(emuptr startPC) {
    // Get the current PC so that we can find the record for this tailpatch.

    emuptr patchPC = startPC;

    // Find the PC.

    TailPatchIndex::iterator iter = gInstalledTailpatches.begin();

    while (iter != gInstalledTailpatches.end()) {
        if (iter->fContext.fNextPC == patchPC) {
            TailpatchProc result = iter->fTailpatch;

            // Decrement the use-count.  If it reaches zero, remove the
            // patch from our list.

            if (--(iter->fCount) == 0) {
                EmAssert(gSession);
                RemoveInstructionBreaks();

                gInstalledTailpatches.erase(iter);

                InstallInstructionBreaks();
            }

            return result;
        }

        ++iter;
    }

    return NULL;
}

/***********************************************************************
 *
 * FUNCTION:	EmPatchMgr::CallHeadpatch
 *
 * DESCRIPTION:	If the given system function is head patched, then call
 *				the headpatch.	Return "handled" (which means whether
 *				or not to call the ROM function after this one).
 *
 * PARAMETERS:	none
 *
 * RETURNED:	nothing
 *
 ***********************************************************************/

CallROMType EmPatchMgr::CallHeadpatch(HeadpatchProc hp) {
    EmValueChanger<bool> trackExecution(executingPatch, true);

    CallROMType handled = kExecuteROM;

    if (hp) handled = hp();

    return handled;
}

/***********************************************************************
 *
 * FUNCTION:	EmPatchMgr::CallTailpatch
 *
 * DESCRIPTION:	If the given function is tail patched, then call the
 *				tailpatch.
 *
 * PARAMETERS:	none
 *
 * RETURNED:	nothing
 *
 ***********************************************************************/

void EmPatchMgr::CallTailpatch(TailpatchProc tp) {
    EmValueChanger<bool> trackExecution(executingPatch, true);

    if (tp) tp();
}

/***********************************************************************
 *
 * FUNCTION:	EmPatchMgr::PuppetString
 *
 * DESCRIPTION:	Puppet stringing function for inserting events into
 *				the system.  We want to insert events when:
 *
 *				- Gremlins is running
 *				- The user types characters
 *				- The user clicks with the mouse
 *				- We need to trigger a switch another application
 *
 *				This function is called from headpatches to
 *				SysEvGroupWait and SysSemaphoreWait.  When the Palm OS
 *				needs an event, it calls EvtGetEvent.  EvtGetEvent
 *				looks in all the usual places for events to return. If
 *				it doesn't find any, it puts the system to sleep by
 *				calling SysEvGroupWait (or SysSemaphoreWait on 1.0
 *				systems).  SysEvGroupWait will wake up and return when
 *				an event is posted via something like EvtEnqueuePenPoint,
 *				EvtEnqueueKey, or KeyHandleInterrupt.
 *
 *				To puppet-string Palm OS, we therefore patch those
 *				functions and post events, preventing them from actually
 *				going to sleep.
 *
 * PARAMETERS:	callROM - return here whether or not the original ROM
 *					function still needs to be called.	Normally, the
 *					answer is "yes".
 *
 *				clearTimeout - set to true if the "timeout" parameter
 *					of the function we've patched needs to be prevented
 *					from being "infinite".
 *
 * RETURNED:	nothing
 *
 ***********************************************************************/

static void PrvForceNilEvent(void) {
    // No event was posted.  What we'd like right now is to force
    // EvtGetEvent to return a nil event.  We can do that by returning
    // a non-zero result code from SysEvGroupWait.  EvtGetEvent doesn't
    // look too closely at the result, but let's try to be as close to
    // reality as possible. SysEvGroupWait currently returns "4"
    // (CJ_WRTMOUT) to indicate a timeout condition.  It should
    // probably get turned into sysErrTimeout somewhere along the way,
    // but that translation doesn't seem to occur.

    m68k_dreg(gRegs, 0) = 4;
}

void EmPatchMgr::PuppetString(CallROMType& callROM) {
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
            ::PrvForceNilEvent();
            callROM = kSkipROM;
            return;
        }

        EmAssert(gSession);

        if (gSession->HasKeyboardEvent()) {
            KeyboardEvent evt = gSession->NextKeyboardEvent();

            if (evt.GetKey() != 0) EvtEnqueueKey(evt.GetKey(), 0, 0);
        } else if (gSession->HasPenEvent()) {
            PenEvent evt = gSession->NextPenEvent();
            PointType point;

            if (evt.isPenDown()) {
                point.x = evt.getX();
                point.y = evt.getY();

                PenScreenToRaw(&point);
            } else {
                point.x = point.y = -1;
            }

            EvtEnqueuePenPoint(&point);
        }
    }
}

/***********************************************************************
 *
 * FUNCTION:	EmPatchMgr::IntlMgrAvailable
 *
 * DESCRIPTION:	Indicates whether the international manager is available.
 *
 * PARAMETERS:
 *
 * RETURNED:	True if it is (OS > 4.0), false otherwise.
 *
 ***********************************************************************/

Bool EmPatchMgr::IntlMgrAvailable(void) {
#if 0  // CSTODO
    UInt32 romVersionData;
    FtrGet(sysFileCSystem, sysFtrNumROMVersion, &romVersionData);
    UInt32 romVersionMajor = sysGetROMVerMajor(romVersionData);

    return (romVersionMajor >= 4);
#endif
    return false;
}

bool EmPatchMgr::IsExecutingPatch() { return executingPatch; }
