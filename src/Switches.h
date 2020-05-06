/* -*- mode: C++; tab-width: 4 -*- */
/* ===================================================================== *\
        Copyright (c) 1998-2001 Palm, Inc. or its subsidiaries.
        All rights reserved.

        This file is part of the Palm OS Emulator.

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.
\* ===================================================================== */

#ifndef _SWITCHES_H_
#define _SWITCHES_H_

// Use convention: for the preprocessor symbols in this file used to
// turn features on and off, we follow the "if 0 or not 0" convention,
// not the "if defined or not defined" convention.  Thus, to turn
// off a feature, set the symbol to 0.  To turn on a feature, set the
// symbol to 1.

// BYTESWAP is used by Byteswapping.h/.cpp to determine if
// byteswapping should actually occur.  If not, it's a NOP.

#if PLATFORM_WINDOWS || defined(HAVE_ENDIAN_H)

    #include <endian.h>

#elif PLATFORM_MAC || defined(HAVE_MACHINE_ENDIAN_H)

    #include <machine/endian.h>

#elif defined(HAVE_SYS_ISA_DEFS_H)

    // This should handle both Solaris/Sparc and Solaris/Intel
    #include <sys/isa_defs.h>  // Defines either _BIG_ENDIAN or _LITTLE_ENDIAN

#else

    // #error "You need to define __BYTE_ORDER for this platform."

    // You can either define __BYTE_ORDER here, or you can provide
    // a file called endian.h.  If you take the latter course, rerun
    // the configure script so that it can rebuild the makefile with
    // HAVE_ENDIAN_H defined.

    #define __LITTLE_ENDIAN 1234
    #define __BIG_ENDIAN 4321
    #define __PDP_ENDIAN 3412
    #define __BYTE_ORDER __LITTLE_ENDIAN

#endif

// Settle on a canonical name.

#if defined(__BYTE_ORDER)

    #define EM_LITTLE_ENDIAN __LITTLE_ENDIAN
    #define EM_BIG_ENDIAN __BIG_ENDIAN
    #define EM_PDP_ENDIAN __PDP_ENDIAN
    #define EM_HOST_BYTE_ORDER __BYTE_ORDER

#elif defined(BYTE_ORDER)

    #define EM_LITTLE_ENDIAN LITTLE_ENDIAN
    #define EM_BIG_ENDIAN BIG_ENDIAN
    #define EM_PDP_ENDIAN PDP_ENDIAN
    #define EM_HOST_BYTE_ORDER BYTE_ORDER

#elif defined(_BIG_ENDIAN) || defined(_LITTLE_ENDIAN)

    #define EM_LITTLE_ENDIAN 1234
    #define EM_BIG_ENDIAN 4321
    #define EM_PDP_ENDIAN 3412

    #if defined(_BIG_ENDIAN)
        #define EM_HOST_BYTE_ORDER EM_BIG_ENDIAN
    #else
        #define EM_HOST_BYTE_ORDER EM_LITTLE_ENDIAN
    #endif

#else

    #error "Neither BYTE_ORDER nor __BYTE_ORDER defined for this platform."

#endif

#if (EM_HOST_BYTE_ORDER == EM_LITTLE_ENDIAN)
    #define BYTESWAP 1
    #define WORDSWAP_MEMORY 1  // Gives us 12% speedup.

// (adam) When WORDSWAP_MEMORY is defined, a sequence of bytes which is stored in memory on
// the Pilot in the sequence A, B, C, D is stored in Windows memory in the sequence B, A, D, C.
// This means that if p is the address of a byte in Pilot memory, (p ^ 1) is its address in
// Windows memory.  This is fast because we can load a 16-bit word from Windows memory without doing
// any byte swapping and without having to mutate its address, and because we can load a 32-bit int
// from Windows memory just by flipping the 16 high-order bits with the 16 low-order bits.
//
// I thought about whether we could store the above bytes in Windows memory in the "longswapped"
// sequence D, C, B, A, which would allow us to load a 32-bit int without doing any byte swapping at
// all; we would then have to mutate a pointer p to a 16-bit-word into (p ^ 2).  That would probably
// be even better than word swapping if all 32-bit accesses were 32-bit aligned.  Since the 68000
// allows 32-bit accesses on arbitrary 16-bit boundaries, however, this scheme won't work.  For
// example, suppose that the 68000 contains a sequence of bytes starting at address 0x1000:
//     1, 2, 3, 4, 5, 6, 7, 8
// And suppose that we store these bytes in the emulator as
//     4, 3, 2, 1, 8, 7, 6, 5
// Now if we load a 32-bit int starting at 0x1000 simply by reading it from Intel memory, we get
// the value 0x01020304, which is correct.
// But if we load a 32-bit int starting at 0x1002, we get the value 0x07080102, whereas we wanted
// the value 0x03040506.
// We could still make this work if we checked for 32-bit accesses which were not aligned on
// 32-bit boundaries, but I'm not sure whether it would be worth the performance gain.
// I don't know whether the "12% speedup" listed above is a speedup over storing bytes in Windows
// memory exactly as they are stored in 68000 memory, or over storing bytes in this "longswapped"
// way.
//
// (keith) The former.  I never implemented "longswapped" for the reasons you gave.
#else
    #define BYTESWAP 0
    #define WORDSWAP_MEMORY 0
