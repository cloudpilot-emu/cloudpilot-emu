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

bool ramAccessF(void* userData, uint32_t pa, uint_fast8_t size, bool write, void* bufP) {
    struct ArmRam* ram = (struct ArmRam*)userData;
    const uint8_t* addr = (uint8_t*)ram->buf + (pa - ram->adr);

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
                *((uint64_t*)(addr + 32)) = htole64(((uint64_t*)bufP)[4]);
                *((uint64_t*)(addr + 40)) = htole64(((uint64_t*)bufP)[5]);
                *((uint64_t*)(addr + 48)) = htole64(((uint64_t*)bufP)[6]);
                *((uint64_t*)(addr + 56)) = htole64(((uint64_t*)bufP)[7]);
                // fallthrough

            case 32:
                *((uint64_t*)(addr + 16)) = htole64(((uint64_t*)bufP)[2]);
                *((uint64_t*)(addr + 24)) = htole64(((uint64_t*)bufP)[3]);
                // fallthrough

            case 16:
                *((uint64_t*)(addr + 8)) = htole64(((uint64_t*)bufP)[1]);
                // fallthrough

            case 8:
                *((uint64_t*)(addr + 0)) = htole64(((uint64_t*)bufP)[0]);
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
                ((uint64_t*)bufP)[4] = le64toh(*((uint64_t*)(addr + 32)));
                ((uint64_t*)bufP)[5] = le64toh(*((uint64_t*)(addr + 40)));
                ((uint64_t*)bufP)[6] = le64toh(*((uint64_t*)(addr + 48)));
                ((uint64_t*)bufP)[7] = le64toh(*((uint64_t*)(addr + 56)));
                // fallthrough
            case 32:

                ((uint64_t*)bufP)[2] = le64toh(*((uint64_t*)(addr + 16)));
                ((uint64_t*)bufP)[3] = le64toh(*((uint64_t*)(addr + 24)));
                // fallthrough
            case 16:

                ((uint64_t*)bufP)[1] = le64toh(*((uint64_t*)(addr + 8)));
                // fallthrough
            case 8:
                ((uint64_t*)bufP)[0] = le64toh(*((uint64_t*)(addr + 0)));
                break;

            default:

                return false;
        }
    }

    return true;
}

struct ArmRam* ramInit(struct ArmMem* mem, uint32_t adr, uint32_t sz, uint32_t* buf, bool primary) {
    struct ArmRam* ram = (struct ArmRam*)malloc(sizeof(*ram));

    if (!ram) ERR("cannot alloc RAM at 0x%08x", adr);

    memset(ram, 0, sizeof(*ram));

    ram->adr = adr;
    ram->sz = sz;
    ram->buf = buf;

    if (!(primary ? memRegionAddRam(mem, adr, sz, ramAccessF, ram)
                  : memRegionAdd(mem, adr, sz, ramAccessF, ram))) {
        ERR("cannot add RAM at 0x%08x to MEM\n", adr);
    }

    return ram;
}
