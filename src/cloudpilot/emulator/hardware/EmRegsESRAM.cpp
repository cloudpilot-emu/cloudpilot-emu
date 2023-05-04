#include "EmRegsESRAM.h"

#include "EmCommon.h"
#include "EmMemory.h"
#include "EmSystemState.h"

namespace {
    constexpr uint32 esramSize = 1024 * 100;

    uint8* esram;
    uint8* dirtyPages;

    inline void markDirty(emuptr offset) {
        dirtyPages[offset >> 13] |= (1 << ((offset >> 10) & 0x07));
    }
}  // namespace

void EmRegsESRAM::Initialize() {
    esram = EmMemory::GetForRegion(MemoryRegion::eSRAM);
    dirtyPages = EmMemory::GetDirtyPagesForRegion(MemoryRegion::eSRAM);

    EmAssert(esram);
    EmAssert(dirtyPages);

    baseAddr = 0;
    isActive = false;
    isFramebuffer = false;
    framebufferBase = 0;

    EmRegs::Initialize();
    memset(esram, 0, esramSize);
}

void EmRegsESRAM::Reset(Bool hardwareReset) {
    if (hardwareReset) {
        Disable();
        framebufferBase = 0;
    }
}

void EmRegsESRAM::Enable(emuptr baseAddr) {
    this->baseAddr = baseAddr;
    isActive = true;

    isFramebuffer = framebufferBase >= baseAddr && (framebufferBase - baseAddr) < esramSize;
}

void EmRegsESRAM::Disable() { isActive = false; }

void EmRegsESRAM::SetFramebufferBase(emuptr framebufferBase) {
    this->framebufferBase = framebufferBase;

    isFramebuffer = framebufferBase >= baseAddr && (framebufferBase - baseAddr) < esramSize;
}

bool EmRegsESRAM::IsFramebuffer() const { return isActive && isFramebuffer; }

uint32 EmRegsESRAM::GetLong(emuptr address) {
    const uint32 offset = address - baseAddr;
    return EmMemDoGet32(esram + offset);
}

uint32 EmRegsESRAM::GetWord(emuptr address) {
    const uint32 offset = address - baseAddr;
    return EmMemDoGet16(esram + offset);
}

uint32 EmRegsESRAM::GetByte(emuptr address) {
    const uint32 offset = address - baseAddr;
    return EmMemDoGet8(esram + offset);
}

void EmRegsESRAM::SetLong(emuptr address, uint32 value) {
    uint32 offset = address - baseAddr;
    EmMemDoPut32(esram + offset, value);

    if (isFramebuffer) gSystemState.MarkScreenDirty(address, address + 4);

    markDirty(offset);
    markDirty(offset + 2);
}

void EmRegsESRAM::SetWord(emuptr address, uint32 value) {
    uint32 offset = address - baseAddr;
    EmMemDoPut16(esram + offset, value);

    if (isFramebuffer) gSystemState.MarkScreenDirty(address, address + 2);

    markDirty(offset);
}

void EmRegsESRAM::SetByte(emuptr address, uint32 value) {
    uint32 offset = address - baseAddr;
    EmMemDoPut8(esram + offset, value);

    if (isFramebuffer) gSystemState.MarkScreenDirty(address, address);

    markDirty(offset);
}

int EmRegsESRAM::ValidAddress(emuptr address, uint32 size) { return true; }

void EmRegsESRAM::SetSubBankHandlers() {}

uint8* EmRegsESRAM::GetRealAddress(emuptr address) {
    const uint32 offset = address - baseAddr;

    return esram + offset;
}

emuptr EmRegsESRAM::GetAddressStart() { return isActive ? baseAddr : 0; }

uint32 EmRegsESRAM::GetAddressRange() { return isActive ? esramSize : 0; }
