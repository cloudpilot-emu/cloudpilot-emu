//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdio.h>
#include <stdlib.h>

#define ERR(...)                      \
    do {                              \
        fprintf(stderr, __VA_ARGS__); \
        abort();                      \
    } while (0)

#define __builtin_add_overflow_u32 __builtin_add_overflow
#define __builtin_add_overflow_i32 __builtin_add_overflow
#define __builtin_sub_overflow_u32 __builtin_sub_overflow
#define __builtin_sub_overflow_i32 __builtin_sub_overflow

#ifdef __cplusplus
extern "C" {
#endif

uint64_t timestampUsec();

#ifdef __cplusplus
}
#endif

#endif
