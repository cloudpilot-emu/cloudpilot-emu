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

#ifndef EmCPU68K_h
#define EmCPU68K_h

#include <vector>  // vector

#include "EmCPU.h"  // EmCPU

#if REGISTER_HISTORY
    #include "UAE.h"  // regstruct
#else
struct regstruct;
#endif

const uint16 kATrapReturnTrapNum = 0x0C;

/*---------------------------------------------------------------------
 *	Register numbering for 68K. Each register must have a unique
 *	non-zero register number.
 *--------------------------------------------------------------------*/
enum EmCPU68KRegID {
    e68KRegID_Invalid = 0, /* Zero is an invalid register number */
    e68KRegID_D0 = 1,
    e68KRegID_D1 = 2,
    e68KRegID_D2 = 3,
    e68KRegID_D3 = 4,
    e68KRegID_D4 = 5,
    e68KRegID_D5 = 6,
    e68KRegID_D6 = 7,
    e68KRegID_D7 = 8,
    e68KRegID_A0 = 9,
    e68KRegID_A1 = 10,
    e68KRegID_A2 = 11,
    e68KRegID_A3 = 12,
    e68KRegID_A4 = 13,
    e68KRegID_A5 = 14,
    e68KRegID_A6 = 15,
    e68KRegID_USP = 16,
    e68KRegID_SSP = 17,
    e68KRegID_PC = 18,
    e68KRegID_SR = 19
};

enum ExceptionNumber {
    kException_BusErr = 2,        // 08 / 2
    kException_AddressErr,        // 0C / 3
    kException_IllegalInstr,      // 10 / 4
    kException_DivideByZero,      // 14 / 5
    kException_Chk,               // 18 / 6
    kException_Trap,              // 1C / 7
    kException_Privilege,         // 20 / 8
    kException_Trace,             // 24 / 9
    kException_ATrap,             // 28 / A
    kException_FTrap,             // 2C / B
    kException_Reseved12,         // 30 / C
    kException_Coproc,            // 34 / D
    kException_FormatErr,         // 38 / E
    kException_UninitializedInt,  // 3C / F

    kException_Reserved0,  // 40-5C / 10-17
    kException_Reserved1,
    kException_Reserved2,
    kException_Reserved3,
    kException_Reserved4,
    kException_Reserved5,
    kException_Reserved6,
    kException_Reserved7,

    kException_SpuriousInt,  // 60 / 18
    kException_AutoVec1,     // 64 / 19
    kException_AutoVec2,     // 68 / 1A
    kException_AutoVec3,     // 6C / 1B
    kException_AutoVec4,     // 70 / 1C
    kException_AutoVec5,     // 74 / 1D
    kException_AutoVec6,     // 78 / 1E
    kException_AutoVec7,     // 7C / 1F

    kException_Trap0,  // 80 - BC / 20 - 2F	// For soft breakpoints
    kException_Trap1,
    kException_Trap2,
    kException_Trap3,
    kException_Trap4,
    kException_Trap5,
    kException_Trap6,
    kException_Trap7,
    kException_Trap8,  // For compiled breakpoints
    kException_Trap9,
    kException_Trap10,
    kException_Trap11,
    kException_Trap12,
    kException_Trap13,  // ATrap returns (emulator convention)
    kException_Trap14,
    kException_Trap15,  // Trap dispatcher

    kException_Unassigned0,  // C0 - FC / 30 - 3F
    kException_Unassigned1,
    kException_Unassigned2,
    kException_Unassigned3,
    kException_Unassigned4,
    kException_Unassigned5,
    kException_Unassigned6,
    kException_Unassigned7,
    kException_Unassigned8,
    kException_Unassigned9,
    kException_Unassigned10,
    kException_Unassigned11,
    kException_Unassigned12,
    kException_Unassigned13,
    kException_Unassigned14,
    kException_Unassigned15,

    kException_LastException,

    kException_SoftBreak = kException_Trap0 + sysDbgBreakpointTrapNum,
    kException_HardBreak = kException_Trap0 + sysDbgTrapNum,
    kException_SysCall = kException_Trap0 + sysDispatchTrapNum,
    kException_ATrapReturn = kException_Trap0 + kATrapReturnTrapNum
};

/*
        Grr...this function doesn't appear to be "seen" by VC++.  Even in its
        presence, an expression involving an ExceptionNumber and an int
        results in the message "error C2440: 'initializing' : cannot convert
        from 'const int' to 'const enum ExceptionNumber'"

inline ExceptionNumber operator+ (ExceptionNumber left, int right)
{
        // NB: cast "left" to an integer, or this routine will recurse!
        return (ExceptionNumber) ((int) left + right);
}
*/

#if EXCEPTION_HISTORY

struct ExceptionHistoryType {
    const char* name;
    uint32 pc;
    uint32 sp;
};

#endif

// Using uint32 instead of uint16 generates slightly better code.
typedef uint32 EmOpcode68K;

enum EmStackChangeType {
    kStackPointerChanged,
    kStackPointerIncremented,
    kStackPointerDecremented,
    kStackPointerKernelStackHack
};

