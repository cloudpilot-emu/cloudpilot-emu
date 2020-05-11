
#include "UAE.h"

///////////////////////////////////////////////////////////////////////////////
// EmUAEGlue
void customreset(void) {}

void Exception(int nr, emuptr /*oldpc*/) {}

unsigned long op_illg(uint32 iOpcode) { return 0; }

void Software_ProcessLINK(int32 linkSize) {}

int Software_ProcessRTS(emuptr dest) { return 0; }

int Software_ProcessRTE(emuptr dest) { return 0; }

int Software_ProcessJSR(emuptr oldpc, emuptr dest) { return 0; }

int Software_ProcessJSR_Ind(emuptr oldpc, emuptr dest) { return 0; }

void Software_CheckNewPC(emuptr dest) {}

void Software_CheckStackPointerAssignment() {}

void Software_CheckStackPointerDecrement() {}

void Software_CheckStackPointerIncrement() {}

void Software_CheckKernelStack() {}

void MakeSR(void) {}

void MakeFromSR(void) {}

void* xmalloc(size_t size) { return malloc(size); }

uint32 get_disp_ea_000(uint32 base, uint32 dp) {
    int reg = (dp >> 12) & 15;
    uae_s32 regd = regs.regs[reg];

    if ((dp & 0x800) == 0) regd = (uae_s32)(uae_s16)regd;

    return base + (uae_s8)dp + regd;
}
///////////////////////////////////////////////////////////////////////////////
