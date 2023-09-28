//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "pxa255_DSP.h"

#include <stdlib.h>
#include <string.h>

#include "util.h"

struct Pxa255dsp {
    uint64_t acc0;
};

bool pxa255dspPrvAccess(struct ArmCpu* cpu, void* userData, bool MRRC, uint8_t op, uint8_t RdLo,
                        uint8_t RdHi, uint8_t acc) {
    struct Pxa255dsp* dsp = (struct Pxa255dsp*)userData;

    if (acc != 0 || op != 0) return false;  // bad encoding

    if (MRRC) {  // MRA: read acc0

        cpuSetReg(cpu, RdLo, dsp->acc0);
        cpuSetReg(cpu, RdHi, (dsp->acc0 >> 32) & 0xff);
    } else {  // MAR: write acc0

        dsp->acc0 = (((uint64_t)(cpuGetRegExternal(cpu, RdHi) & 0xff)) << 32) |
                    cpuGetRegExternal(cpu, RdLo);
    }

    return true;
}

bool pxa255dspPrvOp(struct ArmCpu* cpu, void* userData, bool two /* MCR2/MRC2 ? */, bool MRC,
                    uint8_t op1, uint8_t Rs, uint8_t opcode_3, uint8_t Rm, uint8_t acc) {
    struct Pxa255dsp* dsp = (struct Pxa255dsp*)userData;
    uint64_t addend = dsp->acc0;
    uint32_t Vs, Vm;

    if (op1 != 1 || two || MRC || acc != 0) return false;  // bad encoding

    Vs = cpuGetRegExternal(cpu, Rs);
    Vm = cpuGetRegExternal(cpu, Rm);

    switch (opcode_3 >> 2) {
        case 0:  // MIA
            addend += (int64_t)(int32_t)Vm * (int64_t)(int32_t)Vs;
            break;

        case 1:  // invalid
            return false;

        case 2:  // MIAPH
            addend += (int64_t)(int16_t)Vm * (int64_t)(int16_t)Vs;
            addend += (int64_t)(int16_t)(Vm >> 16) * (int64_t)(int16_t)(Vs >> 16);
            break;

        case 3:                           // MIAxy
            if (opcode_3 & 2) Vm >>= 16;  // X set
            if (opcode_3 & 1) Vs >>= 16;  // Y set
            addend += (int64_t)(int16_t)Vm * (int64_t)(int16_t)Vs;
            break;
    }

    addend &= 0xFFFFFFFFFFULL;

    dsp->acc0 = addend;

    return true;
}

struct Pxa255dsp* pxa255dspInit(struct ArmCpu* cpu) {
    struct Pxa255dsp* dsp = (struct Pxa255dsp*)malloc(sizeof(*dsp));
    struct ArmCoprocessor cp = {
        .regXfer = pxa255dspPrvOp,
        .dataProcessing = NULL,
        .memAccess = NULL,
        .twoRegF = pxa255dspPrvAccess,
        .userData = dsp,
    };

    if (!dsp) ERR("cannot alloc DSP");

    memset(dsp, 0, sizeof(*dsp));

    cpuCoprocessorRegister(cpu, 0, &cp);

    return dsp;
}
