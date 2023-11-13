 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Try to include the right system headers and get other system-specific
  * stuff right & other collected kludges.
  *
  * If you think about modifying this, think twice. Some systems rely on
  * the exact order of the #include statements. That's also the reason
  * why everything gets included unconditionally regardless of whether
  * it's actually needed by the .c file.
  *
  * Copyright 1996, 1997 Bernd Schmidt
  */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "EmTypes.h"		// int8, int16, etc.

#define REGPARAM2 REGPARAM


typedef int8	uae_s8;
typedef int16	uae_s16;
typedef int32	uae_s32;
typedef int64	uae_s64;

typedef uint8	uae_u8;
typedef uint16	uae_u16;
typedef uint32	uae_u32;
typedef uint64	uae_u64;

typedef emuptr	uaecptr;

#ifdef __GNUC__
#define ENUMDECL typedef enum
#define ENUMNAME(name) name
#else
#define ENUMDECL enum
#define ENUMNAME(name) ; typedef int name
#endif

#include "target.h"

#ifndef O_BINARY
#define O_BINARY 0
#endif

#ifndef STATIC_INLINE
#define STATIC_INLINE static __inline__
#endif

#ifdef __cplusplus
}
#endif
