#include "EmRegsVZHandEra330c.h"

EmRegsVZHandEra330c::EmRegsVZHandEra330c(HandEra330PortManager** portManager)
    : EmRegsVZHandEra330(portManager) {}

int32 EmRegsVZHandEra330c::GetROMSize(void) { return 8 * 1024 * 1024; }

bool EmRegsVZHandEra330c::CopyLCDFrame(Frame& frame, bool fullRefresh) {
    return EmHALHandler::CopyLCDFrame(frame, fullRefresh);
}

uint16 EmRegsVZHandEra330c::GetLCD2bitMapping() { return EmHALHandler::GetLCD2bitMapping(); }

void EmRegsVZHandEra330c::MarkScreen() {}

void EmRegsVZHandEra330c::UnmarkScreen() {}

void EmRegsVZHandEra330c::MarkScreenDirty() {}