#endif

// Define UNALIGNED_LONG_ACCESS to 1 if the host processor can
// access 32-bit values on all 16-bit boundaries.  Setting it to
// 0 will cause the emulator to fetch 32-bit values with two
// 16-bit operations.

#if defined(sparc)
    #define UNALIGNED_LONG_ACCESS 0
#else
    #define UNALIGNED_LONG_ACCESS 1
#endif

// Define a utility symbol that is guaranteed to be one in debug mode
// and zero in non-debug mode (I don't know what _DEBUG is actually
// defined to, so I'm not going to make any assumptions...)

#if defined(_DEBUG)
    #define ON_IN_DEBUG_MODE 1
#else
    #define ON_IN_DEBUG_MODE 0
#endif

// Define PROFILE_MEMORY to 1 to turn on profiling of memory access.
// Profiling means collecting how often various sections of memory
// are accessed and how they are accessed (read vs. write, byte vs.
// word vs. long.)

#define PROFILE_MEMORY 0

// Define REGISTER_HISTORY to 1 to keep a history of the last 512
// register states.

#define REGISTER_HISTORY ON_IN_DEBUG_MODE

// Define EXCEPTION_HISTORY to 1 to keep a history of the last 512
// exceptions encountered.

#define EXCEPTION_HISTORY ON_IN_DEBUG_MODE

// The number of ticks between calls to WaitNextEvent (Mac only).

#define EVENT_THRESHHOLD 6

// Define NATIVE_DISPATCHING to 1 to have the emulator manage
// the dispatching of SYSTRAP functions.  Otherwise, the ROM
// handles the dispatching.  The advantage of having the emulator
// do the dispatching is that it's faster.

#define NATIVE_DISPATCHING 1

// Define HAS_TRACER to 1 to include Tracer facility.

#if PLATFORM_MAC || PLATFORM_WINDOWS
    #define HAS_TRACER 1
#else
    #define HAS_TRACER 0
#endif

// Define HAS_OMNI_THREAD 1 to if this platform uses that facility
// for multi-threading.

#if PLATFORM_UNIX || PLATFORM_WINDOWS
    #define HAS_OMNI_THREAD 1
#else
    #define HAS_OMNI_THREAD 0
#endif

// Define sub-flags for specific internal features.

// The emulator can optionally check all access to memory to flag questionable
// or illegal accesses.  There are two ways this checking can be controlled.
//
// At the first level, there are runtime variables that turn on and off
// this checking.  The variables control a fine-grain level of checking:
// there are read and write flags for DRAM, SRAM, ROM, and "unmapped
// memory" (for 8 flags in all).  Additionally, we have flags for checking
// for accesses to Dragonballs registers and low-memory, for 12 flags in
// all.
//
// These flags can be turned on and off at runtime.  However, there is
// still the penalty of constantly checking these flags at runtime, even
// if they are turned off.  Therefore, we can opt to compile those checks
// out for maximum performance.  For each runtime flag, there is a
// corresponding compile time flag that controls whether or not the
// runtime check is made.  If the compile time flag is true, the check
// is made based on the value of the runtime flag.  If the compile time
// flag is false, then the code for the runtime check is removed.
//
// With the compile and runtime flags turned on, startup time on
// my Mac is about 1622 msecs.  With the compile-time flags turned
// off, startup time is about 1407 msecs.  With the compile-time flags
// turned on, but the runtime flags turned off, startup time is
// about 1508 msecs.

#define VALIDATE_DUMMY_GET 1
#define VALIDATE_DUMMY_SET 1
#define VALIDATE_REGISTER_GET 0
#define VALIDATE_REGISTER_SET 0
#define VALIDATE_DRAM_GET 1
#define VALIDATE_DRAM_SET 1
#define VALIDATE_SRAM_GET 0
#define VALIDATE_SRAM_SET 0
#define VALIDATE_ROM_GET 0
#define VALIDATE_ROM_SET 1

#define PREVENT_USER_LOWMEM_GET 1
#define PREVENT_USER_LOWMEM_SET 1
#define PREVENT_USER_GLOBAL_GET 1
#define PREVENT_USER_GLOBAL_SET 1
#define PREVENT_USER_SCREEN_GET 1
#define PREVENT_USER_SCREEN_SET 1
#define PREVENT_USER_SRAM_GET 0
#define PREVENT_USER_SRAM_SET 1
#define PREVENT_USER_ROM_GET 0
#define PREVENT_USER_ROM_SET 1
#define PREVENT_USER_REGISTER_GET 1
#define PREVENT_USER_REGISTER_SET 1

#define PREVENT_SYSTEM_LOWMEM_GET 1
#define PREVENT_SYSTEM_LOWMEM_SET 1
#define PREVENT_SYSTEM_ROM_GET 0
#define PREVENT_SYSTEM_ROM_SET 1

#define CHECK_FOR_ADDRESS_ERROR 1
#define CHECK_FOR_LOW_STACK_ACCESS 1
#define CHECK_CHUNK_ACCESS 0

#define MASTER_RUNTIME_VALIDATE_SWITCH 1
#define MASTER_RUNTIME_PREVENT_SWITCH 1

#endif /* _SWITCHES_H_ */
