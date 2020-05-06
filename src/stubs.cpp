
#include "UAE.h"

///////////////////////////////////////////////////////////////////////////////
// EmCPU68k
int areg_byteinc[] = {1, 1, 1, 1, 1, 1, 1, 2};  // (normally in newcpu.c)
int imm8_table[] = {8, 1, 2, 3, 4, 5, 6, 7};    // (normally in newcpu.c)

int movem_index1[256];  // (normally in newcpu.c)
int movem_index2[256];  // (normally in newcpu.c)
int movem_next[256];    // (normally in newcpu.c)

cpuop_func* cpufunctbl[65536];  // (normally in newcpu.c)

uint16 last_op_for_exception_3;    /* Opcode of faulting instruction */
emuptr last_addr_for_exception_3;  /* PC at fault time */
emuptr last_fault_for_exception_3; /* Address that generated the exception */

struct regstruct regs;        // (normally in newcpu.c)
struct flag_struct regflags;  // (normally in support.c),

uae_u32 gStackHigh;
uae_u32 gStackLowWarn;
uae_u32 gStackLow;
uae_u32 gKernelStackOverflowed;
///////////////////////////////////////////////////////////////////////////////

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
