#include "Debugger.h"

Debugger gDebugger;

Debugger::BreakState Debugger::GetBreakState() const { return breakState; }

void Debugger::Reset() { breakState = BreakState::none; }

void Debugger::NotificyPc(emuptr pc) {}

void Debugger::NotifyMemoryRead(emuptr address) {}

void Debugger::NotifyMemoryWrite(emuptr address) {}

void Debugger::SetBreakpoint(emuptr pc) {}

void Debugger::ClearBreakpoint(emuptr pc) {}

void Debugger::SetWatchpoint(emuptr address) {}

void Debugger::ClearWatchpoint(emuptr address) {}

void Debugger::Interrupt() { breakState = BreakState::externalInterrupt; }

void Debugger::Continue() { breakState = BreakState::none; }

void Debugger::Step() {}
