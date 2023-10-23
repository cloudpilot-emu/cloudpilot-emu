//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "ROM.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mem.h"
#include "uarm_endian.h"
#include "util.h"

#define STRATAFLASH_BLOCK_SIZE 0x20000ul

enum StrataFlashMode {
    StrataFlashNormal,
    StrataFlashReadStatus,
    StrataFlashSetReadConfigRegister,
    StrataFlashReadID,
    StrataFlashReadCFI,
    StrataFlashErzCy1,
    StrataFlashWriCy1,
};

struct ArmRomPiece {
    struct ArmRomPiece *next;
    struct ArmRom *rom;
    uint32_t base, size;
    uint32_t *buf;
};

struct ArmRom {
    uint32_t start, opAddr;
    struct ArmRomPiece *pieces;
    enum RomChipType chipType;
    enum StrataFlashMode mode;
    uint16_t configReg, busyCy;
};

static bool romPrvWrite(struct ArmRom *rom, uint32_t ofst, uint_fast16_t val) {
    struct ArmRomPiece *piece = rom->pieces;

    // fprintf(stderr, "SF write of 0x%04x at 0x%08lx\n", (unsigned)val, (unsigned long)ofst);

    while (piece && piece->size <= ofst) {
        ofst -= piece->size;
        piece = piece->next;
    }

    if (!piece) return false;

    *(uint16_t *)(((char *)piece->buf) + ofst) &= le16toh(val);

    return true;
}

static bool romPrvErase(struct ArmRom *rom, uint32_t ofst) {
    struct ArmRomPiece *piece = rom->pieces;
    uint32_t now, sz = STRATAFLASH_BLOCK_SIZE;

    fprintf(stderr, "SF erase at 0x%08x\n", ofst);

    while (piece && piece->size <= ofst) {
        ofst -= piece->size;
        piece = piece->next;
    }

    if (!piece) return false;

    while (sz) {
        if (!piece) return false;

        now = piece->size - ofst;
        if (now > sz) now = sz;

        memset(((char *)piece->buf) + ofst, 0xff, now);
        sz -= now;
        ofst = 0;
        piece = piece->next;
    }

    return true;
}

