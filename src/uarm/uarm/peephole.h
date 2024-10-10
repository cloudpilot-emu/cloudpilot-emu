#ifndef _PEEPHOLE_H_
#define _PEEPHOLE_H_

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define INSTR_PEEPHOLE_ADS_UDIVMOD 0xfff9ee90
#define INSTR_PEEPHOLE_ADS_SDIVMOD 0xfff9ee91
#define INSTR_PEEPHOLE_ADS_UDIV10 0xfff9ee92
#define INSTR_PEEPHOLE_ADS_SDIV10 0xfff9ee93
#define INSTR_PEEPHOLE_ADS_MEMCPY 0xfff9ee94

extern const size_t OFFSET_PEEPHOLE_ADS_UDIVMOD_DIVISION_BY_ZERO;
extern const size_t OFFSET_PEEPHOLE_ADS_SDIVMOD_DIVISION_BY_ZERO;

void peepholeOptimize(uint32_t* code, size_t size);

#ifdef __cplusplus
}
#endif

#endif  // _PEEPHOLE_H_