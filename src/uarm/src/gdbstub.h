//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _GDB_STUB_H_
#define _GDB_STUB_H_

#include <stdbool.h>
#include <stdint.h>

#include "CPU.h"

struct stub;

struct stub *gdbStubInit(struct ArmCpu *cpu, int port);
void gdbStubDebugBreakRequested(struct stub *stub);
void gdbStubReportPc(struct stub *stub, uint32_t pc, bool thumb);
void gdbStubReportMemAccess(struct stub *stub, uint32_t addr, uint_fast8_t sz, bool write);

#endif
