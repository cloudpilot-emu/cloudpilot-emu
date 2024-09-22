//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "RAM.h"

#include <stdlib.h>
#include <string.h>

#include "SoC.h"
#include "uarm_endian.h"
#include "util.h"

struct ArmRam {
    uint32_t adr;
    uint32_t sz;
    struct RamBuffer buf;
    struct SoC* soc;

    uint32_t framebufferStart;
    uint32_t framebufferStart_2;
    uint32_t framebufferStart_4;
    uint32_t framebufferStart_8;
    uint32_t framebufferStart_16;
    uint32_t framebufferStart_32;
    uint32_t framebufferStart_64;
    uint32_t framebufferEnd;
};

bool ramAccessF(void* userData, uint32_t pa, uint_fast8_t size, bool write, void* bufP) {
    struct ArmRam* ram = (struct ArmRam*)userData;
    const uint32_t offset = pa - ram->adr;
    const uint8_t* addr = (uint8_t*)ram->buf.buffer + offset;

    if (write) {
        RAM_BUFFER_MARK_DIRTY(ram->buf, offset);

        switch (size) {
            case 1:
                if (offset < ram->framebufferEnd && offset >= ram->framebufferStart)
                    socSetFramebufferDirty(ram->soc);

                *((uint8_t*)addr) = *(uint8_t*)bufP;  // our memory system is little-endian
                break;

            case 2:
                if (offset < ram->framebufferEnd && offset >= ram->framebufferStart_2)
                    socSetFramebufferDirty(ram->soc);

                *((uint16_t*)addr) =
                    htole16(*(uint16_t*)bufP);  // our memory system is little-endian
                break;

            case 4:
                if (offset < ram->framebufferEnd && offset >= ram->framebufferStart_4)
                    socSetFramebufferDirty(ram->soc);

                *((uint32_t*)addr) = htole32(*(uint32_t*)bufP);
                break;

            case 64:
                if (offset < ram->framebufferEnd && offset >= ram->framebufferStart_64)
                    socSetFramebufferDirty(ram->soc);

                if (offset & 0x3f) RAM_BUFFER_MARK_DIRTY(ram->buf, offset + 0x3f);

                *((uint64_t*)(addr + 0)) = htole64(((uint64_t*)bufP)[0]);
                *((uint64_t*)(addr + 8)) = htole64(((uint64_t*)bufP)[1]);
                *((uint64_t*)(addr + 16)) = htole64(((uint64_t*)bufP)[2]);
                *((uint64_t*)(addr + 24)) = htole64(((uint64_t*)bufP)[3]);
                *((uint64_t*)(addr + 32)) = htole64(((uint64_t*)bufP)[4]);
                *((uint64_t*)(addr + 40)) = htole64(((uint64_t*)bufP)[5]);
                *((uint64_t*)(addr + 48)) = htole64(((uint64_t*)bufP)[6]);
                *((uint64_t*)(addr + 56)) = htole64(((uint64_t*)bufP)[7]);
                break;

            case 32:
                if (offset < ram->framebufferEnd && offset >= ram->framebufferStart_32)
                    socSetFramebufferDirty(ram->soc);

                if (offset & 0x1f) RAM_BUFFER_MARK_DIRTY(ram->buf, offset + 0x1f);

                *((uint64_t*)(addr + 0)) = htole64(((uint64_t*)bufP)[0]);
                *((uint64_t*)(addr + 8)) = htole64(((uint64_t*)bufP)[1]);
                *((uint64_t*)(addr + 16)) = htole64(((uint64_t*)bufP)[2]);
                *((uint64_t*)(addr + 24)) = htole64(((uint64_t*)bufP)[3]);
                break;

            case 16:
                if (offset < ram->framebufferEnd && offset >= ram->framebufferStart_16)
                    socSetFramebufferDirty(ram->soc);

                if (offset & 0x0f) RAM_BUFFER_MARK_DIRTY(ram->buf, offset + 0x0f);

                *((uint64_t*)(addr + 0)) = htole64(((uint64_t*)bufP)[0]);
                *((uint64_t*)(addr + 8)) = htole64(((uint64_t*)bufP)[1]);
                break;

            case 8:
                if (offset < ram->framebufferEnd && offset >= ram->framebufferStart_8)
                    socSetFramebufferDirty(ram->soc);

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

void ramSetFramebuffer(struct ArmRam* ram, uint32_t base, uint32_t size) {
    if (size > 0) {
        ram->framebufferStart = base - ram->adr;
        ram->framebufferStart_2 = ram->framebufferStart - 2;
        ram->framebufferStart_4 = ram->framebufferStart - 4;
        ram->framebufferStart_8 = ram->framebufferStart - 8;
        ram->framebufferStart_16 = ram->framebufferStart - 16;
        ram->framebufferStart_32 = ram->framebufferStart - 32;
        ram->framebufferStart_64 = ram->framebufferStart - 64;
        ram->framebufferEnd = ram->framebufferStart + size;
    } else {
        ram->framebufferStart = base - ram->adr;
        ram->framebufferStart_2 = 0;
        ram->framebufferStart_4 = 0;
        ram->framebufferStart_8 = 0;
        ram->framebufferStart_16 = 0;
        ram->framebufferStart_32 = 0;
        ram->framebufferStart_64 = 0;
        ram->framebufferEnd = 0xffffffff;
    }
}

struct ArmRam* ramInit(struct ArmMem* mem, struct SoC* soc, uint32_t adr, uint32_t sz,
                       const struct RamBuffer* buf, bool primary) {
    struct ArmRam* ram = (struct ArmRam*)malloc(sizeof(*ram));

    if (!ram) ERR("cannot alloc RAM at 0x%08x", adr);

    memset(ram, 0, sizeof(*ram));

    ram->soc = soc;
    ram->adr = adr;
    ram->sz = sz;
    memcpy(&ram->buf, buf, sizeof(ram->buf));

    ramSetFramebuffer(ram, 0, 0);

    if (!(primary ? memRegionAddRam(mem, adr, sz, ramAccessF, ram)
                  : memRegionAdd(mem, adr, sz, ramAccessF, ram))) {
        ERR("cannot add RAM at 0x%08x to MEM\n", adr);
    }

    return ram;
}
