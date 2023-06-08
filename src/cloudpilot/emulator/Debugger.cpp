#include "Debugger.h"

#include "EmCPU68K.h"
#include "EmSession.h"
#include "UAE.h"

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
