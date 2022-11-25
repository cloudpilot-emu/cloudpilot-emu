 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Stuff
  *
  * Copyright 1995, 1996 Ed Hanway
  * Copyright 1995-98 Bernd Schmidt
  */

#ifdef __cplusplus
extern "C" {
#endif

#define UAEMAJOR 0
#define UAEMINOR 8
#define UAESUBREV 10


/*
 * You can specify numbers from 0 to 5 here. It is possible that higher
 * numbers will make the CPU emulation slightly faster, but if the setting
 * is too high, you will run out of memory while compiling.
 * Best to leave this as it is.
 */
#define CPU_EMU_SIZE 0

#ifdef __cplusplus
}
#endif

