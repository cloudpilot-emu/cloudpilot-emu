#include "pv_hypercall_interface.h"

#include <cstdio>

#include "CPU.h"

#define HYPERCALL_IDX_GET_RAM_SIZE 0
#define HYPERCALL_IDX_CARD_GET_NUM_SEC 1
#define HYPERCALL_IDX_CARD_READ_SEC 2
#define HYPERCALL_IDX_CARD_WRITE_SEC 3

struct PvHypercallInterface {
    uint32_t ramSize{0};

    ArmCoprocessor cp0{};
};

static bool executeHyptercall(PvHypercallInterface *hypercallIface, ArmCpu *cpu) {
    switch (cpuGetRegExternal(cpu, 0)) {
        case HYPERCALL_IDX_GET_RAM_SIZE:
            cpuSetReg(cpu, 0, hypercallIface->ramSize);
            break;

        case HYPERCALL_IDX_CARD_GET_NUM_SEC:
            cpuSetReg(cpu, 0, 0);
            break;

        case HYPERCALL_IDX_CARD_READ_SEC:
            printf("hypercall: attempt to read sector\n");
            cpuSetReg(cpu, 0, 1);
            break;

        case HYPERCALL_IDX_CARD_WRITE_SEC:
            printf("hypercall: attempt to write sector\n");
            cpuSetReg(cpu, 0, 1);
            break;

        default:
            return false;
    }

    return true;
}

static bool coprocDataProcF(ArmCpu *cpu, void *userData, bool two /* CDP2 ? */, uint8_t op1,
                            uint8_t CRd, uint8_t CRn, uint8_t CRm, uint8_t op2) {
    if (two || op1 || CRd || CRn || CRm || op2) return false;

    return executeHyptercall(reinterpret_cast<PvHypercallInterface *>(userData), cpu);
}

PvHypercallInterface *pvHypercallInterfaceInit(ArmCpu *cpu, uint32_t ramSize) {
    auto hypercallIface = new PvHypercallInterface();
    hypercallIface->ramSize = ramSize;

    hypercallIface->cp0 = {
        .regXfer = nullptr,
        .dataProcessing = coprocDataProcF,
        .memAccess = nullptr,
        .twoRegF = nullptr,
        .userData = hypercallIface,
    };

    cpuCoprocessorRegister(cpu, 0, &hypercallIface->cp0);

    return hypercallIface;
}
