#include "Debugger.h"

#include "DebuggerMemoryBinding.h"
#include "EmCPU68K.h"
#include "EmHAL.h"
#include "EmMemory.h"
#include "EmSession.h"
#include "Miscellaneous.h"
#include "UAE.h"

namespace {
    template <typename T, typename K>
    bool setContains(const T& set, K key) {
        return set.find(key) != set.end();
    }
}  // namespace

Debugger gDebugger;
void* gDebuggerPtr = &gDebugger;

Debugger::BreakState Debugger::GetBreakState() const { return breakState; }

bool Debugger::IsStopped() const { return breakState != BreakState::none; }

void Debugger::Reset() {
    breakState = BreakState::none;
    stepping = false;
    lastBreakAtPc = 0xffffffff;
    enabled = false;

    breakpoints.clear();
    watchpointsRead.clear();
    watchpointsWrite.clear();
}

void Debugger::Enable() {
    enabled = true;

    romStart = EmHAL::GetROMBaseAddress();
    romSize = EmHAL::GetROMSize();
}

void Debugger::ResetBreakMode() { breakMode = BreakMode::all; }

void Debugger::ResetAppRegion() { appStart = appSize = 0; }

void Debugger::SetBreakMode(BreakMode mode) { breakMode = mode; }

void Debugger::SetAppRegion(emuptr start, uint32 size) {
    appStart = start;
    appSize = size;
}

Debugger::BreakMode Debugger::GetBreakMode() const { return breakMode; }

emuptr Debugger::GetAppStart() const { return appStart; }

uint32 Debugger::GetAppSize() const { return appSize; }

void Debugger::NotificyPc(emuptr pc) {
    if (!enabled) return;
    if (breakMode == BreakMode::appOnly && (pc < appStart || pc >= appStart + appSize)) return;
    if (breakMode == BreakMode::ramOnly && pc >= romStart && pc < romStart + romSize) return;

    EmAssert(gSession);
    if (gSession->IsNested() || breakState != BreakState::none) return;

    if (setContains(breakpoints, pc))
        Break(BreakState::breakpoint);
    else if (stepping && pc != lastBreakAtPc)
        Break(BreakState::step);
}

void Debugger::NotifyMemoryRead8(emuptr address) {
    if (!enabled || memoryAccess) return;

    if (setContains(watchpointsRead, address)) {
        Break(BreakState::trapRead);

        watchpointAddress = address;
    }
}

void Debugger::NotifyMemoryRead16(emuptr address) {
    if (!enabled || memoryAccess) return;

    if (setContains(watchpointsRead, address) || setContains(watchpointsRead, address + 1)) {
        Break(BreakState::trapRead);

        watchpointAddress = address;
    }
}

void Debugger::NotifyMemoryRead32(emuptr address) {
    if (!enabled || memoryAccess) return;

    if (setContains(watchpointsRead, address) || setContains(watchpointsRead, address + 1) ||
        setContains(watchpointsRead, address + 2) || setContains(watchpointsRead, address + 3)) {
        Break(BreakState::trapRead);

        watchpointAddress = address;
    }
}

void Debugger::NotifyMemoryWrite8(emuptr address) {
    if (!enabled || memoryAccess) return;

    if (setContains(watchpointsWrite, address)) {
        Break(BreakState::trapWrite);

        watchpointAddress = address;
    }
}

void Debugger::NotifyMemoryWrite16(emuptr address) {
    if (!enabled || memoryAccess) return;

    if (setContains(watchpointsWrite, address) || setContains(watchpointsWrite, address + 1)) {
        Break(BreakState::trapWrite);

        watchpointAddress = address;
    }
}

void Debugger::NotifyMemoryWrite32(emuptr address) {
    if (!enabled || memoryAccess) return;

    if (setContains(watchpointsWrite, address) || setContains(watchpointsWrite, address + 1) ||
        setContains(watchpointsWrite, address + 2) || setContains(watchpointsWrite, address + 3)) {
        Break(BreakState::trapWrite);

        watchpointAddress = address;
    }
}

void Debugger::SetBreakpoint(emuptr pc) { breakpoints.insert(pc); }

void Debugger::ClearBreakpoint(emuptr pc) { breakpoints.erase(pc); }

void Debugger::SetWatchpoint(emuptr address, WatchpointType type, size_t len) {
    for (size_t i = 0; i < len; i++) {
        switch (type) {
            case WatchpointType::read:
                watchpointsRead.insert(address + i);
                break;

            case WatchpointType::write:
                watchpointsWrite.insert(address + i);
                break;

            case WatchpointType::readwrite:
                watchpointsRead.insert(address + i);
                watchpointsWrite.insert(address + i);
                break;
        }
    }
}

