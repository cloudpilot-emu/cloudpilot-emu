//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "pxa_PWM.h"

#include <stdlib.h>
#include <string.h>

#include "cputil.h"
#include "mem.h"
#include "savestate/savestateAll.h"

#define PXA_PWM_SIZE 0x0010

#define SAVESTATE_VERSION 0

struct PxaPwm {
    uint32_t duty;
    uint32_t per;
    uint32_t ctrl;

    template <typename T>
    void DoSaveLoad(T& chunkHelper) {
        chunkHelper.Do32(duty).Do32(per).Do32(ctrl);
    }
};

static bool pxaPwmPrvMemAccessF(void* userData, uint32_t pa, uint_fast8_t size, bool write,
                                void* buf) {
    struct PxaPwm* pwm = (struct PxaPwm*)userData;
    uint32_t val = 0;

    if (size != 4) {
        fprintf(stderr, "%s: Unexpected %s of %u bytes to 0x%08lx\n", __func__,
                write ? "write" : "read", size, (unsigned long)pa);
        return false;
    }

    pa %= PXA_PWM_SIZE;
    pa >>= 2;

    if (write) val = *(uint32_t*)buf;

    switch (pa) {
        case 0:
            if (write)
                pwm->ctrl = val;
            else
                val = pwm->ctrl;
            break;

        case 1:
            if (write)
                pwm->duty = val;
            else
                val = pwm->duty;
            break;

        case 2:
            if (write)
                pwm->per = val;
            else
                val = pwm->per;
            break;
    }

    if (!write) *(uint32_t*)buf = val;

    return true;
}

struct PxaPwm* pxaPwmInit(struct ArmMem* physMem, uint32_t base) {
    struct PxaPwm* pwm = (struct PxaPwm*)malloc(sizeof(*pwm));

    if (!pwm) ERR("cannot alloc PWM at 0x%08lx", (unsigned long)base);

    memset(pwm, 0, sizeof(*pwm));

    if (!memRegionAdd(physMem, base, PXA_PWM_SIZE, pxaPwmPrvMemAccessF, pwm))
        ERR("cannot add PWM at 0x%08lx to MEM\n", (unsigned long)base);

    return pwm;
}

template <typename T>
void pxaPwmSave(struct PxaPwm* pwm, T& savestate, uint32_t index) {
    auto chunk = savestate.GetChunk(ChunkType::pxaPwm + index, SAVESTATE_VERSION);
    if (!chunk) ERR("unable to allocate chunk");

    SaveChunkHelper helper(*chunk);
    pwm->DoSaveLoad(helper);
}

template <typename T>
void pxaPwmLoad(struct PxaPwm* pwm, T& loader, uint32_t index) {
    auto chunk = loader.GetChunkOrFail(ChunkType::pxaPwm, SAVESTATE_VERSION, "pxa pwm");
    if (!chunk) return;

    LoadChunkHelper helper(*chunk);
    pwm->DoSaveLoad(helper);
}

template void pxaPwmSave<Savestate<ChunkType>>(PxaPwm* pwm, Savestate<ChunkType>& savestate,
                                               uint32_t index);
template void pxaPwmSave<SavestateProbe<ChunkType>>(PxaPwm* pwm,
                                                    SavestateProbe<ChunkType>& savestate,
                                                    uint32_t index);
template void pxaPwmLoad<SavestateLoader<ChunkType>>(PxaPwm* pwm,
                                                     SavestateLoader<ChunkType>& loader,
                                                     uint32_t index);