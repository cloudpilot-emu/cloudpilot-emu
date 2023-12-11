//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#ifndef _MMU_H_
#define _MMU_H_

#include <stdbool.h>
#include <stdint.h>

#include "mem.h"

#ifdef __cplusplus
extern "C" {
#endif

struct ArmMmu;

typedef uint64_t MMUTranslateResult;

#define MMU_TRANSLATE_RESULT_OK(x) (!(x & (1ull << 63)))
#define MMU_TRANSLATE_RESULT_CACHEABLE(x) (x & (1ull << 62))
#define MMU_TRANSLATE_RESULT_PA(x) ((uint32_t)x)
#define MMU_TRANSLATE_RESULT_FSR(x) ((uint8_t)((x >> 32) & 0xff))

#define MMU_DISABLED_TTP 0xFFFFFFFFUL

#define MMU_MAPPING_CACHEABLE 0x0001

struct ArmMmu *mmuInit(struct ArmMem *mem, bool xscaleMode);
void mmuReset(struct ArmMmu *mmu);

MMUTranslateResult mmuTranslate(struct ArmMmu *mmu, uint32_t va, bool priviledged, bool write);

bool mmuIsOn(struct ArmMmu *mmu);

uint32_t mmuGetTTP(struct ArmMmu *mmu);
void mmuSetTTP(struct ArmMmu *mmu, uint32_t ttp);

void mmuSetS(struct ArmMmu *mmu, bool on);
void mmuSetR(struct ArmMmu *mmu, bool on);
bool mmuGetS(struct ArmMmu *mmu);
bool mmuGetR(struct ArmMmu *mmu);

uint32_t mmuGetDomainCfg(struct ArmMmu *mmu);
void mmuSetDomainCfg(struct ArmMmu *mmu, uint32_t val);

void mmuTlbFlush(struct ArmMmu *mmu);

void mmuDump(struct ArmMmu *mmu);  // for calling in GDB :)

#ifdef __cplusplus
}
#endif

#endif
