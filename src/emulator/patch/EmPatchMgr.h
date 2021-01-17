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

#ifndef EmPatchMgr_h
#define EmPatchMgr_h

#include <vector>

#include "EmPatchModuleTypes.h"

struct SystemCallContext;
class EmPatchModule;
class SavestateLoader;

class EmPatchMgr {
   public:
    static void Initialize(void);
    static void Reset(void);
    static void Dispose(void);

    template <typename T>
    static void Save(T& savestate);

    static void Load(SavestateLoader& loader);

    static void PostLoad(void);

    static CallROMType HandleSystemCall(const SystemCallContext&);

    static void HandleInstructionBreak(void);
    static void InstallInstructionBreaks(void);
    static void RemoveInstructionBreaks(void);

    static void GetPatches(const SystemCallContext&, HeadpatchProc& hp, TailpatchProc& tp);

    static CallROMType HandlePatches(const SystemCallContext&, HeadpatchProc hp, TailpatchProc tp);

    static EmPatchModule* GetLibPatchTable(uint16 refNum);

    static CallROMType CallHeadpatch(HeadpatchProc);
    static void CallTailpatch(TailpatchProc);

    static void PuppetString(CallROMType& callROM);

    static Bool IntlMgrAvailable(void);

   private:
    static void SetupForTailpatch(TailpatchProc tp, const SystemCallContext&);
    static TailpatchProc RecoverFromTailpatch(emuptr oldpc);

    static EmPatchModule* patchModuleSys;
    static EmPatchModule* patchModuleHtal;

   private:
};

#endif /* EmPatchMgr_h */
