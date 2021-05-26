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

#ifndef EMFILEIMPORT_H
#define EMFILEIMPORT_H

#include <cstddef>

#include "EmErrCodes.h"
#include "EmTypes.h"  // ErrCode

enum EmFileImportMethod { kMethodBest, kMethodHomebrew, kMethodExgMgr };

class EmFileImport {
   public:
    EmFileImport(const uint8* buffer, size_t len, EmFileImportMethod method);
    ~EmFileImport(void);

    static ErrCode LoadPalmFile(const uint8*, size_t, EmFileImportMethod);

    static ErrCode InstallExgMgrLib(void);
    static Bool CanUseExgMgr(void);

    // Interface for the dialog to display its stuff

    ErrCode Continue(void);
    Bool Done(void);

   private:
    void SetResult(Err);
    void SetResult(ErrCode);
    void SetDone(void);

    void IncrementalInstall(void);

    void ExgMgrInstallStart(void);
    void ExgMgrInstallMiddle(void);
    void ExgMgrInstallEnd(void);
    void ExgMgrInstallCancel(void);

    void HomeBrewInstallStart(void);
    void HomeBrewInstallMiddle(void);
    void HomeBrewInstallEnd(void);
    void HomeBrewInstallCancel(void);

    void ValidateStream(void);
    void DeleteCurrentDatabase(void);

   private:
    Bool fUsingExgMgr{false};
    long fState;
    ErrCode fError{errNone};

    // Fields for homebrew installer
    void* fFileBuffer{nullptr};
    size_t fFileBufferSize{0};
    LocalID fDBID{0};
    UInt16 fCardNo{0};
    emuptr fOpenID{0};
    long fCurrentEntry{0};
};

#endif /* EMFILEIMPORT_H */
