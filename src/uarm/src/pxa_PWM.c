//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "pxa_PWM.h"

#include <stdlib.h>
#include <string.h>

#include "mem.h"
#include "util.h"

#define PXA_PWM_SIZE 0x0010

struct PxaPwm {
    uint32_t duty : 11;
    uint32_t per : 10;
    uint32_t ctrl : 7;
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
