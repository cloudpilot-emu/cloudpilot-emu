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

#ifndef EmPalmFunction_h
#define EmPalmFunction_h

#include "EmCommon.h"

// Inline function to turn a trap word (0xA###) into an index into the
// trap table.  The method used here (masking off the uppermost nybble
// instead of, say, subtracting sysTrapBase) matches the ROM.

inline uint16 SysTrapIndex(uint16 trapWord) { return (uint16)(trapWord & ~0xF000); }

inline uint16 LibTrapIndex(uint16 trapWord) {
    return (uint16)(SysTrapIndex(trapWord) - SysTrapIndex(sysLibTrapBase));
}

inline Bool IsSystemTrap(uint16 trapWord) {
    return SysTrapIndex(trapWord) < SysTrapIndex(sysLibTrapBase);
}

inline Bool IsLibraryTrap(uint16 trapWord) { return !IsSystemTrap(trapWord); }

#if 0                   // CSDUBIOUS
    #include "EmCPU.h"  // GetPC

struct SystemCallContext;

void	EmPalmFunctionInit					(void);

    #define FOR_EACH_FUNCTION(DO_TO_FUNCTION)            \
        DO_TO_FUNCTION(cj_kptkdelete)                    \
        DO_TO_FUNCTION(dns_decode_name)                  \
        DO_TO_FUNCTION(BackspaceChar)                    \
        DO_TO_FUNCTION(Crc16CalcBlock)                   \
        DO_TO_FUNCTION(DmWrite)                          \
        DO_TO_FUNCTION(ECValidateField)                  \
        DO_TO_FUNCTION(EggOfInfiniteWisdom)              \
        DO_TO_FUNCTION(ExpInit)                          \
        DO_TO_FUNCTION(FindShowResults)                  \
        DO_TO_FUNCTION(FindSaveFindStr)                  \
        DO_TO_FUNCTION(FldDelete)                        \
        DO_TO_FUNCTION(FntDefineFont)                    \
        DO_TO_FUNCTION(GrfProcessStroke)                 \
        DO_TO_FUNCTION(HsPrvInit)                        \
        DO_TO_FUNCTION(HsPrvInitCard)                    \
        DO_TO_FUNCTION(MemMove)                          \
        DO_TO_FUNCTION(MenuHandleEvent)                  \
        DO_TO_FUNCTION(NetLibBitMove)                    \
        DO_TO_FUNCTION(NetPrvSettingSet)                 \
        DO_TO_FUNCTION(NetPrvTaskMain)                   \
        DO_TO_FUNCTION(PrvCompressedInnerBitBlt)         \
        DO_TO_FUNCTION(PrvConvertDepth1To2BW)            \
        DO_TO_FUNCTION(PrvDrawSliderControl)             \
        DO_TO_FUNCTION(PrvFindMemoryLeaks)               \
        DO_TO_FUNCTION(PrvGetExpGlobals)                 \
        DO_TO_FUNCTION(PrvGetVFSGlobals)                 \
        DO_TO_FUNCTION(PrvGetIntlMgrGlobalsP)            \
        DO_TO_FUNCTION(PrvSetIntlMgrGlobalsP)            \
        DO_TO_FUNCTION(PrvMisAlignedForwardInnerBitBlt)  \
        DO_TO_FUNCTION(PrvMisAlignedBackwardInnerBitBlt) \
        DO_TO_FUNCTION(PrvSystemTimerProc)               \
        DO_TO_FUNCTION(PrvReleaseExpGlobals)             \
        DO_TO_FUNCTION(PrvReleaseVFSGlobals)             \
        DO_TO_FUNCTION(SecPrvRandomSeed)                 \
        DO_TO_FUNCTION(SysAppExit)                       \
        DO_TO_FUNCTION(SysLaunch)                        \
        DO_TO_FUNCTION(TsmGlueGetFepGlobals)             \
        DO_TO_FUNCTION(VFSInit)                          \
        DO_TO_FUNCTION(_CerticomMemCpy)			\


