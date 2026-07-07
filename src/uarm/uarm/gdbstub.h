//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _GDB_STUB_H_
#define _GDB_STUB_H_

#include <cstdint>

#include "CPU.h"

struct stub;

struct stub *gdbStubInit(struct ArmCpu *cpu, int port);

#ifdef GDB_STUB_ENABLED
bool gdbStubEnabled(struct stub *stub);
void gdbStubDebugBreakRequested(struct stub *stub);
void gdbStubReportPc(struct stub *stub, uint32_t pc, bool thumb);
void gdbStubReportMemAccess(struct stub *stub, uint32_t addr, uint_fast8_t sz, bool write);
#else
    #define gdbStubEnabled(...) false
    #define gdbStubDebugBreakRequested(...)
    #define gdbStubReportPc(...)
    #define gdbStubReportMemAccess(...)
#endif

#endif
