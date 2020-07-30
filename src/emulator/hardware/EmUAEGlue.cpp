/* -*- mode: C++; tab-width: 4 -*- */
/* ===================================================================== *\
        Copyright (c) 1999-2001 Palm, Inc. or its subsidiaries.
        All rights reserved.

        This file is part of the Palm OS Emulator.

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.
\* ===================================================================== */

#include "EmUAEGlue.h"

#include "EmCPU68K.h"  // ProcessException, etc.
#include "EmCommon.h"
#include "EmSession.h"  // gSession->Reset
#include "Platform.h"   // AllocateMemory
#include "UAE.h"        // MakeSR, uae_s32, etc.

// ---------------------------------------------------------------------------
// Glue functions to bridge from UAE-generated code to the implemenations
// we define.
// ---------------------------------------------------------------------------

void customreset(void) {
    EmAssert(gSession);

    gSession->ScheduleReset(EmSession::ResetType::soft);
}

void Exception(int nr, emuptr /*oldpc*/) {
    EmAssert(gCPU68K);

    gCPU68K->ProcessException((ExceptionNumber)nr);
}

unsigned long op_illg(uint32 iOpcode) {
    EmAssert(gCPU68K);

    gCPU68K->ProcessIllegalInstruction(iOpcode);
    return 0;
}

void Software_ProcessLINK(int32 linkSize) {
    EmAssert(gCPU68K);

    gCPU68K->ProcessLINK(linkSize);
}

int Software_ProcessRTS(emuptr dest) {
    EmAssert(gCPU68K);

    gCPU68K->CheckNewPC(dest);
    return gCPU68K->ProcessRTS(dest);
}

int Software_ProcessRTE(emuptr dest) {
    EmAssert(gCPU68K);

    gCPU68K->CheckNewPC(dest);
    return gCPU68K->ProcessRTE(dest);
}

int Software_ProcessJSR(emuptr oldpc, emuptr dest) {
    EmAssert(gCPU68K);

    gCPU68K->CheckNewPC(dest);
    return gCPU68K->ProcessJSR(oldpc, dest);
}

int Software_ProcessJSR_Ind(emuptr oldpc, emuptr dest) {
    EmAssert(gCPU68K);

    return gCPU68K->ProcessJSR_Ind(oldpc, dest);
}

void Software_CheckNewPC(emuptr dest) {
    EmAssert(gCPU68K);

    gCPU68K->CheckNewPC(dest);
}

void Software_CheckStackPointerAssignment() {
    EmAssert(gCPU68K);

    gCPU68K->CheckNewSP(kStackPointerChanged);
}

void Software_CheckStackPointerDecrement() {
    EmAssert(gCPU68K);

    gCPU68K->CheckNewSP(kStackPointerDecremented);
}

void Software_CheckStackPointerIncrement() {
    EmAssert(gCPU68K);

    gCPU68K->CheckNewSP(kStackPointerIncremented);
}

void Software_CheckKernelStack() {
    EmAssert(gCPU68K);

    gCPU68K->CheckNewSP(kStackPointerKernelStackHack);
}

void MakeSR(void) {
    EmAssert(gCPU68K);

    gCPU68K->UpdateSRFromRegisters();
}

void MakeFromSR(void) {
    EmAssert(gCPU68K);

    gCPU68K->UpdateRegistersFromSR();
}

void* xmalloc(size_t size) { return Platform::AllocateMemory(size); }

uint32 get_disp_ea_000(uint32 base, uint32 dp) {
    int reg = (dp >> 12) & 15;
    uae_s32 regd = regs.regs[reg];

    if ((dp & 0x800) == 0) regd = (uae_s32)(uae_s16)regd;

    return base + (uae_s8)dp + regd;
}
