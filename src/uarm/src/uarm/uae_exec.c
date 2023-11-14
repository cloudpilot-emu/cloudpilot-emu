#include "uae_exec.h"

#include "uae/UAE.h"

#define EXC_ILLEGAL_INSTRUCTION 4
#define EXC_DIVISION_BY_ZERO 7
#define EXC_TRAP0 32
#define EXC_TRAP8 40
#define EXC_SYSCALL 47
#define EXC_UNIMPLEMENTED 0x0100
#define EXC_LINE_1111 0x0200
#define EXC_LINE_1010 0x0300
#define EXC_RETURN 0x1000

static struct ArmMem* mem = NULL;
static struct ArmMmu* mmu = NULL;

static uint8_t fsr = 0;
static uint32_t pendingException = 0;
static bool priviledged = false;

static uint32_t uae_get(uint32_t addr, uint8_t size) {
    struct ArmMemRegion* region;
    uint32_t pa;
    if (!mmuTranslate(mmu, addr, priviledged, false, &pa, &fsr, NULL, &region)) return 0;

    uint8_t result;
    bool ok = region ? region->aF(region->uD, pa, size, false, &result)
                     : memAccess(mem, pa, size, MEM_ACCESS_TYPE_READ, &result);

    if (!ok) {
        fsr = 10;  // external abort on non-linefetch
        return 0;
    }

    return result;
}

uint8_t uae_get8(uint32_t addr) {
    fsr = 0;

    return uae_get(addr, 1);
}

uint16_t uae_get16(uint32_t addr) {
    fsr = 0;

    if (addr & 0x01) {
        fsr = 1;
        return 0;
    }

    return uae_get(addr, 2);
}

uint32_t uae_get32(uint32_t addr) {
    fsr = 0;

    if (addr & 0x03) {
        fsr = 1;
        return 0;
    }

    return uae_get(addr, 4);
}

static void uae_put(uint32_t value, uint32_t addr, uint8_t size) {
    struct ArmMemRegion* region;
    uint32_t pa;
    if (!mmuTranslate(mmu, addr, priviledged, false, &pa, &fsr, NULL, &region)) return;

    bool ok = region ? region->aF(region->uD, pa, size, true, &value)
                     : memAccess(mem, pa, size, MEM_ACCESS_TYPE_WRITE, &value);

    if (!ok) {
        fsr = 10;  // external abort on non-linefetch
    }
}

void uae_put8(uint8_t value, uint32_t addr) {
    fsr = 0;

    uae_put(value, addr, 1);
};

void uae_put16(uint16_t value, uint32_t addr) {
    fsr = 0;

    if (addr & 0x01) {
        fsr = 1;
        return;
    }

    uae_put(value, addr, 2);
}

void uae_put32(uint32_t value, uint32_t addr) {
    fsr = 0;

    if (addr & 0x03) {
        fsr = 1;
        return;
    }

    uae_put(value, addr, 4);
}

void Exception(int exception, uaecptr lastPc) {
    if (pendingException == 0) pendingException = exception;
}

unsigned long op_unimplemented(uint32_t opcode) REGPARAM {
    if (pendingException == 0) pendingException = EXC_UNIMPLEMENTED;
    return 0;
}

unsigned long op_illg(uae_u32) REGPARAM {
    if (pendingException == 0) pendingException = EXC_ILLEGAL_INSTRUCTION;
    return 0;
}

void notifiyReturn() {
    if (pendingException == 0) pendingException = EXC_RETURN;
}
