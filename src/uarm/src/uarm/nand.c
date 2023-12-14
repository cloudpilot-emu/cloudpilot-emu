//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "nand.h"

#include <stdlib.h>
#include <string.h>

#include "CPU.h"
#include "mem.h"
#include "util.h"

enum K9nandState {
    K9nandStateReset,
    K9nandStateReadId,
    K9nandStateProgramAddrRxing,
    K9nandStateProgramDataRxing,
    K9nandStateEraseAddrRxing,
    K9nandStateReading,
    K9nandStateStatusReading,
};

enum K9nandArea {  // K9nandAreaB, K9nandAreaC only used if
                   // flags.NAND_FLAG_SAMSUNG_ADDRESSED_VIA_AREAS is set
    K9nandAreaA,
    K9nandAreaB,
    K9nandAreaC,
};

struct NAND {
    NandReadyCbk readyCbk[2];
    void *readyCbkData[2];

    // geometry and id
    uint32_t bytesPerPage;  // main plus spare
    uint32_t blocksPerDevice;
    uint16_t areaSize;
    uint8_t pagesPerBlockLg2;  // log base 2 (eg: if device has 32 pages per block, this will be 5)
    uint8_t deviceId[6];
    uint8_t deviceIdLen;
    uint8_t byteAddrBytes;
    uint8_t pageAddrBytes;
    uint8_t flags;

    // runtime state
    enum K9nandState state;
    enum K9nandArea area;
    uint8_t addrBytesRxed;
    uint8_t addrBytes[8];
    uint32_t pageNo;    // read & program only
    uint32_t pageOfst;  // for read ops only
    uint32_t busyCt;
    uint8_t *pageBuf;

    // data
    uint8_t *data;  // stores inverted data (so 0-init is valid)
};

void nandSecondReadyCbkSet(struct NAND *nand, NandReadyCbk readyCbk, void *readyCbkData) {
    nand->readyCbk[1] = readyCbk;
    nand->readyCbkData[1] = readyCbkData;
}

static void nandPrvCallReadyCbks(struct NAND *nand, bool ready) {
    uint_fast8_t i;

    for (i = 0; i < sizeof(nand->readyCbk) / sizeof(*nand->readyCbk); i++) {
        if (nand->readyCbk[i]) nand->readyCbk[i](nand->readyCbkData[i], ready);
    }
}

static void nandPrvBusy(struct NAND *nand, uint32_t count) {
    if (nand->busyCt)
        ERR("NAND was already busy\n");
    else {
        nand->busyCt = count;
        nandPrvCallReadyCbks(nand, false);
    }
}

static uint32_t nandPrvGetAddrValLE(struct NAND *nand, uint_fast8_t startByte,
                                    uint_fast8_t numBytes) {
    uint32_t ret = 0;
    uint_fast8_t i;

    for (i = 0; i < numBytes; i++) {
        ret <<= 8;
        ret += nand->addrBytes[startByte + numBytes - 1 - i];
    }

    return ret;
}

static uint32_t nandPrvGetPageAddr(struct NAND *nand, uint_fast8_t addrOfst) {
    return nandPrvGetAddrValLE(nand, addrOfst, nand->pageAddrBytes);
}

static uint32_t nandPrvGetByteAddr(struct NAND *nand) {
    return nandPrvGetAddrValLE(nand, 0, nand->byteAddrBytes);
}

static bool nandPrvBlockErase(struct NAND *nand) {
    uint32_t addr = nandPrvGetPageAddr(nand, 0);

    if (addr & ((1UL << nand->pagesPerBlockLg2) - 1)) return false;

    // fprintf(stderr, " NAND ERASE BLOCK %u\n", addr / NAND_PAGES_PER_BLOCK);
    memset(nand->data + addr * nand->bytesPerPage, 0xff,
           nand->bytesPerPage << nand->pagesPerBlockLg2);

    return true;
}