static bool romAccessF(void *userData, uint32_t pa, uint_fast8_t size, bool write, void *bufP) {
    struct ArmRomPiece *piece = (struct ArmRomPiece *)userData;
    uint8_t *addr = (uint8_t *)piece->buf;
    struct ArmRom *rom = piece->rom;
    uint32_t cmd, fromStart;
    bool haveCmd = 0;

    fromStart = pa - rom->start;  // flashes care how far we are from start of flash, not of this
                                  // arbitrary piece of it
    pa -= piece->base;
    if (pa >= piece->size) return false;

    addr += pa;

    if (write) {
        switch (rom->chipType) {
            case RomWriteIgnore:
                return true;
            case RomWriteError:
                return false;
            case RomStrataflash16x2x:
                if (size != 4) {
                    fprintf(stderr, "StrataflashX2 command of improper size!\n");
                    return false;
                }
                cmd = *(uint32_t *)bufP;
                if (cmd < 4)  // STS commands are weird...
                    cmd &= 0xffff;
                else if ((cmd & 0xffff) != (cmd >> 16)) {
                    fprintf(stderr, "StrataflashX2 commands differ for flash halves: 0x%08x!\n",
                            cmd);
                    return false;
                }
                cmd &= 0xffff;
                haveCmd = true;
                break;
            case RomStrataFlash16x:
                if (size != 2) {
                    fprintf(stderr, "Strataflash command of improper size!\n");
                    return false;
                }
                cmd = *(uint16_t *)bufP;
                haveCmd = true;
                break;
            default:
                return false;
        }

        if (haveCmd) {
            if (rom->mode == StrataFlashSetReadConfigRegister) {
                rom->configReg = cmd;
                rom->mode = StrataFlashNormal;
                return true;
            } else if (rom->mode == StrataFlashWriCy1) {
                if (fromStart != rom->opAddr) return false;
                if (!romPrvWrite(rom, fromStart, cmd)) return false;
                rom->busyCy = 0x0010;
                rom->mode = StrataFlashReadStatus;
                return true;
            } else
                switch (cmd & 0xff) {
                    case 0x00 ... 0x03:  // STS Settings
                        return true;

                    case 0x50:
                        if (rom->mode == StrataFlashErzCy1 || rom->mode == StrataFlashWriCy1)
                            return false;

                        // clear status register
                        rom->mode = StrataFlashNormal;
                        return true;

                    case 0x60:
                        if (rom->mode == StrataFlashErzCy1 || rom->mode == StrataFlashWriCy1)
                            return false;

                        // set read config reg
                        rom->mode = StrataFlashSetReadConfigRegister;
                        return true;

                    case 0x70:
                        if (rom->mode == StrataFlashErzCy1 || rom->mode == StrataFlashWriCy1)
                            return false;

                        // read status register
                        rom->mode = StrataFlashReadStatus;
                        return true;

                    case 0x20:
                        if (rom->mode != StrataFlashNormal) return false;
                        rom->mode = StrataFlashErzCy1;
                        rom->opAddr = fromStart;
                        return true;

                    case 0x40:
                        if (rom->mode != StrataFlashNormal) return false;
                        rom->mode = StrataFlashWriCy1;
                        rom->opAddr = fromStart;
                        return true;

                    case 0xd0:
                        if (rom->mode != StrataFlashErzCy1) return false;
                        if (fromStart != rom->opAddr) return false;
                        fromStart /= STRATAFLASH_BLOCK_SIZE;
                        fromStart *= STRATAFLASH_BLOCK_SIZE;
                        if (!romPrvErase(rom, fromStart)) return false;
                        rom->busyCy = 0x1000;
                        rom->mode = StrataFlashReadStatus;
                        return true;

                    case 0x90:
                        if (rom->mode == StrataFlashErzCy1 || rom->mode == StrataFlashWriCy1)
                            return false;

                        // read identifier
                        rom->mode = StrataFlashReadID;
                        return true;

                    case 0x98:
                        if (rom->mode == StrataFlashErzCy1 || rom->mode == StrataFlashWriCy1)
                            return false;

                        // read query CFI
                        rom->mode = StrataFlashReadCFI;
                        return true;

                    case 0xff:
                        if (rom->mode == StrataFlashErzCy1 || rom->mode == StrataFlashWriCy1)
                            return false;

                        // read
                        rom->mode = StrataFlashNormal;
                        return true;

                    default:
                        fprintf(stderr, "Unknown strataflash command 0x%04x\n", cmd);
                        return false;
                }
        }
        switch (size) {
            case 1:

                *((uint8_t *)addr) = *(uint8_t *)bufP;  // our memory system is little-endian
                break;

            case 2:

                *((uint16_t *)addr) =
                    htole16(*(uint16_t *)bufP);  // our memory system is little-endian
                break;

            case 4:

                *((uint32_t *)addr) = htole32(*(uint32_t *)bufP);
                break;

            default:

                return false;
        }
    } else {
        // 128mbit reply
        static const uint16_t qryReplies_from_0x10[] = {
            'Q', 'R', 'Y',  1,   0,   0x31, 0,    0, 0, 0,    0, 0x27, 0x36, 0, 0, 8,    9,
            10,  0,   1,    1,   2,   0,    0x18, 1, 0, 6,    0, 1,    0x7f, 0, 0, 3,

            'P', 'R', 'I',  '1', '1', 0xe6, 1,    0, 0, 1,    7, 0,    0x33, 0, 2, 0x80, 0,
            3,   3,   0x89, 0,   0,   0,    0,    0, 0, 0x10, 0, 4,    4,    2, 2, 3

        };
        bool command = false;

        switch (rom->mode) {  // what modes expect a read of command size? which arent allowed at
                              // all

            case StrataFlashReadStatus:
            case StrataFlashReadID:
            case StrataFlashReadCFI:
                command = true;
                switch (rom->chipType) {
                    case RomStrataFlash16x:
                        if (size != 2) {
                            fprintf(stderr, "Strataflash read of improper size!\n");
                            return false;
                        }
                        fromStart /= 2;
                        break;

                    case RomStrataflash16x2x:
                        if (size != 4) {
                            fprintf(stderr, "StrataflashX2 read of improper size!\n");
                            return false;
                        }
                        fromStart /= 4;
                        break;

                    default:
                        return false;
                }
                break;

            case StrataFlashNormal:
            case StrataFlashSetReadConfigRegister:  // in this mode we can still fetch
                break;

            default:
                return false;
        }

        if (command) {
            bool skipdup = false;
            uint32_t reply;

            switch (rom->mode) {
                case StrataFlashReadStatus:
                    if (rom->busyCy) {
                        rom->busyCy--;
                        reply = 0;  // busy
                    } else {
                        rom->mode = StrataFlashNormal;  // only if not busy
                        reply = 0x0080;                 // ready;
                    }
                    break;

                case StrataFlashReadID:
                    switch (fromStart) {
                        case 0:
                            reply = 0x0089;
                            break;
                        case 1:
                            reply = 0x8802;
                            break;
                        case 5:
                            reply = rom->configReg;
                            break;
                        case 0x80:  // protection register lock
                            reply = 2;
                            break;

                        case 0x81:  // protection registers (uniq ID by intel and by manuf) copied
                                    // frpom same chip as this rom
                            reply = 0x001d0017ul;
                            skipdup = true;
                            break;

                        case 0x82:
                            reply = 0x000a0003ul;
                            skipdup = true;
                            break;

                        case 0x83:
                            reply = 0x3fb03fa6ul;
                            skipdup = true;
                            break;

                        case 0x84:
                            reply = 0x48d9c99aul;
                            skipdup = true;
                            break;

                        case 0x85 ... 0x88:
                            reply = 0xffff;
                            break;

                        case 0x89:  // otp lock - all locked fo rus
                            reply = 0;
                            break;
                        case 0x8a ... 0x109:  // otp data
                            reply = 0;
                            break;
                        default:
                            switch (fromStart & 0x7fff) {
                                case 0:  // id?
                                    reply = 0x80;
                                    fprintf(stderr,
                                            "strataflash weird read of 0x%08x in ID mode returns "
                                            "0x%04x\n",
                                            fromStart, reply);
                                    break;
                                case 2:  // block lock/lockdown
                                    reply = 0;
                                    break;
                                default:
                                    fprintf(stderr,
                                            "strataflash unknown read of 0x%08x in ID mode returns "
                                            "0xffff\n",
                                            fromStart);
                                    reply = 0xffff;
                                    break;
                            }
                            break;
                    }
                    break;

                case StrataFlashReadCFI:
                    fprintf(stderr, "CFI Read 0x%08x\n", fromStart);
                    switch (fromStart) {
                        case 0x00:
                            reply = 0x0089;
                            break;
                        case 0x01:
                            reply = 0x8802;
                            break;
                        case 0x10 ... sizeof(qryReplies_from_0x10) + 0x10:
                            reply = qryReplies_from_0x10[fromStart - 0x10];
                            break;
                        default:
                            switch (fromStart & 0xffff) {
                                case 2:  // block status register
                                    reply = 0;
                                    break;
                                default:
                                    return false;
                            }
                            break;
                    }
                    fprintf(stderr, "CFI Read 0x%08x -> 0x%04x\n", fromStart, reply);
                    break;

                default:
                    return false;
            }

            if (!skipdup) reply |= reply << 16;

            if (rom->chipType == RomStrataFlash16x)
                *(uint16_t *)bufP = reply;
            else
                *(uint32_t *)bufP = reply;

            return true;
        }
        switch (size) {
            case 1:

                *(uint8_t *)bufP = *((uint8_t *)addr);
                break;

            case 2:

                *(uint16_t *)bufP = le16toh(*((uint16_t *)addr));
                break;

            case 4:

                *(uint32_t *)bufP = le32toh(*((uint32_t *)addr));
                break;

            case 64:
                ((uint32_t *)bufP)[8] = le32toh(*((uint32_t *)(addr + 32)));
                ((uint32_t *)bufP)[9] = le32toh(*((uint32_t *)(addr + 36)));
                ((uint32_t *)bufP)[10] = le32toh(*((uint32_t *)(addr + 40)));
                ((uint32_t *)bufP)[11] = le32toh(*((uint32_t *)(addr + 44)));
                ((uint32_t *)bufP)[12] = le32toh(*((uint32_t *)(addr + 48)));
                ((uint32_t *)bufP)[13] = le32toh(*((uint32_t *)(addr + 52)));
                ((uint32_t *)bufP)[14] = le32toh(*((uint32_t *)(addr + 56)));
                ((uint32_t *)bufP)[15] = le32toh(*((uint32_t *)(addr + 60)));
                // fallthrough
            case 32:

                ((uint32_t *)bufP)[4] = le32toh(*((uint32_t *)(addr + 16)));
                ((uint32_t *)bufP)[5] = le32toh(*((uint32_t *)(addr + 20)));
                ((uint32_t *)bufP)[6] = le32toh(*((uint32_t *)(addr + 24)));
                ((uint32_t *)bufP)[7] = le32toh(*((uint32_t *)(addr + 28)));
                // fallthrough
            case 16:

                ((uint32_t *)bufP)[2] = le32toh(*((uint32_t *)(addr + 8)));
                ((uint32_t *)bufP)[3] = le32toh(*((uint32_t *)(addr + 12)));
                // fallthrough
            case 8:
                ((uint32_t *)bufP)[0] = le32toh(*((uint32_t *)(addr + 0)));
                ((uint32_t *)bufP)[1] = le32toh(*((uint32_t *)(addr + 4)));
                break;

            default:

                return false;
        }
    }

    return true;
}

