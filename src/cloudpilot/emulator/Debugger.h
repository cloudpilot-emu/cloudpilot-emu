#ifndef _DEBUGGER_H_
#define _DEBUGGER_H_

#include <array>
#include <unordered_set>

#include "EmCommon.h"

class Debugger {
   public:
    static constexpr size_t REGISTER_COUNT = 18;

    enum class BreakState {
        none,
        breakpoint,
        step,
        trapRead,
        trapWrite,
        trapInternal,
        externalInterrupt
    };

   public:
    Debugger() = default;

    BreakState GetBreakState() const;
    bool IsStopped() const;

    void Reset();

    void NotificyPc(emuptr pc);
    void NotifyMemoryRead(emuptr address);
    void NotifyMemoryWrite(emuptr address);

    void SetBreakpoint(emuptr pc);
    void ClearBreakpoint(emuptr pc);

    void SetWatchpoint(emuptr address);
    void ClearWatchpoint(emuptr address);

    void Interrupt();
    void Continue();
    void Step();

    uint8 MemoryRead8(emuptr addr);
    uint16 MemoRead16(emuptr addr);
    uint32 MemoryRead32(emuptr addr);

    bool IsMemoryAccess() const;

    const array<uint32, REGISTER_COUNT>& ReadRegisters();

   private:
    BreakState breakState{BreakState::none};

    array<uint32, REGISTER_COUNT> registers;

    bool stepping{false};
    emuptr lastBreakAtPc{0xffffffff};

    bool memoryAccess{false};

    unordered_set<emuptr> breakpoints;

   private:
    Debugger(const Debugger&) = delete;
    Debugger(Debugger&&) = delete;
    Debugger& operator=(const Debugger&) = delete;
    Debugger& operator=(Debugger&&) = delete;
};

extern Debugger gDebugger;

#endif  // _DEBUGGER_H_