static bool nandPrvPageProgram(struct NAND *nand) {
    uint32_t i;

    // fprintf(stderr, " NAND PROGRAM PAGE %u (addr %u.%u)\n", nand->pageNo, nand->pageNo /
    // NAND_PAGES_PER_BLOCK, nand->pageNo % NAND_PAGES_PER_BLOCK);

    for (i = 0; i < nand->bytesPerPage; i++) {
        nand->data[nand->pageNo * nand->bytesPerPage + i] &= nand->pageBuf[i];
        //	if (nand->data[nand->pageNo * NAND_PAGE_SIZE + i] != nand->pageBuf[i])
        //		fprintf(stderr, "write fail for page %u af ofst %u. wrote 0x%02x, read
        // 0x%02x\n", nand->pageNo, i, nand->pageBuf[i], nand->data[nand->pageNo * NAND_PAGE_SIZE +
        // i]);
    }

    return true;
}

static bool nandPrvAcceptProgramData(struct NAND *nand, uint8_t val) {
    if (nand->pageOfst >= nand->bytesPerPage) {
        // real hardware ignored extra bytes being written, and so do we
        return true;
    }

    nand->pageBuf[nand->pageOfst++] = val;

    return true;
}

bool nandWrite(struct NAND *nand, bool cle, bool ale, uint8_t val) {
    if (cle && !ale) {  // command

        switch (val) {
            case 0x00:
                nand->area = K9nandAreaA;
                nand->state = K9nandStateReading;
                nand->addrBytesRxed = 0;
                break;

            case 0x01:
                if (nand->flags & NAND_FLAG_SAMSUNG_ADDRESSED_VIA_AREAS) {
                    nand->area = K9nandAreaB;
                    nand->state = K9nandStateReading;
                    nand->addrBytesRxed = 0;
                } else
                    return false;
                break;

            case 0x30:
                if ((nand->flags & NAND_HAS_SECOND_READ_CMD) &&
                    nand->addrBytesRxed == nand->byteAddrBytes + nand->pageAddrBytes)
                    nand->addrBytesRxed = 0xfe;  // special value
                else
                    return false;
                break;

            case 0x50:
                if (nand->flags & NAND_FLAG_SAMSUNG_ADDRESSED_VIA_AREAS) {
                    nand->area = K9nandAreaC;
                    nand->state = K9nandStateReading;
                    nand->addrBytesRxed = 0;
                } else
                    return false;
                break;

            case 0x60:
                if (nand->state != K9nandStateReset && nand->state != K9nandStateReading &&
                    nand->state != K9nandStateReadId && nand->state != K9nandStateStatusReading)
                    return false;
                nand->state = K9nandStateEraseAddrRxing;
                nand->addrBytesRxed = 0;
                break;

            case 0xd0:
                if (nand->area == K9nandAreaB) nand->area = K9nandAreaA;
                if (nand->state != K9nandStateEraseAddrRxing || nand->addrBytesRxed != 2)
                    return false;
                if (!nandPrvBlockErase(nand)) return false;
                nandPrvBusy(nand, 100);
                nand->state = K9nandStateReset;
                nand->addrBytesRxed = 0;
                break;

            case 0x80:
                if (nand->state != K9nandStateReset && nand->state != K9nandStateReading &&
                    nand->state != K9nandStateReadId && nand->state != K9nandStateStatusReading)
                    return false;
                nand->state = K9nandStateProgramAddrRxing;
                nand->addrBytesRxed = 0;
                break;

            case 0x10:
                if (nand->area == K9nandAreaB) nand->area = K9nandAreaA;
                if (nand->state != K9nandStateProgramDataRxing) return false;
                // fprintf(stderr, "comitting page write for page %u, cur ofst %u\n", nand->pageNo,
                // nand->pageOfst);
                if (!nandPrvPageProgram(nand)) return false;
                nandPrvBusy(nand, 10);
                nand->state = K9nandStateReset;
                nand->addrBytesRxed = 0;
                break;

            case 0x90:
                if (nand->state != K9nandStateReset && nand->state != K9nandStateReading &&
                    nand->state != K9nandStateReadId && nand->state != K9nandStateStatusReading)
                    return false;
                nand->state = K9nandStateReadId;
                nand->addrBytesRxed = 0;
                break;

            case 0x70:
                nand->state = K9nandStateStatusReading;
                nand->addrBytesRxed = 0;
                break;

            case 0xff:
                nand->area = K9nandAreaA;
                nand->state = K9nandStateReset;
                nand->addrBytesRxed = 0;
                break;

            default:
                fprintf(stderr, "unknown command 0x%02x. halt.\n", val);
                while (1)
                    ;
        }
    } else if (!cle && ale) {  // addr

        switch (nand->state) {
            case K9nandStateReadId:
                if (nand->addrBytesRxed >= 1) return false;
                break;
            case K9nandStateProgramAddrRxing:
                if (nand->addrBytesRxed >= nand->byteAddrBytes + nand->pageAddrBytes)
                    return true;  // ignore
                break;

            case K9nandStateEraseAddrRxing:
                if (nand->addrBytesRxed >= nand->pageAddrBytes) return true;  // ignore
                break;
            case K9nandStateReading:
                if (nand->addrBytesRxed >= nand->byteAddrBytes + nand->pageAddrBytes)
                    return true;  // ignore
                if (nand->addrBytesRxed ==
                    nand->byteAddrBytes + nand->pageAddrBytes - 1)  // about to become enough
                    nandPrvBusy(nand, 1);
                break;
            default:
                return false;
        }
        nand->addrBytes[nand->addrBytesRxed++] = val;
    } else if (!cle && !ale) {  // data

        switch (nand->state) {
            case K9nandStateProgramAddrRxing:
                if (nand->addrBytesRxed != nand->byteAddrBytes + nand->pageAddrBytes) return false;
                nand->state = K9nandStateProgramDataRxing;
                nand->pageNo = nandPrvGetPageAddr(nand, nand->byteAddrBytes);

                // fprintf(stderr, "writing page %5u (block addr %4u.%2u)\n", nand->pageNo,
                // nand->pageNo / NAND_PAGES_PER_BLOCK, nand->pageNo % NAND_PAGES_PER_BLOCK);

                if (nand->pageNo >= (nand->blocksPerDevice << nand->pagesPerBlockLg2)) return false;

                memset(nand->pageBuf, 0xff, nand->bytesPerPage);

                switch (nand->area) {
                    case K9nandAreaA:
                        nand->pageOfst = 0;
                        break;
                    case K9nandAreaB:
                        nand->area = K9nandAreaA;
                        nand->pageOfst = nand->areaSize;
                        break;
                    case K9nandAreaC:
                        nand->pageOfst = 2 * nand->areaSize;
                        break;
                    default:
                        return false;
                }
                nand->pageOfst += nandPrvGetByteAddr(nand);
                // fallthrough

            case K9nandStateProgramDataRxing:
                if (!nandPrvAcceptProgramData(nand, val)) return false;
                break;

            default:
                return false;
        }
    } else
        return false;

    return true;
}

