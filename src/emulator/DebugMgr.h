/* -*- mode: C++; tab-width: 4 -*- */
/* ===================================================================== *\
        Copyright (c) 1998-2001 Palm, Inc. or its subsidiaries.
        All rights reserved.

        This file is part of the Palm OS Emulator.

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.
\* ===================================================================== */

#ifndef _DEBUGMGR_H_
#define _DEBUGMGR_H_

#include "EmCPU68K.h"  // ExceptionNumber
#include "EmMemory.h"  // CEnableFullAccess
#include "EmTypes.h"   // ErrCode

// Types

#pragma mark Types

class SLP;
class CSocket;
class CTCPSocket;

struct SystemCallContext;

typedef uint32 (*registerFun)(int num);
typedef Bool (*compareFun)(UInt32 a, UInt32 b);

// Breakpoint conditions are of the form:
//
//		<register-expr>[<size>] <cond> <value>
//
// where:
//
//		<register-expr>
//			is either a 68000 register (e.g. "d5") or an indirect reference at
//			a constant offset from a 68000 register (e.g. "8(a6)").  An indirect
//			reference reads from memory; when you set up a breakpoint condition
//			involving an indirect reference, you must be sure that the indirect
//			reference will point to a valid memory address whenever the breakpoint
//			is hit.
//
//		<size>
//			is either ".l" (long), ".w" (word) or ".b" (byte).  If no size is
//			specified, the expression is assumed to be long.
//
//		<cond>
//			is a binary comparison operator: ==, !=, <=, >=, <, or >
//
//		<value>
//			is a 32-bit integer
//
// All comparisons are unsigned!

struct BreakpointCondition {
    registerFun regType;
    int regNum;
    Bool indirect;
    uint32 indirectOffset;
    int size; /* number of bytes to compare: 4, 2, or 1 */

    compareFun condition;
    uint32 value;

    // The source text.  We keep this around so that, for example, a user can specify
    // a condition using either hex or decimal notation and they will see the same
    // notation the next time they edit the breakpoint.

    char* source;

    Bool Evaluate(void);
};

struct EmBreakpointType {
    MemPtr addr;        // address of breakpoint
    Boolean enabled;    // true if enabled
    Boolean installed;  // for alignment
};

struct DebugGlobalsType {
    // Mode settings

    bool ignoreDbgBreaks;    // if true, ignore DbgBreak's
    bool firstEntrance;      // true first time we enter debugger
    bool stepSpy;            // true if step spying.
    bool breakingOnATrap;    // true if there are A-Traps to check
    bool continueOverATrap;  // true if skipping over next system call
    bool continueOverBP;     // true if skipping over next breakpoint

    bool checkTrapWordOnExit;
    uint16 trapWord;
    uint16 refNum;

    // Breakpoints and saved opcodes behind each one

    EmBreakpointType bp[dbgTotalBreakpoints];
    BreakpointCondition* bpCondition[dbgTotalBreakpoints];  // condition, or NULL if none

    // Current trap breaks

    UInt16 trapBreak[dbgTotalTrapBreaks];
    UInt16 trapParam[dbgTotalTrapBreaks];

    // Step spy support

    emuptr ssAddr;   // address to step spy on
    UInt32 ssValue;  // saved value

    // Exception type

    int32 excType;  // why we entered debugger

    // (adam) Data breakpoint support.	This is similar to the step spy capability, but can monitor
    // an arbitrary range of addresses for writes, and doesn't require a saved value.

    bool watchEnabled;
    emuptr watchAddr;   // address to watch, or 0 if none
    UInt32 watchBytes;  // number of bytes to watch
};

// class Debug

// Referenced in SystemPacket.cpp
extern DebugGlobalsType gDebuggerGlobals;

extern emuptr gExceptionAddress;
extern int gExceptionSize;
extern Bool gExceptionForRead;

class Debug {
   public:
    static void CheckStepSpy(emuptr writeAddress, int writeBytes);
};

#endif /* _DEBUGMGR_H_ */
