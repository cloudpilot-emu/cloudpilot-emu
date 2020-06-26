 /* 
  * UAE - The Un*x Amiga Emulator
  * 
  * Memory access functions
  *
  * Copyright 1996 Bernd Schmidt
  */

#include "EmMemory.h"

#define do_get_mem_long		EmMemDoGet32
#define do_get_mem_word		EmMemDoGet16
#define do_get_mem_byte		EmMemDoGet8

#define do_put_mem_long		EmMemDoPut32
#define do_put_mem_word		EmMemDoPut16
#define do_put_mem_byte		EmMemDoPut8

#define get_long			EmMemGet32
#define get_word			EmMemGet16
#define get_byte			EmMemGet8

#define put_long			EmMemPut32
#define put_word			EmMemPut16
#define put_byte			EmMemPut8

#define get_real_address	EmMemGetRealAddress