bool nandRead(struct NAND *nand, bool cle, bool ale, uint8_t *valP) {
    if (cle || ale) return false;

    switch (nand->state) {
        case K9nandStateReadId:
            if (nand->addrBytesRxed != 1) return false;
            if (nand->addrBytes[0] >= nand->deviceIdLen) return false;
            *valP = nand->deviceId[nand->addrBytes[0]++];
            return true;

        case K9nandStateStatusReading:
            *valP = nand->busyCt ? 0x00 : 0x40;
            return true;

        case K9nandStateReading:

            if (!(nand->flags & NAND_HAS_SECOND_READ_CMD) &&
                nand->addrBytesRxed == nand->byteAddrBytes + nand->pageAddrBytes)
                nand->addrBytesRxed = 0xfe;  // special marker

            if (nand->addrBytesRxed == 0xfe) {
                nand->pageNo = nandPrvGetPageAddr(nand, nand->byteAddrBytes);
                nand->addrBytesRxed = 0xff;  // special marker

                // fprintf(stderr, "reading page %5u (block addr %4u.%2u) area %c offset %u\n",
                //	nand->pageNo, nand->pageNo / NAND_PAGES_PER_BLOCK, nand->pageNo %
                // NAND_PAGES_PER_BLOCK, 'A' + (unsigned)nand->area, nand->addr[0]);

                if (nand->pageNo >= (nand->blocksPerDevice << nand->pagesPerBlockLg2)) {
                    fprintf(stderr, "page number ouf of bounds\n");
                    return false;
                }

                memcpy(nand->pageBuf, &nand->data[nand->pageNo * nand->bytesPerPage],
                       nand->bytesPerPage);

                switch (nand->area) {
                    case K9nandAreaA:
                        nand->pageOfst = 0;
                        break;
                    case K9nandAreaB:
                        nand->pageOfst = nand->areaSize;
                        nand->area = K9nandAreaA;
                        break;
                    case K9nandAreaC:
                        nand->pageOfst = 2 * nand->areaSize;
                        break;
                }
                nand->pageOfst += nandPrvGetByteAddr(nand);
            }
            if (nand->addrBytesRxed != 0xff) return false;
            if (nand->pageOfst == nand->bytesPerPage) {  // read next page

                nand->pageNo++;
                nand->pageOfst = (nand->area == K9nandAreaC) ? 2 * nand->areaSize : 0;

                // fprintf(stderr, "reading page %5u (block addr %4u.%2u)\n", nand->pageNo,
                // nand->pageNo / NAND_PAGES_PER_BLOCK, nand->pageNo % NAND_PAGES_PER_BLOCK);

                memcpy(nand->pageBuf, &nand->data[nand->pageNo * nand->bytesPerPage],
                       nand->bytesPerPage);
            }
            *valP = nand->pageBuf[nand->pageOfst++];
            // fprintf(stderr, "[%3x] = 0x%02x\n", nand->pageOfst - 1, *valP);
            return true;

        default:
            return false;
    }
}

