//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "mmiodev_DirectNAND.h"

#include <stdlib.h>
#include <string.h>

#include "CPU.h"
#include "mem.h"
#include "nand.h"
#include "util.h"

struct DirectNAND {
    uint32_t baseCleAddr, baseAleAddr, baseDataAddr, maskBitsAddr;
    struct SocGpio *gpio;
    struct NAND *nand;

    int8_t rdyPinNo;
};

static void directNandPrvReady(void *userData, bool ready) {
    struct DirectNAND *directNand = (struct DirectNAND *)userData;

    if (directNand->rdyPinNo >= 0) socGpioSetState(directNand->gpio, directNand->rdyPinNo, ready);
}

static bool directNandPrvMemAccessF(void *userData, uint32_t pa, uint_fast8_t size, bool write,
                                    void *buf) {
    struct DirectNAND *directNand = (struct DirectNAND *)userData;
    bool ret, cle = false, ale = false;

    if (size != 1) {
        fprintf(stderr, "%s: Unexpected %s of %u bytes to 0x%08lx\n", __func__,
                write ? "write" : "read", size, (unsigned long)pa);
        return false;
    }

    pa &= ~directNand->maskBitsAddr;

    if (pa == directNand->baseCleAddr)
        cle = true;
    else if (pa == directNand->baseAleAddr)
        ale = true;
    else if (pa != directNand->baseDataAddr)
        return false;

    if (write) {
        ret = nandWrite(directNand->nand, cle, ale, *(uint8_t *)buf);
        //		if (pa & 6)
        //			fprintf(stderr, "NAND w c%u a%u 0x%02x -> %s\n", cle, ale,
        //*(uint8_t*)buf, ret ? "OK" : "FAIL");
    } else {
        ret = nandRead(directNand->nand, cle, ale, (uint8_t *)buf);
        //		if (pa & 6)
        //			fprintf(stderr, "NAND r c%u a%u 0x%02x -> %s\n", cle, ale,
        //*(uint8_t*)buf, ret ? "OK" : "FAIL");
    }

    if (!ret) ERR("NAND ACCESS FAILS\n");

    return ret;
}

struct DirectNAND *directNandInit(struct ArmMem *physMem, uint32_t baseCleAddr,
                                  uint32_t baseAleAddr, uint32_t baseDataAddr,
                                  uint32_t maskBitsAddr, struct SocGpio *gpio, int rdyPin,
                                  const struct NandSpecs *specs, uint8_t *nandContent,
                                  size_t nandSize) {
    struct DirectNAND *directNand = (struct DirectNAND *)malloc(sizeof(*directNand));
    uint32_t minAddr = 0xfffffffful, maxAddr = 0;

    if (!directNand) ERR("cannot alloc TE2 NAND");

    memset(directNand, 0, sizeof(*directNand));
    directNand->baseCleAddr = baseCleAddr;
    directNand->baseAleAddr = baseAleAddr;
    directNand->baseDataAddr = baseDataAddr;
    directNand->maskBitsAddr = maskBitsAddr;
    directNand->gpio = gpio;
    directNand->rdyPinNo = rdyPin;

    directNand->nand = nandInit(nandContent, nandSize, specs, directNandPrvReady, directNand);
    if (!directNand->nand) ERR("Cannot init underlying NAND\n");

    if (baseCleAddr > maxAddr) maxAddr = baseCleAddr;
    if (baseCleAddr < minAddr) minAddr = baseCleAddr;

    if (baseAleAddr > maxAddr) maxAddr = baseAleAddr;
    if (baseAleAddr < minAddr) minAddr = baseAleAddr;

    if (baseDataAddr > maxAddr) maxAddr = baseDataAddr;
    if (baseDataAddr < minAddr) minAddr = baseDataAddr;

    if (!memRegionAdd(physMem, minAddr, maxAddr - minAddr + 4, directNandPrvMemAccessF, directNand))
        ERR("cannot add NAND to MEM\n");

    return directNand;
}

void directNandPeriodic(struct DirectNAND *directNand) { nandPeriodic(directNand->nand); }
