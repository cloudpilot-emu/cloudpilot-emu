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

#ifndef EmPalmOS_h
#define EmPalmOS_h

#include "EmCPU68K.h"  // ExceptionNumber
#include "EmPatchModuleTypes.h"
#include "EmStructs.h"  // EmStackFrameList
#include "EmThreadSafeQueue.h"
#include "KeyboardEvent.h"
#include "PenEvent.h"

class EmPalmOS {
   public:
    static void Initialize(void);
    static void Reset(void);
    static void Dispose(void);

    static void QueuePenEvent(PenEvent evt);
    static void QueueKeyboardEvent(KeyboardEvent evt);

    static bool HasPenEvent();
    static bool HasKeyboardEvent();
    static PenEvent PeekPenEvent();

    static void InjectSystemEvent(CallROMType& callROM);
    static void InjectUIEvent();

    static bool LaunchAppByName(const string& name);
    static bool HasPendingAppForLaunch();

   protected:
    static bool HandleTrap15(ExceptionNumber);
    static bool HandleJSR_Ind(emuptr oldPC, emuptr dest);

   private:
    static bool HandleSystemCall(Bool fromTrap);

    static bool DispatchNextEvent();
    static bool DispatchKeyboardEvent();
    static bool DispatchPenEvent();
    static void Wakeup();

    static void ClearQueues();

    static EmThreadSafeQueue<PenEvent> penEventQueue;
    static EmThreadSafeQueue<KeyboardEvent> keyboardEventQueue;

    static EmThreadSafeQueue<PenEvent> penEventQueueIncoming;
    static EmThreadSafeQueue<KeyboardEvent> keyboardEventQueueIncoming;
    static uint64 lastEventPromotedAt;

    static LocalID dbForLaunch;
    static bool postNilEvent;
};

#endif /* EmPalmOS_h */