// Declare a bunch of InFoo(emuptr) functions that can be used
// to determine if a memory location is within a particular
// Palm OS function.
//
// Note that the following macro used to declare just one function
// that took a default emuptr parameter set to gCPU->GetPC ().
// However, gcc 2.95.x and 2.96.x ran into internal compiler errors
// trying to compile it (egcs 1.1.x and gcc 3.0 appear to be OK).
// So the single function was broken into two, avoiding the error.

    #ifdef BROKEN_VIRTUAL_DEFAULT_ARGUMENTS

        #define DECLARE_FUNCTION(fn_name) \
            Bool In##fn_name(emuptr);     \
            inline Bool In##fn_name() { return In##fn_name(gCPU->GetPC()); }

    #else

        #define DECLARE_FUNCTION(fn_name) Bool In##fn_name(emuptr = gCPU->GetPC());

    #endif


FOR_EACH_FUNCTION(DECLARE_FUNCTION)

    #undef DECLARE_FUNCTION

    #define kProscribedDocumentedSystemUseOnly 1
    #define kProscribedUndocumentedSystemUseOnly 2
    #define kProscribedKernelUseOnly 3
    #define kProscribedObsolete 4
    #define kProscribedGhost 5
    #define kProscribedSystemUseOnlyAnyway 6
    #define kProscribedRare 7

Bool	ProscribedFunction		(const SystemCallContext& context);
int		GetProscribedReason		(const SystemCallContext& context);

emuptr	GetFunctionAddress		(uint16 trapWord, uint32 extra = sysInvalidRefNum, Bool digDeep = false);
emuptr	GetLibFunctionAddress	(uint16 trapWord, UInt16 refNum, Bool digDeep);
emuptr	GetSysFunctionAddress	(uint16 trapWord, uint32 extra, Bool digDeep);
emuptr	GetStdDispatchAddress	(emuptr entryPt, uint32 regD2);
emuptr	GetFlpEmDispatchAddress	(emuptr entryPt, uint32 regD2);
emuptr	GetIntlDispatchAddress	(emuptr entryPt, uint32 regD2);


class EmUnimplementedFunctionException
{
	public:
		EmUnimplementedFunctionException (long n = 0) :
			fLibIndex (n)
		{
		}

		long	fLibIndex;
};

class EmInvalidRefNumException
{
	public:
		EmInvalidRefNumException (long n = 0, long m = sysInvalidRefNum) :
			fLibIndex (n),
			fMaxRefNum (m)
		{
		}

		long	fLibIndex;
		long	fMaxRefNum;
};

char*	GetTrapName				(const SystemCallContext&, Bool digDeep = false);
char*	GetTrapName				(uint16 trapWord, uint32 extra = sysInvalidRefNum, Bool digDeep = false);
void	FindTrapName			(uint16 trapWord, char* nameP, uint32 extra = sysInvalidRefNum, Bool digDeep = false);

// Llamagraphics, Inc:  Added nameCapacity argument so that callers
// can retrieve more than 31 character function names.  The default
// capacity is 32, so callers that don't provide the extra argument
// will get the same result as before.

void	FindFunctionName		(emuptr addr,
								 char* nameP,
								 emuptr* startAddrP = NULL,
								 emuptr* endAddrP = NULL,
								 long nameCapacity = 32);
emuptr	FindFunctionStart		(emuptr addr);
emuptr	FindFunctionEnd			(emuptr addr);
Bool	EndOfFunctionSequence	(emuptr addr);

// Llamagraphics, Inc:  Added nameCapacity argument so that callers
// can retrieve more than 31 character function names.  The default
// capacity is 32, so callers that don't provide the extra argument
// will get the same result as before.

void	GetMacsbugInfo			(emuptr eof, char* name, long nameCapacity, emuptr* sof);
void	MacsbugNameLength		(emuptr addr, uint8* length, Bool* isFixed, emuptr* namePtr);
Bool	ValidMacsbugChar		(uint8 ch);

#endif

#endif /* EmPalmFunction_h */
