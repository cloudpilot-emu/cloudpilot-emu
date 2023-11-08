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

#define MMU_DISABLED_TTP 0xFFFFFFFFUL

#define MMU_MAPPING_CACHEABLE 0x0001

struct ArmMmu *mmuInit(struct ArmMem *mem, bool xscaleMode);
bool mmuTranslate(struct ArmMmu *mmu, uint32_t va, bool priviledged, bool write, uint32_t *paP,
                  uint_fast8_t *fsrP, uint8_t *mappingInfoP, struct ArmMemRegion **region);

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