void nandPeriodic(struct NAND *nand) {
    if (nand->busyCt && !--nand->busyCt) nandPrvCallReadyCbks(nand, true);
}

bool nandIsReady(struct NAND *nand) { return !nand->busyCt; }

struct NAND *nandInit(uint8_t *nandContent, size_t nandSize, const struct NandSpecs *specs,
                      NandReadyCbk readyCbk, void *readyCbkData) {
    struct NAND *nand = (struct NAND *)malloc(sizeof(*nand));
    uint32_t nandSz, nandPages, t;

    if (!nand) ERR("cannot alloc NAND");

    memset(nand, 0, sizeof(*nand));
    nand->readyCbk[0] = readyCbk;
    nand->readyCbkData[0] = readyCbkData;

    nand->flags = specs->flags;
    nand->bytesPerPage = specs->bytesPerPage;
    nand->pagesPerBlockLg2 = specs->pagesPerBlockLg2;
    nand->blocksPerDevice = specs->blocksPerDevice;
    if (specs->devIdLen > sizeof(nand->deviceId))
        ERR("Device ID too long\n");
    else
        nand->deviceIdLen = specs->devIdLen;
    memcpy(nand->deviceId, specs->devId, nand->deviceIdLen);

    nandPages = nand->blocksPerDevice << nand->pagesPerBlockLg2;
    nandSz = nand->bytesPerPage * nandPages;

    t = 31 - __builtin_clz(specs->bytesPerPage - 1);
    if (specs->flags &
        NAND_FLAG_SAMSUNG_ADDRESSED_VIA_AREAS)  // one bit of address goes away via "area" commands
        t--;

    nand->byteAddrBytes = (t + 7) / 8;
    nand->areaSize = 1 << t;
    nand->pageAddrBytes = (31 - __builtin_clz(nandPages - 1) + 7) / 8;

    nand->pageBuf = (uint8_t *)malloc(nand->bytesPerPage);
    if (!nand->pageBuf) ERR("canont allcoate NAND page buffer\n");

    if (nandContent) {
        if (nandSize != nandSz) {
            fprintf(stderr, "Cannot use nand. got %lu, wanted %lu\n", (unsigned long)t,
                    (unsigned long)nandSz);
            free(nand);
        }

        nand->data = nandContent;
    } else {
        memset(nand->data, 0xff, nandSz);

        nand->data = (uint8_t *)malloc(nandSz);
        if (!nand->data) ERR("canont allcoate NAND data buffer\n");
    }

    nandPrvBusy(nand, 1);  // we start busy for a little bit

    return nand;
}
