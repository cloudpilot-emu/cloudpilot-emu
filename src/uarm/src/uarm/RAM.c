//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "RAM.h"

#include <stdlib.h>
#include <string.h>

#include "uarm_endian.h"
#include "util.h"

struct ArmRam {
    uint32_t adr;
    uint32_t sz;
    uint32_t* buf;
};

static bool ramAccessF(void* userData, uint32_t pa, uint_fast8_t size, bool write, void* bufP) {
    struct ArmRam* ram = (struct ArmRam*)userData;
    uint8_t* addr = (uint8_t*)ram->buf;

    pa -= ram->adr;
    if (pa >= ram->sz) return false;

    addr += pa;

    if (write) {
        switch (size) {
            case 1:

                *((uint8_t*)addr) = *(uint8_t*)bufP;  // our memory system is little-endian
                break;

            case 2:

                *((uint16_t*)addr) =
                    htole16(*(uint16_t*)bufP);  // our memory system is little-endian
                break;

            case 4:

                *((uint32_t*)addr) = htole32(*(uint32_t*)bufP);
                break;

            case 64:
                *((uint32_t*)(addr + 32)) = htole32(((uint32_t*)bufP)[8]);
                *((uint32_t*)(addr + 36)) = htole32(((uint32_t*)bufP)[9]);
                *((uint32_t*)(addr + 40)) = htole32(((uint32_t*)bufP)[10]);
                *((uint32_t*)(addr + 44)) = htole32(((uint32_t*)bufP)[11]);
                *((uint32_t*)(addr + 48)) = htole32(((uint32_t*)bufP)[12]);
                *((uint32_t*)(addr + 52)) = htole32(((uint32_t*)bufP)[13]);
                *((uint32_t*)(addr + 56)) = htole32(((uint32_t*)bufP)[14]);
                *((uint32_t*)(addr + 60)) = htole32(((uint32_t*)bufP)[15]);
                // fallthrough

            case 32:
                *((uint32_t*)(addr + 16)) = htole32(((uint32_t*)bufP)[4]);
                *((uint32_t*)(addr + 20)) = htole32(((uint32_t*)bufP)[5]);
                *((uint32_t*)(addr + 24)) = htole32(((uint32_t*)bufP)[6]);
                *((uint32_t*)(addr + 28)) = htole32(((uint32_t*)bufP)[7]);
                // fallthrough

            case 16:
                *((uint32_t*)(addr + 8)) = htole32(((uint32_t*)bufP)[2]);
                *((uint32_t*)(addr + 12)) = htole32(((uint32_t*)bufP)[3]);
                // fallthrough

            case 8:
                *((uint32_t*)(addr + 0)) = htole32(((uint32_t*)bufP)[0]);
                *((uint32_t*)(addr + 4)) = htole32(((uint32_t*)bufP)[1]);
                break;

            default:
                return false;
        }
    } else {
        switch (size) {
            case 1:

                *(uint8_t*)bufP = *((uint8_t*)addr);
                break;

            case 2:

                *(uint16_t*)bufP = le16toh(*((uint16_t*)addr));
                break;

            case 4:

                *(uint32_t*)bufP = le32toh(*((uint32_t*)addr));
                break;

            case 64:
                ((uint32_t*)bufP)[8] = le32toh(*((uint32_t*)(addr + 32)));
                ((uint32_t*)bufP)[9] = le32toh(*((uint32_t*)(addr + 36)));
                ((uint32_t*)bufP)[10] = le32toh(*((uint32_t*)(addr + 40)));
                ((uint32_t*)bufP)[11] = le32toh(*((uint32_t*)(addr + 44)));
                ((uint32_t*)bufP)[12] = le32toh(*((uint32_t*)(addr + 48)));
                ((uint32_t*)bufP)[13] = le32toh(*((uint32_t*)(addr + 52)));
                ((uint32_t*)bufP)[14] = le32toh(*((uint32_t*)(addr + 56)));
                ((uint32_t*)bufP)[15] = le32toh(*((uint32_t*)(addr + 60)));
                // fallthrough
            case 32:

                ((uint32_t*)bufP)[4] = le32toh(*((uint32_t*)(addr + 16)));
                ((uint32_t*)bufP)[5] = le32toh(*((uint32_t*)(addr + 20)));
                ((uint32_t*)bufP)[6] = le32toh(*((uint32_t*)(addr + 24)));
                ((uint32_t*)bufP)[7] = le32toh(*((uint32_t*)(addr + 28)));
                // fallthrough
            case 16:

                ((uint32_t*)bufP)[2] = le32toh(*((uint32_t*)(addr + 8)));
                ((uint32_t*)bufP)[3] = le32toh(*((uint32_t*)(addr + 12)));
                // fallthrough
            case 8:
                ((uint32_t*)bufP)[0] = le32toh(*((uint32_t*)(addr + 0)));
                ((uint32_t*)bufP)[1] = le32toh(*((uint32_t*)(addr + 4)));
                break;

            default:

                return false;
        }
    }

    return true;
}

struct ArmRam* ramInit(struct ArmMem* mem, uint32_t adr, uint32_t sz, uint32_t* buf) {
    struct ArmRam* ram = (struct ArmRam*)malloc(sizeof(*ram));

    if (!ram) ERR("cannot alloc RAM at 0x%08x", adr);

    memset(ram, 0, sizeof(*ram));

    ram->adr = adr;
    ram->sz = sz;
    ram->buf = buf;

    if (!memRegionAdd(mem, adr, sz, ramAccessF, ram)) ERR("cannot add RAM at 0x%08x to MEM\n", adr);

    return ram;
}
