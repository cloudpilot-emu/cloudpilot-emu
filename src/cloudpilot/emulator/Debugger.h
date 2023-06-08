#ifndef _DEBUGGER_H_
#define _DEBUGGER_H_

#include <array>

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

    const array<uint32, REGISTER_COUNT>& ReadRegisters();

   private:
    BreakState breakState{BreakState::none};
    array<uint32, REGISTER_COUNT> registers;

   private:
    Debugger(const Debugger&) = delete;
    Debugger(Debugger&&) = delete;
    Debugger& operator=(const Debugger&) = delete;
    Debugger& operator=(Debugger&&) = delete;
};

extern Debugger gDebugger;

#endif  // _DEBUGGER_H_
