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

#ifndef EmMemory_h
#define EmMemory_h

// Normally, I'd assume that these includes were pulled in
// by EmCommon.h.  However, EmMemory.h gets included by UAE,
// which doesn't pull in EmCommon.h.  So I have to explicitly
// make sure they're included.

#include "EmAssert.h"   // EmAssert
#include "EmTypes.h"    // uint32, etc.
#include "Switches.h"   // WORDSWAP_MEMORY, UNALIGNED_LONG_ACCESS
#include "sysconfig.h"  // STATIC_INLINE

#ifdef __cplusplus
extern "C" {
#endif

// ---------------------------------------------------------------------------
//		� EmAddressBank
// ---------------------------------------------------------------------------

typedef uint32 (*EmMemGetFunc)(emuptr);
typedef void (*EmMemPutFunc)(emuptr, uint32);
typedef uint8* (*EmMemTranslateFunc)(emuptr);
typedef int (*EmMemCheckFunc)(emuptr, uint32);
typedef void (*EmMemCycleFunc)(void);
typedef uint8* (*EmMemTranslateMetaFunc)(emuptr);

typedef struct EmAddressBank {
    /* These ones should be self-explanatory... */
    EmMemGetFunc lget, wget, bget;
    EmMemPutFunc lput, wput, bput;

    /* Use xlateaddr to translate an Amiga address to a uae_u8 * that can
     * be used to address memory without calling the wget/wput functions.
     * This doesn't work for all memory banks, so this function may call
     * abort(). */
    EmMemTranslateFunc xlateaddr;

    /* To prevent calls to abort(), use check before calling xlateaddr.
     * It checks not only that the memory bank can do xlateaddr, but also
     * that the pointer points to an area of at least the specified size.
     * This is used for example to translate bitplane pointers in custom.c */
    EmMemCheckFunc checkaddr;

    EmMemTranslateMetaFunc xlatemetaaddr;
    EmMemCycleFunc EmMemAddOpcodeCycles;
} EmAddressBank;

#ifndef ECM_DYNAMIC_PATCH

extern EmAddressBank* gEmMemBanks[65536];

#else  // ECM_DYNAMIC_PATCH

extern EmAddressBank** gDynEmMemBanksP;

#endif  // ECM_DYNAMIC_PATCH

// ---------------------------------------------------------------------------
//		� Support macros
// ---------------------------------------------------------------------------

#ifndef ECM_DYNAMIC_PATCH

    #define EmMemBankIndex(addr) (((emuptr)(addr)) >> 16)

    #define EmMemGetBank(addr) (*gEmMemBanks[EmMemBankIndex(addr)])
    #define EmMemPutBank(addr, b) (gEmMemBanks[EmMemBankIndex(addr)] = (b))

#else  // ECM_DYNAMIC_PATCH

    #define EmMemBankIndex(addr) (((unsigned long)(addr)) >> 16)

    #define EmMemGetBank(addr) (*((gDynEmMemBanksP)[EmMemBankIndex(addr)]))
    #define EmMemPutBank(addr, b) ((gDynEmMemBanksP)[EmMemBankIndex(addr)] = (b))

#endif  // ECM_DYNAMIC_PATCH

#define EmMemCallGetFunc(func, addr) ((*EmMemGetBank(addr).func)(addr))
#define EmMemCallPutFunc(func, addr, v) ((*EmMemGetBank(addr).func)(addr, v))

// ---------------------------------------------------------------------------
//		� EmMemGet32
// ---------------------------------------------------------------------------

STATIC_INLINE uint32 EmMemGet32(emuptr addr) { return EmMemCallGetFunc(lget, addr); }

// ---------------------------------------------------------------------------
//		� EmMemGet16
// ---------------------------------------------------------------------------

STATIC_INLINE uint32 EmMemGet16(emuptr addr) { return EmMemCallGetFunc(wget, addr); }

// ---------------------------------------------------------------------------
//		� EmMemGet8
// ---------------------------------------------------------------------------

STATIC_INLINE uint32 EmMemGet8(emuptr addr) { return EmMemCallGetFunc(bget, addr); }

// ---------------------------------------------------------------------------
//		� EmMemPut32
// ---------------------------------------------------------------------------

STATIC_INLINE void EmMemPut32(emuptr addr, uint32 l) { EmMemCallPutFunc(lput, addr, l); }

// ---------------------------------------------------------------------------
//		� EmMemPut16
// ---------------------------------------------------------------------------

STATIC_INLINE void EmMemPut16(emuptr addr, uint32 w) { EmMemCallPutFunc(wput, addr, w); }

// ---------------------------------------------------------------------------
//		� EmMemPut8
// ---------------------------------------------------------------------------

STATIC_INLINE void EmMemPut8(emuptr addr, uint32 b) { EmMemCallPutFunc(bput, addr, b); }

// ---------------------------------------------------------------------------
//		� EmMemGetRealAddress
// ---------------------------------------------------------------------------

STATIC_INLINE uint8* EmMemGetRealAddress(emuptr addr) { return EmMemGetBank(addr).xlateaddr(addr); }

// ---------------------------------------------------------------------------
//		� EmMemCheckAddress
// ---------------------------------------------------------------------------

STATIC_INLINE int EmMemCheckAddress(emuptr addr, uint32 size) {
    return EmMemGetBank(addr).checkaddr(addr, size);
}

// ---------------------------------------------------------------------------
//		� EmMemAddOpcodeCycles
// ---------------------------------------------------------------------------

STATIC_INLINE void EmMemAddOpcodeCycles(emuptr addr) {
    EmAssert(EmMemGetBank(addr).EmMemAddOpcodeCycles);
    EmMemGetBank(addr).EmMemAddOpcodeCycles();
}

// ---------------------------------------------------------------------------
//		� EmMemGetMetaAddress
// ---------------------------------------------------------------------------

STATIC_INLINE uint8* EmMemGetMetaAddress(emuptr addr) {
    EmAssert(EmMemGetBank(addr).xlatemetaaddr);
    return EmMemGetBank(addr).xlatemetaaddr(addr);
}

// ---------------------------------------------------------------------------
//		� EmMemDoGet32
// ---------------------------------------------------------------------------

STATIC_INLINE uint32 EmMemDoGet32(void* a) {
#if WORDSWAP_MEMORY || !UNALIGNED_LONG_ACCESS
    return (((uint32) * (((uint16*)a) + 0)) << 16) | (((uint32) * (((uint16*)a) + 1)));
#else
    return *(uint32*)a;
#endif
}

// ---------------------------------------------------------------------------
//		� EmMemDoGet16
// ---------------------------------------------------------------------------

STATIC_INLINE uint16 EmMemDoGet16(void* a) { return *(uint16*)a; }

// ---------------------------------------------------------------------------
//		� EmMemDoGet8
// ---------------------------------------------------------------------------

STATIC_INLINE uint8 EmMemDoGet8(void* a) {
#if WORDSWAP_MEMORY
    return *(uint8*)((long)a ^ 1);
#else
    return *(uint8*)a;
#endif
}

// ---------------------------------------------------------------------------
//		� EmMemDoPut32
// ---------------------------------------------------------------------------

STATIC_INLINE void EmMemDoPut32(void* a, uint32 v) {
#if WORDSWAP_MEMORY || !UNALIGNED_LONG_ACCESS
    *(((uint16*)a) + 0) = (uint16)(v >> 16);
    *(((uint16*)a) + 1) = (uint16)(v);
#else
    *(uint32*)a = v;
#endif
}

// ---------------------------------------------------------------------------
//		� EmMemDoPut16
// ---------------------------------------------------------------------------

STATIC_INLINE void EmMemDoPut16(void* a, uint16 v) { *(uint16*)a = v; }

// ---------------------------------------------------------------------------
//		� EmMemDoPut8
// ---------------------------------------------------------------------------

STATIC_INLINE void EmMemDoPut8(void* a, uint8 v) {
#if WORDSWAP_MEMORY
    *(uint8*)((long)a ^ 1) = v;
#else
    *(uint8*)a = v;
#endif
}

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

class EmStream;

// Types.

// This struct is used to control access to memory.  The first set of fields
// are booleans which, when set to true, turn on address validation for the
// various ranges of memory.  The second second set of fields are booleans
// which, when set to true, prevent access by user applications to the various
// ranges of memory.

struct MemAccessFlags {
    Bool fValidate_DummyGet;
    Bool fValidate_DummySet;
    Bool fValidate_RegisterGet;
    Bool fValidate_RegisterSet;
    Bool fValidate_DRAMGet;
    Bool fValidate_DRAMSet;
    Bool fValidate_SRAMGet;
    Bool fValidate_SRAMSet;
    Bool fValidate_ROMGet;
    Bool fValidate_ROMSet;

    //	Bool				fProtect_LowMemGet;
    //	Bool				fProtect_LowMemSet;
    //	Bool				fProtect_GlobalGet;
    //	Bool				fProtect_GlobalSet;
    //	Bool				fProtect_ScreenGet;
    //	Bool				fProtect_ScreenSet;
    Bool fProtect_SRAMGet;
    Bool fProtect_SRAMSet;
    Bool fProtect_ROMGet;
    Bool fProtect_ROMSet;
    Bool fProtect_RegisterGet;
    Bool fProtect_RegisterSet;

    //	Bool				fCheck_UserChunkGet;
    //	Bool				fCheck_UserChunkSet;
    //	Bool				fCheck_SysChunkGet;
    //	Bool				fCheck_SysChunkSet;

    //	Bool				fProtect_SysLowMemGet;
    //	Bool				fProtect_SysLowMemSet;
    //	Bool				fProtect_SysGlobalGet;
    //	Bool				fProtect_SysGlobalSet;
    //	Bool				fProtect_SysScreenGet;
    //	Bool				fProtect_SysScreenSet;
    //	Bool				fProtect_SysSRAMGet;
    //	Bool				fProtect_SysSRAMSet;
    Bool fProtect_SysROMGet;
    Bool fProtect_SysROMSet;
    //	Bool				fProtect_SysRegisterGet;
    //	Bool				fProtect_SysRegisterSet;
};

// Globals.

extern MemAccessFlags gMemAccessFlags;
extern Bool gPCInRAM;
extern Bool gPCInROM;

struct EmAddressBank;
class SessionFile;

// Function prototypes.

class Memory {
   public:
    static void Initialize(const uint8* romBuffer, size_t romSize, RAMSizeType ramSize);
    static void Reset(Bool hardwareReset);
    static void Save(SessionFile&);
    static void Load(SessionFile&);
    static void Dispose(void);

    static void InitializeBanks(EmAddressBank& iBankInitializer, int32 iStartingBankIndex,
                                int32 iNumberOfBanks);

    static void ResetBankHandlers(void);

    static void MapPhysicalMemory(const void*, uint32);
    static void UnmapPhysicalMemory(const void*);
    static void GetMappingInfo(emuptr, void**, uint32*);

    static void CheckNewPC(emuptr newPC);
    static int IsPCInRAM(void) { return gPCInRAM; }
    static int IsPCInROM(void) { return gPCInROM; }
};

typedef Memory EmMemory;

// There are places within the emulator where we'd like to access low-memory
// and/or Dragonball registers.  If the PC happens to be in RAM, then
// the checks implied by the above booleans and switches will flag our
// access as an error.  Before making such accesses, create an instance
// of CEnableFullAccess to suspend and restore the checks.
//
// Since such accesses are typically "meta" accesses where the emulator is
// accessing memory outside the normal execution of an opcode, we also
// turn off the profiling variable that controls whether or not cycles
// spent accessing memory are counted.

class CEnableFullAccess {
   public:
    CEnableFullAccess(void);
    ~CEnableFullAccess(void);

    static Bool AccessOK(void);

   private:
    MemAccessFlags fOldMemAccessFlags;

    static long fgAccessCount;
};

// Std C Library-ish routines for manipulating data
// in emulated memory.

emuptr EmMem_memset(emuptr dst, int val, size_t len);

emuptr EmMem_memchr(emuptr src, int val, size_t len);

template <class T1, class T2>
int EmMem_memcmp(T1 src1, T2 src2, size_t len);

template <class T1, class T2>
T1 EmMem_memcpy(T1 dst, T2 src, size_t len);

template <class T1, class T2>
T1 EmMem_memmove(T1 dst, T2 src, size_t len);

size_t EmMem_strlen(emuptr str);

template <class T1, class T2>
T1 EmMem_strcpy(T1 dst, T2 src);

template <class T1, class T2>
T1 EmMem_strncpy(T1 dst, T2 src, size_t len);

template <class T1, class T2>
T1 EmMem_strcat(T1 dst, T2 src);

template <class T1, class T2>
T1 EmMem_strncat(T1 dst, T2 src, size_t len);

template <class T1, class T2>
int EmMem_strcmp(T1 dst, T2 src);

template <class T1, class T2>
int EmMem_strncmp(T1 dst, T2 src, size_t len);

#endif  // __cplusplus

#endif /* EmMemory_h */
