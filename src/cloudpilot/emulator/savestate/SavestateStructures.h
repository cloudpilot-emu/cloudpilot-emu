#ifndef _SAVESTATE_STRUCTURES_H_
#define _SAVESTATE_STRUCTURES_H_

#include "Byteswapping.h"
#include "ChunkHelper.h"
#include "EmCommon.h"
#include "UAE.h"

template <typename T>
void DoSaveLoad(T& helper, regstruct& regs);

template <typename T>
void DoSaveLoad(SaveChunkHelper<T>& helper, HwrM68EZ328Type& regs);

template <typename T>
inline void DoSaveLoad(LoadChunkHelper<T>& helper, HwrM68EZ328Type& regs);

///////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
///////////////////////////////////////////////////////////////////////////////

template <typename T>
void DoSaveLoad(T& helper, regstruct& regs) {
    for (uint8 i = 0; i < 16; i++) helper.Do32(regs.regs[i]);

    helper.Do32(regs.usp)
        .Do32(regs.isp)
        .Do32(regs.msp)
        .Do16(regs.sr)
        .Do(typename T::Pack8() << regs.t1 << regs.t0)
        .Do(typename T::Pack8() << regs.s << regs.m << regs.x << regs.stopped)
        .Do32(regs.intmask)
        .Do32(regs.pc)
        .Do32(regs.vbr)
        .Do32(regs.sfc)
        .Do32(regs.dfc);

    for (uint8 i = 0; i < 8; i++) helper.DoDouble(regs.fp[i]);

    helper.Do32(regs.fpcr)
        .Do32(regs.fpsr)
        .Do32(regs.fpiar)
        .Do32(regs.spcflags)
        .Do32(regs.kick_mask)
        .Do32(regs.prefetch);
}

template <typename T>
void DoSaveLoad(SaveChunkHelper<T>& helper, HwrM68EZ328Type& regs) {
#if (EM_HOST_BYTE_ORDER == EM_BIG_ENDIAN)
    HwrM68EZ328Type regsCopy = regs;
    Byteswap(regs);

    helper.DoBuffer(static_cast<void*>(&regsCopy), sizeof(regsCopy));
#else
    helper.DoBuffer(static_cast<void*>(&regs), sizeof(regs));
#endif
}

template <typename T>
void DoSaveLoad(LoadChunkHelper<T>& helper, HwrM68EZ328Type& regs) {
    helper.DoBuffer(static_cast<void*>(&regs), sizeof(regs));

#if (EM_HOST_BYTE_ORDER == EM_BIG_ENDIAN)
    Byteswap(regs);
#endif
}

template <typename T>
void DoSaveLoad(SaveChunkHelper<T>& helper, HwrM68VZ328Type& regs) {
#if (EM_HOST_BYTE_ORDER == EM_BIG_ENDIAN)
    HwrM68EV328Type regsCopy = regs;
    Byteswap(regs);

    helper.DoBuffer(static_cast<void*>(&regsCopy), sizeof(regsCopy));
#else
    helper.DoBuffer(static_cast<void*>(&regs), sizeof(regs));
#endif
}

template <typename T>
void DoSaveLoad(SaveChunkHelper<T>& helper, HwrM68328Type& regs) {
#if (EM_HOST_BYTE_ORDER == EM_BIG_ENDIAN)
    HwrM68EV328Type regsCopy = regs;
    Byteswap(regs);

    helper.DoBuffer(static_cast<void*>(&regsCopy), sizeof(regsCopy));
#else
    helper.DoBuffer(static_cast<void*>(&regs), sizeof(regs));
#endif
}

template <typename T>
void DoSaveLoad(LoadChunkHelper<T>& helper, HwrM68VZ328Type& regs) {
    helper.DoBuffer(static_cast<void*>(&regs), sizeof(regs));

#if (EM_HOST_BYTE_ORDER == EM_BIG_ENDIAN)
    Byteswap(regs);
#endif
}

template <typename T>
void DoSaveLoad(LoadChunkHelper<T>& helper, HwrM68328Type& regs) {
    helper.DoBuffer(static_cast<void*>(&regs), sizeof(regs));

#if (EM_HOST_BYTE_ORDER == EM_BIG_ENDIAN)
    Byteswap(regs);
#endif
}

// The SZ register file has a huge gap between 0x0082c and 0x1f000 --- excluding it
// saves > 120k of space.

template <typename T>
void DoSaveLoad(SaveChunkHelper<T>& helper, HwrM68SZ328Type& regs, bool includeClut) {
#if (EM_HOST_BYTE_ORDER == EM_BIG_ENDIAN)
    HwrM68EV328Type regsCopy = regs;
    Byteswap(regsCopy);

    uint8* registerFile = reinterpret_cast<uint8*>(&regsCopy);

    helper.DoBuffer(registerFile, 0x082c);
    helper.DoBuffer(registerFile + 0x1f000, sizeof(HwrM68SZ328Type - 0x1f000));
#else
    uint8* registerFile = reinterpret_cast<uint8*>(&regs);

    helper.DoBuffer(registerFile, 0x082c)
        .DoBuffer(registerFile + 0x1f000, sizeof(HwrM68SZ328Type) - 0x1f000);

    if (includeClut) {
        helper.DoBuffer(registerFile + 0x0a00, 512);
    }
#endif
}

template <typename T>
void DoSaveLoad(LoadChunkHelper<T>& helper, HwrM68SZ328Type& regs, bool includeClut) {
    uint8* registerFile = reinterpret_cast<uint8*>(&regs);

    helper.DoBuffer(registerFile, 0x082c)
        .DoBuffer(registerFile + 0x1f000, sizeof(HwrM68SZ328Type) - 0x1f000);

    if (includeClut) {
        helper.DoBuffer(registerFile + 0x0a00, 512);
    }

#if (EM_HOST_BYTE_ORDER == EM_BIG_ENDIAN)
    Byteswap(regs);
#endif
}

#endif  // _SAVESTATE_STRUCTURES_H_
