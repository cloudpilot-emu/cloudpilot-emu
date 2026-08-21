#include "pv_sysctl.h"

#include <cstdio>

#include "SoC.h"
#include "mem.h"

#define SYSCTL_BASE 0x30000900
#define SYSCTL_SIZE 8
#define SYSCTL_OFFSET_RAMSIZE 0
#define SYSCTL_OFFSET_IDLE 4

struct PvSysctl {
    uint32_t ramSize{0};
    SoC* soc{nullptr};
};

static bool pvSysctlPrvMemAccessF(void* userData, uint32_t pa, uint_fast8_t size, bool write,
                                  void* buf) {
    if (size != 4) {
        fprintf(stderr, "invalid access to sysctl interface\n");
        return false;
    }

    auto sysctl = reinterpret_cast<PvSysctl*>(userData);

    switch ((pa - SYSCTL_BASE) >> 2) {
        case (SYSCTL_OFFSET_RAMSIZE >> 2):
            if (write) return false;

            *reinterpret_cast<uint32_t*>(buf) = sysctl->ramSize;
            break;

        case (SYSCTL_OFFSET_IDLE >> 2):
            if (!write) return false;

            sysctl->soc->Sleep();
            break;

        default:
            return false;
    }

    return true;
}

PvSysctl* pvSysctlInit(ArmMem* mem, SoC* soc, uint32_t ramSize) {
    auto sysctl = new PvSysctl();

    sysctl->ramSize = ramSize;
    sysctl->soc = soc;

    memRegionAdd(mem, SYSCTL_BASE, SYSCTL_SIZE, pvSysctlPrvMemAccessF, sysctl);

    return sysctl;
}
