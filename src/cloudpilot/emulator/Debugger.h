#ifndef _DEBUGGER_H_
#define _DEBUGGER_H_

#include "EmCommon.h"

class Debugger {
   public:
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

   private:
    BreakState breakState{BreakState::none};

   private:
    Debugger(const Debugger&) = delete;
    Debugger(Debugger&&) = delete;
    Debugger& operator=(const Debugger&) = delete;
    Debugger& operator=(Debugger&&) = delete;
};

extern Debugger gDebugger;

#endif  // _DEBUGGER_H_