struct ArmRom *romInit(struct ArmMem *mem, uint32_t adr, void **pieces, const uint32_t *pieceSizes,
                       uint32_t numPieces, enum RomChipType chipType) {
    struct ArmRom *rom = (struct ArmRom *)malloc(sizeof(*rom));
    struct ArmRomPiece *prev = NULL, *t, *piece = NULL;
    uint32_t i;

    if (!rom) ERR("cannot alloc ROM at 0x%08x", adr);

    memset(rom, 0, sizeof(*rom));

    if (numPieces > 1 && chipType != RomWriteIgnore && chipType != RomWriteError)
        ERR("piecewise roms cannot be writeable\n");

    rom->start = adr;

    for (i = 0; i < numPieces; i++) {
        piece = (struct ArmRomPiece *)malloc(sizeof(*piece));
        if (!piece) ERR("cannot alloc ROM piece at 0x%08x", adr);

        memset(piece, 0, sizeof(*piece));
        piece->next = prev;  // we'll reverse the list later

        if (adr & 0x1f) ERR("rom piece cannot start at 0x%08x\n", adr);

        piece->base = adr;
        piece->size = *pieceSizes++;
        piece->buf = (uint32_t *)*pieces++;
        piece->rom = rom;

        adr += piece->size;

        if (!memRegionAdd(mem, piece->base, piece->size, romAccessF, piece))
            ERR("cannot add ROM piece at 0x%08x to MEM\n", adr);
    }

    // we linked the list in reverse. fix this
    while (piece) {
        t = piece->next;
        piece->next = rom->pieces;
        rom->pieces = piece;
        piece = t;
    }

    rom->chipType = chipType;
    rom->mode = StrataFlashNormal;
    rom->configReg = 0xffc7;

    return rom;
}
