#ifndef _PACE_PATCH_H_
#define _PACE_PATCH_H_

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct PacePatch {
    uint32_t enterPace;
    uint32_t resumePace;
    uint32_t returnFromCallout;

    uint32_t calloutIllegalInstr;
    uint32_t calloutDivisionByZero;
    uint32_t calloutTrap0;
    uint32_t calloutTrap8;
    uint32_t calloutSyscall;
    uint32_t calloutLine1111;
    uint32_t calloutLine1010;
    uint32_t calloutUnimplementedInstr;
};

struct PacePatch* createPacePatch();

void pacePatchInit(struct PacePatch* patch, uint32_t romBase, void* rom, size_t romSize);

#ifdef __cplusplus
}
#endif

#endif  // _PACE_PATCH_H_