void Debugger::ClearWatchpoint(emuptr address, WatchpointType type, size_t len) {
    for (size_t i = 0; i < len; i++) {
        switch (type) {
            case WatchpointType::read:
                watchpointsRead.erase(address + i);
                break;

            case WatchpointType::write:
                watchpointsWrite.erase(address + i);
                break;

            case WatchpointType::readwrite:
                watchpointsRead.erase(address + i);
                watchpointsWrite.erase(address + i);
                break;
        }
    }
}

Debugger::WatchpointType Debugger::GetWatchpointType() const {
    if (setContains(watchpointsRead, watchpointAddress))
        return setContains(watchpointsWrite, watchpointAddress) ? WatchpointType::readwrite
                                                                : WatchpointType::read;

    return WatchpointType::write;
}

emuptr Debugger::GetWatchpointAddress() const { return watchpointAddress; }

void Debugger::Interrupt() { Break(BreakState::externalInterrupt); }

void Debugger::Continue() {
    breakState = BreakState::none;
    stepping = false;
}

void Debugger::Step() {
    stepping = true;
    breakState = BreakState::none;
}

uint8 Debugger::MemoryRead8(emuptr addr) {
    CEnableFullAccess munge;
    EmValueChanger<bool> trackAccess(memoryAccess, true);

    return EmMemGet8(addr);
}

uint16 Debugger::MemoRead16(emuptr addr) {
    CEnableFullAccess munge;
    EmValueChanger<bool> trackAccess(memoryAccess, true);

    if (addr & 0x01)
        return (EmMemGet8(addr) << 8) | EmMemGet8(addr + 1);
    else
        return EmMemGet16(addr);
}

uint32 Debugger::MemoryRead32(emuptr addr) {
    CEnableFullAccess munge;
    EmValueChanger<bool> trackAccess(memoryAccess, true);

    if (addr & 0x01)
        return (EmMemGet8(addr) << 24) | (EmMemGet8(addr + 1) << 16) | (EmMemGet8(addr + 2) << 8) |
               EmMemGet8(addr + 3);
    else
        return EmMemGet32(addr);
}

void Debugger::MemoryWrite(emuptr addr, uint8* data, size_t len) {
    if (len == 2 && (addr & 0x01) == 0)
        EmMemPut16(addr, (data[0] << 8) | data[1]);
    else if (len == 4 && (addr & 0x01) == 0)
        EmMemPut32(addr, (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3]);
    else {
        for (size_t i = 0; i < len; i++) EmMemPut8(addr + i, data[i]);
    }
}

bool Debugger::IsMemoryAccess() const { return memoryAccess; }

void Debugger::UpdateBreakState() { lastBreakAtPc = regs.pc; }

const array<uint32, Debugger::REGISTER_COUNT>& Debugger::ReadRegisters() {
    // straight from the horse's mouth:
    //
    // static const char * const m68k_register_names[] = {
    //     "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7",
    //     "a0", "a1", "a2", "a3", "a4", "a5", "fp", "sp",
    //     "ps", "pc"

    EmAssert(gCPU68K);
    gCPU68K->UpdateSRFromRegisters();

    for (int i = 0; i < 8; i++) {
        registers[i] = m68k_dreg(regs, i);
        registers[8 + i] = m68k_areg(regs, i);
    }

    registers[16] = regs.sr;
    registers[17] = regs.pc;

    return registers;
}

void Debugger::SetRegister(size_t index, uint32 value) {
    if (index < 8)
        m68k_dreg(regs, index) = value;
    else if (index < 16)
        m68k_areg(regs, index - 8) = value;
    else if (index == 16) {
        regs.sr = value;

        EmAssert(gCPU68K);
        gCPU68K->UpdateRegistersFromSR();
    } else if (index == 17)
        regs.pc = value;
}

void Debugger::Break(BreakState state) {
    breakState = state;

    if (state != BreakState::none) {
        lastBreakAtPc = regs.pc;
    }
}

void DbgNotifyRead8(emuptr address) {
    reinterpret_cast<Debugger*>(gDebuggerPtr)->NotifyMemoryRead8(address);
}

void DbgNotifyRead16(emuptr address) {
    reinterpret_cast<Debugger*>(gDebuggerPtr)->NotifyMemoryRead16(address);
}

void DbgNotifyRead32(emuptr address) {
    reinterpret_cast<Debugger*>(gDebuggerPtr)->NotifyMemoryRead32(address);
}

void DbgNotifyWrite8(emuptr address) {
    reinterpret_cast<Debugger*>(gDebuggerPtr)->NotifyMemoryWrite8(address);
}

void DbgNotifyWrite16(emuptr address) {
    reinterpret_cast<Debugger*>(gDebuggerPtr)->NotifyMemoryWrite16(address);
}

void DbgNotifyWrite32(emuptr address) {
    reinterpret_cast<Debugger*>(gDebuggerPtr)->NotifyMemoryWrite32(address);
}