class EmCPU68K;
typedef Bool (*Hook68KException)(ExceptionNumber);
typedef Bool (*Hook68KJSR)(emuptr oldPC, emuptr dest);
typedef Bool (*Hook68KJSR_Ind)(emuptr oldPC, emuptr dest);
typedef void (*Hook68KLINK)(int linkSize);
typedef Bool (*Hook68KRTE)(emuptr dest);
typedef Bool (*Hook68KRTS)(emuptr dest);
typedef void (*Hook68KNewPC)(emuptr dest);
typedef void (*Hook68KNewSP)(EmStackChangeType);

typedef vector<Hook68KException> Hook68KExceptionList;
typedef vector<Hook68KJSR> Hook68KJSRList;
typedef vector<Hook68KJSR_Ind> Hook68KJSR_IndList;
typedef vector<Hook68KLINK> Hook68KLINKList;
typedef vector<Hook68KRTE> Hook68KRTEList;
typedef vector<Hook68KRTS> Hook68KRTSList;
typedef vector<Hook68KNewPC> Hook68KNewPCList;
typedef vector<Hook68KNewSP> Hook68KNewSPList;

class EmCPU68K;
extern EmCPU68K* gCPU68K;

// These variables should strictly be in a sub-system that implements
// the stack overflow checking, etc.  However, for performance reasons,
// we need to expose them to UAE (see the CHECK_STACK_POINTER_ASSIGNMENT,
// et al macros), so define them here.
//
// Similar comments for the CheckKernelStack function.

#if 0  // CSDUBIOUS
extern "C" uint32 gStackHigh;
extern "C" uint32 gStackLowWarn;
extern "C" uint32 gStackLow;
extern "C" uint32 gKernelStackOverflowed;
#endif

class EmCPU68K : public EmCPU {
   public:
    // -----------------------------------------------------------------------------
    // constructor / destructor
    // -----------------------------------------------------------------------------

    EmCPU68K(EmSession*);
    virtual ~EmCPU68K(void);

    // -----------------------------------------------------------------------------
    // public methods
    // -----------------------------------------------------------------------------

    // Standard sub-system methods:
    //		Reset:	Resets the state.  Called on hardware resets or on
    //				calls to SysReset.  Also called from constructor.
    //		Save:	Saves the state to the given file.
    //		Load:	Loads the state from the given file.  Can assume that
    //				Reset has been called first.

    virtual void Reset(Bool hardwareReset);
    virtual void Save(Savestate&);
    virtual void Save(SavestateProbe&);
    virtual void Load(SavestateLoader&);

    // Execute the main CPU loop until asked to stop.

    virtual uint32 Execute(uint32 maxCycles);
    virtual void CheckAfterCycle(void);

    // Low-level access to CPU state.

    virtual emuptr GetPC(void);
    virtual emuptr GetSP(void);
    virtual uint32 GetRegister(int);

    virtual void SetPC(emuptr);
    virtual void SetSP(emuptr);
    virtual void SetRegister(int, uint32);

    virtual Bool Stopped(void);

    // Called from routines in EmUAEGlue.cpp

    void ProcessException(ExceptionNumber);
    void ProcessIllegalInstruction(EmOpcode68K opcode);
    int ProcessJSR_Ind(emuptr oldPC, emuptr dest);

    // Called by CPU clients who need to be notified when certain
    // actions occur.

    void InstallHookException(ExceptionNumber, Hook68KException);

    void InstallHookJSR_Ind(Hook68KJSR_Ind);

    void RemoveHookException(ExceptionNumber, Hook68KException);

    void RemoveHookJSR_Ind(Hook68KJSR_Ind);

    // Register management.  Clients should call Get/SetRegisters for
    // the most part.  UpdateXFromY are here so that MakeSR and
    // MakeFromSR (UAE glue functions) can call them.

    void GetRegisters(regstruct& registers);
    void SetRegisters(regstruct& registers);
    void UpdateSRFromRegisters(void);
    void UpdateRegistersFromSR(void);

    void BusError(emuptr address, long size, Bool forRead);
    void AddressError(emuptr address, long size, Bool forRead);

   private:
    Bool ExecuteSpecial(uint32 maxCycles);
    Bool ExecuteStoppedLoop(uint32 maxCycles);

    void CycleSlowly(Bool sleeping);
    Bool CheckForBreak(void);

    void ProcessInterrupt(int32 interrupt);

    void InitializeUAETables(void);

    template <typename T>
    void DoSave(T& savestate);

    template <typename T>
    void DoSaveLoad(T& helper);

   private:
    emuptr fLastTraceAddress;
    Hook68KExceptionList fExceptionHandlers[kException_LastException];
    Hook68KJSR_IndList fHookJSR_Ind;
    uint32 fCurrentCycles{0};

#if REGISTER_HISTORY
    #define kRegHistorySize 512
    long fRegHistoryIndex;
    struct regstruct fRegHistory[kRegHistorySize];
#endif

#if EXCEPTION_HISTORY
    #define kExceptionHistorySize 512
    long fExceptionHistoryIndex;
    struct ExceptionHistoryType fExceptionHistory[kExceptionHistorySize];
#endif
};

#endif  // EmCPU68K_h
