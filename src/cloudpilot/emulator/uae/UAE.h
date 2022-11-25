#ifndef UAE_h
#define UAE_h

#include "sysconfig.h"		// (should really be included by sysdeps.h ...)
#include "sysdeps.h"		// uae_u8, uae_u16, uae_u32, etc.
#include "config.h"
#include "options.h"
#include "machdep_m68k.h"
#include "memory_cpu.h"
#include "custom.h"
#include "readcpu.h"
#include "newcpu.h"
#include "compiler.h"
#include "cputbl.h"

// sysdeps.h brings in assert.h.  However,
// we don't want to use assert in our application
// so undef it.  Use EmAssert instead.
#undef assert

#endif /* UAE_h */
