//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "pxa270_WMMX.h"

#include <stdlib.h>
#include <string.h>

#include "uarm_endian.h"
#include "util.h"

union REG64 {
    uint64_t v64;
    int64_t s64;
    uint32_t v32[2];
    int32_t s32[2];
    uint16_t v16[4];
    int16_t s16[4];
    uint8_t v8[8];
    int8_t s8[8];
};

struct Pxa270wmmx {
    union REG64 wR[16];
    uint32_t wCGR[4], wCASF;  // NZCV
    uint8_t wCon, wCSSF;
};

#define pxa270wmmxPrvDataRegsChanged(_wmmx) \
    do {                                    \
        _wmmx->wCon |= 2;                   \
    } while (0)
#define pxa270wmmxPrvControlRegsChanged(_wmmx) \
    do {                                       \
        _wmmx->wCon |= 1;                      \
    } while (0)

#if __BYTE_ORDER == __LITTLE_ENDIAN
    #define ACCESS_REG_8(_idx) (_idx)
    #define ACCESS_REG_16(_idx) (_idx)
    #define ACCESS_REG_32(_idx) (_idx)
#else
    #define ACCESS_REG_8(_idx) (7 - (_idx))
    #define ACCESS_REG_16(_idx) (3 - (_idx))
    #define ACCESS_REG_32(_idx) (1 - (_idx))
#endif

static void pxa270wmmxPrvSetFlagsForLogical64(struct Pxa270wmmx *wmmx, uint64_t val) {
    wmmx->wCASF = ((val >> 63) ? 0x80000000ul : 0) | (val ? 0 : 0x40000000ul);
    pxa270wmmxPrvControlRegsChanged(wmmx);
}

static void pxa270wmmxPrvAlign(struct Pxa270wmmx *wmmx, uint_fast8_t CRd, uint_fast8_t CRn,
                               uint_fast8_t CRm, uint_fast8_t by) {
    union REG64 ret;
    uint_fast8_t i;

    for (i = 0; i < 8 - by; i++) ret.v8[ACCESS_REG_8(i)] = wmmx->wR[CRn].v8[ACCESS_REG_8(i + by)];
    for (; i < 8; i++) ret.v8[ACCESS_REG_8(i)] = wmmx->wR[CRn].v8[ACCESS_REG_8(i - 8 + by)];
    wmmx->wR[CRd].v64 = ret.v64;
    pxa270wmmxPrvDataRegsChanged(wmmx);
}

static bool pxa270wmmxPrvDataProcessingMisc(struct Pxa270wmmx *wmmx, uint_fast8_t op1,
                                            uint_fast8_t CRd, uint_fast8_t CRn, uint_fast8_t CRm) {
    uint_fast8_t i, tf8;
    uint_fast16_t tf16;
    uint64_t tmp;

    switch (op1) {
        case 0b0000:  // WOR
            wmmx->wR[CRd].v64 = tmp = wmmx->wR[CRn].v64 | wmmx->wR[CRm].v64;
            pxa270wmmxPrvDataRegsChanged(wmmx);
            pxa270wmmxPrvSetFlagsForLogical64(wmmx, tmp);
            break;

        case 0b0001:  // WXOR
            wmmx->wR[CRd].v64 = tmp = wmmx->wR[CRn].v64 ^ wmmx->wR[CRm].v64;
            pxa270wmmxPrvDataRegsChanged(wmmx);
            pxa270wmmxPrvSetFlagsForLogical64(wmmx, tmp);
            break;

        case 0b0010:  // WAND
            wmmx->wR[CRd].v64 = tmp = wmmx->wR[CRn].v64 & wmmx->wR[CRm].v64;
            pxa270wmmxPrvDataRegsChanged(wmmx);
            pxa270wmmxPrvSetFlagsForLogical64(wmmx, tmp);
            break;

        case 0b0011:  // WANDN
            wmmx->wR[CRd].v64 = tmp = wmmx->wR[CRn].v64 & ~wmmx->wR[CRm].v64;
            pxa270wmmxPrvDataRegsChanged(wmmx);
            pxa270wmmxPrvSetFlagsForLogical64(wmmx, tmp);
            break;

        case 0b1000:  // WAVG2 (byte size)
        case 0b1001:
            wmmx->wCASF = 0;
            for (i = 0, tmp = 0x04; i < 8; i++, tmp <<= 4) {
                wmmx->wR[CRd].v8[ACCESS_REG_8(i)] = tf8 =
                    (wmmx->wR[CRn].v8[ACCESS_REG_8(i)] + wmmx->wR[CRm].v8[ACCESS_REG_8(i)] +
                     (op1 & 1)) /
                    2;
                if (!tf8) wmmx->wCASF |= tmp;
            }
            pxa270wmmxPrvControlRegsChanged(wmmx);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b1100:  // WAVG2 (halfword size)
        case 0b1101:
            wmmx->wCASF = 0;
            for (i = 0, tmp = 0x40; i < 4; i++, tmp <<= 8) {
                wmmx->wR[CRd].v16[ACCESS_REG_16(i)] = tf16 =
                    (wmmx->wR[CRn].v16[ACCESS_REG_16(i)] + wmmx->wR[CRm].v16[ACCESS_REG_16(i)] +
                     (op1 & 1)) /
                    2;
                if (!tf16) wmmx->wCASF |= tmp;
            }
            pxa270wmmxPrvControlRegsChanged(wmmx);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        default:
            return false;
    }
    return true;
}

static bool pxa270wmmxPrvDataProcessingAlign(struct Pxa270wmmx *wmmx, uint_fast8_t op1,
                                             uint_fast8_t CRd, uint_fast8_t CRn, uint_fast8_t CRm) {
    switch (op1) {
        case 0b0000:  // WALIGNI
        case 0b0001:
        case 0b0010:
        case 0b0011:
        case 0b0100:
        case 0b0101:
        case 0b0110:
        case 0b0111:
            pxa270wmmxPrvAlign(wmmx, CRd, CRn, CRm, (op1 & 7));
            break;

        case 0b1000:  // WALIGNR
        case 0b1001:
        case 0b1010:
        case 0b1011:
            pxa270wmmxPrvAlign(wmmx, CRd, CRn, CRm, wmmx->wCGR[op1 & 3] & 7);
            break;

        default:
            return false;
    }
    return true;
}

static bool pxa270wmmxPrvDataProcessingShift(struct Pxa270wmmx *wmmx, bool cp1, uint_fast8_t op1,
                                             uint_fast8_t CRd, uint_fast8_t CRn, uint_fast8_t CRm) {
    uint_fast8_t i, by;
    uint_fast16_t tf16;
    uint_fast32_t tf32;
    uint64_t tmp;

    switch (op1) {
        case 0b0100:  // WSRA.h
            if (!cp1)
                by = wmmx->wR[CRm].v8[ACCESS_REG_8(0)];
            else if (CRm >= 4)
                return false;
            else
                by = wmmx->wCGR[CRm];

            wmmx->wCASF = 0;
            for (i = 0; i < 4; i++) {
                wmmx->wR[CRd].v16[ACCESS_REG_16(i)] = tf16 =
                    wmmx->wR[CRn].s16[ACCESS_REG_16(i)] >> by;
                if (!tf16) wmmx->wCASF |= 0x40ul << (i * 8);
                if (tf16 & 0x8000) wmmx->wCASF |= 0x80ul << (i * 8);
            }
            pxa270wmmxPrvControlRegsChanged(wmmx);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b1000:  // WSRA.w
            if (!cp1)
                by = wmmx->wR[CRm].v8[ACCESS_REG_8(0)];
            else if (CRm >= 4)
                return false;
            else
                by = wmmx->wCGR[CRm];

            wmmx->wCASF = 0;
            for (i = 0; i < 2; i++) {
                wmmx->wR[CRd].v32[ACCESS_REG_32(i)] = tf32 =
                    wmmx->wR[CRn].s32[ACCESS_REG_32(i)] >> by;
                if (!tf32) wmmx->wCASF |= 0x4000ul << (i * 16);
                if (tf32 & 0x80000000ul) wmmx->wCASF |= 0x8000ul << (i * 16);
            }
            pxa270wmmxPrvControlRegsChanged(wmmx);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b1100:  // WSRA.d
            if (!cp1)
                by = wmmx->wR[CRm].v8[ACCESS_REG_8(0)];
            else if (CRm >= 4)
                return false;
            else
                by = wmmx->wCGR[CRm];

            wmmx->wR[CRd].v64 = tmp = wmmx->wR[CRn].s64 >> by;
            pxa270wmmxPrvSetFlagsForLogical64(wmmx, tmp);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b0101:  // WSLL.h
            if (!cp1)
                by = wmmx->wR[CRm].v8[ACCESS_REG_8(0)];
            else if (CRm >= 4)
                return false;
            else
                by = wmmx->wCGR[CRm];

            wmmx->wCASF = 0;
            for (i = 0; i < 4; i++) {
                wmmx->wR[CRd].v16[ACCESS_REG_16(i)] = tf16 = wmmx->wR[CRn].v16[ACCESS_REG_16(i)]
                                                             << by;
                if (!tf16) wmmx->wCASF |= 0x40ul << (i * 8);
                if (tf16 & 0x8000) wmmx->wCASF |= 0x80ul << (i * 8);
            }
            pxa270wmmxPrvControlRegsChanged(wmmx);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b1001:  // WSLL.w
            if (!cp1)
                by = wmmx->wR[CRm].v8[ACCESS_REG_8(0)];
            else if (CRm >= 4)
                return false;
            else
                by = wmmx->wCGR[CRm];

            wmmx->wCASF = 0;
            for (i = 0; i < 2; i++) {
                wmmx->wR[CRd].v32[ACCESS_REG_32(i)] = tf32 = wmmx->wR[CRn].v32[ACCESS_REG_32(i)]
                                                             << by;
                if (!tf32) wmmx->wCASF |= 0x4000ul << (i * 16);
                if (tf32 & 0x80000000ul) wmmx->wCASF |= 0x8000ul << (i * 16);
            }
            pxa270wmmxPrvControlRegsChanged(wmmx);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b1101:  // WSLL.d
            if (!cp1)
                by = wmmx->wR[CRm].v8[ACCESS_REG_8(0)];
            else if (CRm >= 4)
                return false;
            else
                by = wmmx->wCGR[CRm];

            wmmx->wR[CRd].v64 = tmp = wmmx->wR[CRn].v64 << by;
            pxa270wmmxPrvSetFlagsForLogical64(wmmx, tmp);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b0110:  // WSRL.h
            if (!cp1)
                by = wmmx->wR[CRm].v8[ACCESS_REG_8(0)];
            else if (CRm >= 4)
                return false;
            else
                by = wmmx->wCGR[CRm];

            wmmx->wCASF = 0;
            for (i = 0; i < 4; i++) {
                wmmx->wR[CRd].v16[ACCESS_REG_16(i)] = tf16 =
                    wmmx->wR[CRn].v16[ACCESS_REG_16(i)] >> by;
                if (!tf16) wmmx->wCASF |= 0x40ul << (i * 8);
                if (tf16 & 0x8000) wmmx->wCASF |= 0x80ul << (i * 8);
            }
            pxa270wmmxPrvControlRegsChanged(wmmx);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b1010:  // WSRL.w
            if (!cp1)
                by = wmmx->wR[CRm].v8[ACCESS_REG_8(0)];
            else if (CRm >= 4)
                return false;
            else
                by = wmmx->wCGR[CRm];

            wmmx->wCASF = 0;
            for (i = 0; i < 2; i++) {
                wmmx->wR[CRd].v32[ACCESS_REG_32(i)] = tf32 =
                    wmmx->wR[CRn].v32[ACCESS_REG_32(i)] >> by;
                if (!tf32) wmmx->wCASF |= 0x4000ul << (i * 16);
                if (tf32 & 0x80000000ul) wmmx->wCASF |= 0x8000ul << (i * 16);
            }
            pxa270wmmxPrvControlRegsChanged(wmmx);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b1110:  // WSRL.d
            if (!cp1)
                by = wmmx->wR[CRm].v8[ACCESS_REG_8(0)];
            else if (CRm >= 4)
                return false;
            else
                by = wmmx->wCGR[CRm];

            wmmx->wR[CRd].v64 = tmp = wmmx->wR[CRn].v64 >> by;
            pxa270wmmxPrvSetFlagsForLogical64(wmmx, tmp);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b0111:  // WROR.h
            if (!cp1)
                by = wmmx->wR[CRm].v8[ACCESS_REG_8(0)];
            else if (CRm >= 4)
                return false;
            else
                by = wmmx->wCGR[CRm];

            wmmx->wCASF = 0;
            for (i = 0; i < 4; i++) {
                if (by)
                    tf16 = (wmmx->wR[CRn].v16[ACCESS_REG_16(i)] >> by) |
                           (wmmx->wR[CRn].v16[ACCESS_REG_16(i)] << (16 - by));
                else
                    tf16 = wmmx->wR[CRn].v16[ACCESS_REG_16(i)];
                wmmx->wR[CRd].v16[ACCESS_REG_16(i)] = tf16;
                if (!tf16) wmmx->wCASF |= 0x40ul << (i * 8);
                if (tf16 & 0x8000) wmmx->wCASF |= 0x80ul << (i * 8);
            }
            pxa270wmmxPrvControlRegsChanged(wmmx);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b1011:  // WROR.w
            if (!cp1)
                by = wmmx->wR[CRm].v8[ACCESS_REG_8(0)];
            else if (CRm >= 4)
                return false;
            else
                by = wmmx->wCGR[CRm];

            wmmx->wCASF = 0;
            for (i = 0; i < 2; i++) {
                if (by)
                    tf32 = (wmmx->wR[CRn].v32[ACCESS_REG_32(i)] >> by) |
                           (wmmx->wR[CRn].v32[ACCESS_REG_32(i)] << (32 - by));
                else
                    tf32 = wmmx->wR[CRn].v32[ACCESS_REG_32(i)];
                wmmx->wR[CRd].v32[ACCESS_REG_32(i)] = tf32;
                if (!tf32) wmmx->wCASF |= 0x4000ul << (i * 16);
                if (tf32 & 0x80000000ul) wmmx->wCASF |= 0x8000ul << (i * 16);
            }
            pxa270wmmxPrvControlRegsChanged(wmmx);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b1111:  // WROR.d
            if (!cp1)
                by = wmmx->wR[CRm].v8[ACCESS_REG_8(0)];
            else if (CRm >= 4)
                return false;
            else
                by = wmmx->wCGR[CRm];

            if (by)
                tmp = (wmmx->wR[CRn].v64 >> by) | (wmmx->wR[CRn].v64 << (64 - by));
            else
                tmp = wmmx->wR[CRn].v64;
            wmmx->wR[CRd].v64 = tmp;
            pxa270wmmxPrvSetFlagsForLogical64(wmmx, tmp);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        default:
            return false;
    }
    return true;
}

static bool pxa270wmmxPrvDataProcessingCompare(struct Pxa270wmmx *wmmx, uint_fast8_t op1,
                                               uint_fast8_t CRd, uint_fast8_t CRn,
                                               uint_fast8_t CRm) {
    uint_fast8_t i, tf8;
    uint_fast16_t tf16;
    uint_fast32_t tf32;

    switch (op1) {
        case 0b0000:  // WCMPEQ.b
            wmmx->wCASF = 0;
            for (i = 0; i < 8; i++) {
                wmmx->wR[CRd].v8[ACCESS_REG_8(i)] = tf8 =
                    (wmmx->wR[CRn].v8[ACCESS_REG_8(i)] == wmmx->wR[CRm].v8[ACCESS_REG_8(i)]) ? 0xFF
                                                                                             : 0x00;
                if (tf8)
                    wmmx->wCASF |= 0x8ul << (i * 4);
                else
                    wmmx->wCASF |= 0x4ul << (i * 4);
            }
            pxa270wmmxPrvControlRegsChanged(wmmx);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b0100:  // WCMPEQ.h
            wmmx->wCASF = 0;
            for (i = 0; i < 4; i++) {
                wmmx->wR[CRd].v16[ACCESS_REG_16(i)] = tf16 =
                    (wmmx->wR[CRn].v16[ACCESS_REG_16(i)] == wmmx->wR[CRm].v16[ACCESS_REG_16(i)])
                        ? 0xFFFF
                        : 0x00;
                if (tf16)
                    wmmx->wCASF |= 0x80ul << (i * 8);
                else
                    wmmx->wCASF |= 0x40ul << (i * 8);
            }
            pxa270wmmxPrvControlRegsChanged(wmmx);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b1000:  // WCMPEQ.w
            wmmx->wCASF = 0;
            for (i = 0; i < 2; i++) {
                wmmx->wR[CRd].v32[ACCESS_REG_32(i)] = tf32 =
                    (wmmx->wR[CRn].v32[ACCESS_REG_32(i)] == wmmx->wR[CRm].v32[ACCESS_REG_32(i)])
                        ? 0xFFFFFFFFUL
                        : 0x00;
                if (tf32)
                    wmmx->wCASF |= 0x8000ul << (i * 16);
                else
                    wmmx->wCASF |= 0x4000ul << (i * 16);
            }
            pxa270wmmxPrvControlRegsChanged(wmmx);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b0001:  // WCMPGTU.b
            wmmx->wCASF = 0;
            for (i = 0; i < 8; i++) {
                wmmx->wR[CRd].v8[ACCESS_REG_8(i)] = tf8 =
                    (wmmx->wR[CRn].v8[ACCESS_REG_8(i)] > wmmx->wR[CRm].v8[ACCESS_REG_8(i)]) ? 0xFF
                                                                                            : 0x00;
                if (tf8)
                    wmmx->wCASF |= 0x8ul << (i * 4);
                else
                    wmmx->wCASF |= 0x4ul << (i * 4);
            }
            pxa270wmmxPrvControlRegsChanged(wmmx);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b0101:  // WCMPGTU.h
            wmmx->wCASF = 0;
            for (i = 0; i < 4; i++) {
                wmmx->wR[CRd].v16[ACCESS_REG_16(i)] = tf16 =
                    (wmmx->wR[CRn].v16[ACCESS_REG_16(i)] > wmmx->wR[CRm].v16[ACCESS_REG_16(i)])
                        ? 0xFFFF
                        : 0x00;
                if (tf16)
                    wmmx->wCASF |= 0x80ul << (i * 8);
                else
                    wmmx->wCASF |= 0x40ul << (i * 8);
            }
            pxa270wmmxPrvControlRegsChanged(wmmx);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b1001:  // WCMPGTU.w
            wmmx->wCASF = 0;
            for (i = 0; i < 2; i++) {
                wmmx->wR[CRd].v32[ACCESS_REG_32(i)] = tf32 =
                    (wmmx->wR[CRn].v32[ACCESS_REG_32(i)] > wmmx->wR[CRm].v32[ACCESS_REG_32(i)])
                        ? 0xFFFFFFFFUL
                        : 0x00;
                if (tf32)
                    wmmx->wCASF |= 0x8000ul << (i * 16);
                else
                    wmmx->wCASF |= 0x4000ul << (i * 16);
            }
            pxa270wmmxPrvControlRegsChanged(wmmx);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b0011:  // WCMPGTS.b
            wmmx->wCASF = 0;
            for (i = 0; i < 8; i++) {
                wmmx->wR[CRd].v8[ACCESS_REG_8(i)] = tf8 =
                    (wmmx->wR[CRn].s8[ACCESS_REG_8(i)] > wmmx->wR[CRm].s8[ACCESS_REG_8(i)]) ? 0xFF
                                                                                            : 0x00;
                if (tf8)
                    wmmx->wCASF |= 0x8ul << (i * 4);
                else
                    wmmx->wCASF |= 0x4ul << (i * 4);
            }
            pxa270wmmxPrvControlRegsChanged(wmmx);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b0111:  // WCMPGTS.h
            wmmx->wCASF = 0;
            for (i = 0; i < 4; i++) {
                wmmx->wR[CRd].v16[ACCESS_REG_16(i)] = tf16 =
                    (wmmx->wR[CRn].s16[ACCESS_REG_16(i)] > wmmx->wR[CRm].s16[ACCESS_REG_16(i)])
                        ? 0xFFFF
                        : 0x00;
                if (tf16)
                    wmmx->wCASF |= 0x80ul << (i * 8);
                else
                    wmmx->wCASF |= 0x40ul << (i * 8);
            }
            pxa270wmmxPrvControlRegsChanged(wmmx);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b1011:  // WCMPGTS.w
            wmmx->wCASF = 0;
            for (i = 0; i < 2; i++) {
                wmmx->wR[CRd].v32[ACCESS_REG_32(i)] = tf32 =
                    (wmmx->wR[CRn].s32[ACCESS_REG_32(i)] > wmmx->wR[CRm].s32[ACCESS_REG_32(i)])
                        ? 0xFFFFFFFFUL
                        : 0x00;
                if (tf32)
                    wmmx->wCASF |= 0x8000ul << (i * 16);
                else
                    wmmx->wCASF |= 0x4000ul << (i * 16);
            }
            pxa270wmmxPrvControlRegsChanged(wmmx);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        default:
            return false;
    }
    return true;
}

static bool pxa270wmmxPrvDataProcessingPack(struct Pxa270wmmx *wmmx, uint_fast8_t op1,
                                            uint_fast8_t CRd, uint_fast8_t CRn, uint_fast8_t CRm) {
    uint_fast8_t i, j, from;
    int_fast16_t ts16;
    int_fast32_t ts32;
    int_fast64_t ts64;
    union REG64 ret;

    switch (op1) {
        case 0b0101:  // WPACKUS.h
            wmmx->wCASF = 0;
            for (i = 0; i < 8; i++) {
                from = i < 4 ? CRn : CRm;
                j = i % 4;

                ts16 = wmmx->wR[from].s16[ACCESS_REG_16(j)];
                if (ts16 < 0) {
                    ts16 = 0;
                    wmmx->wCSSF |= 1 << i;
                    wmmx->wCASF |= 0x4ul << (i * 4);
                } else if (ts16 > 0xff) {
                    ts16 = 0xff;
                    wmmx->wCSSF |= 1 << i;
                    wmmx->wCASF |= 0x8ul << (i * 4);
                } else {
                    if (!ts16) wmmx->wCASF |= 0x4ul << (i * 4);
                    if (ts16 & 0x80) wmmx->wCASF |= 0x8ul << (i * 4);
                }

                ret.v8[ACCESS_REG_8(i)] = ts16;
            }
            wmmx->wR[CRd].v64 = ret.v64;
            pxa270wmmxPrvControlRegsChanged(wmmx);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b1001:  // WPACKUS.w
            wmmx->wCASF = 0;
            for (i = 0; i < 4; i++) {
                from = i < 2 ? CRn : CRm;
                j = i % 2;

                ts32 = wmmx->wR[from].s32[ACCESS_REG_32(j)];
                if (ts32 < 0) {
                    ts32 = 0;
                    wmmx->wCSSF |= 1 << (i * 2);
                    wmmx->wCASF |= 0x40ul << (i * 8);
                } else if (ts32 > 0xffffl) {
                    ts32 = 0xffff;
                    wmmx->wCSSF |= 1 << (i * 2);
                    wmmx->wCASF |= 0x80ul << (i * 8);
                } else {
                    if (!ts32) wmmx->wCASF |= 0x40ul << (i * 8);
                    if (ts32 & 0x8000u) wmmx->wCASF |= 0x80ul << (i * 8);
                }

                ret.v16[ACCESS_REG_16(i)] = ts32;
            }
            wmmx->wR[CRd].v64 = ret.v64;
            pxa270wmmxPrvControlRegsChanged(wmmx);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b1101:  // WPACKUS.d
            wmmx->wCASF = 0;
            for (i = 0; i < 2; i++) {
                ts64 = wmmx->wR[i ? CRm : CRn].s64;
                if (ts64 < 0) {
                    ts64 = 0;
                    wmmx->wCSSF |= 1 << (i * 4);
                    wmmx->wCASF |= 0x4000ul << (i * 16);
                } else if (ts64 > 0xffffffffll) {
                    ts64 = 0xffffffffll;
                    wmmx->wCSSF |= 1 << (i * 4);
                    wmmx->wCASF |= 0x8000ul << (i * 16);
                } else {
                    if (!ts64) wmmx->wCASF |= 0x4000ul << (i * 16);
                    if (ts64 & 0x800000ul) wmmx->wCASF |= 0x8000ul << (i * 16);
                }

                ret.v32[ACCESS_REG_32(i)] = ts64;
            }
            wmmx->wR[CRd].v64 = ret.v64;
            pxa270wmmxPrvControlRegsChanged(wmmx);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b0111:  // WPACKSS.h
            wmmx->wCASF = 0;
            for (i = 0; i < 8; i++) {
                from = i < 4 ? CRn : CRm;
                j = i % 4;

                ts16 = wmmx->wR[from].s16[ACCESS_REG_16(j)];
                if (ts16 < -0x80) {
                    ts16 = -0x80;
                    wmmx->wCSSF |= 1 << i;
                    wmmx->wCASF |= 0x8ul << (i * 4);
                } else if (ts16 > 0x7f) {
                    ts16 = 0x7f;
                    wmmx->wCSSF |= 1 << i;
                } else {
                    if (!ts16) wmmx->wCASF |= 0x4ul << (i * 4);
                    if (ts16 < 0) wmmx->wCASF |= 0x8ul << (i * 4);
                }

                ret.v8[ACCESS_REG_8(i)] = ts16;
            }
            wmmx->wR[CRd].v64 = ret.v64;
            pxa270wmmxPrvControlRegsChanged(wmmx);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b1011:  // WPACKSS.w
            wmmx->wCASF = 0;
            for (i = 0; i < 4; i++) {
                from = i < 2 ? CRn : CRm;
                j = i % 2;

                ts32 = wmmx->wR[from].s32[ACCESS_REG_32(j)];
                if (ts32 < -0x8000) {
                    ts32 = -0x8000;
                    wmmx->wCSSF |= 1 << (i * 2);
                    wmmx->wCASF |= 0x80ul << (i * 8);
                } else if (ts32 > 0x7fff) {
                    ts32 = 0x7fff;
                    wmmx->wCSSF |= 1 << (i * 2);
                } else {
                    if (!ts32) wmmx->wCASF |= 0x40ul << (i * 8);
                    if (ts32 < 0) wmmx->wCASF |= 0x80ul << (i * 8);
                }

                ret.v16[ACCESS_REG_16(i)] = ts32;
            }
            wmmx->wR[CRd].v64 = ret.v64;
            pxa270wmmxPrvControlRegsChanged(wmmx);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b1111:  // WPACKSS.d
            wmmx->wCASF = 0;
            for (i = 0; i < 2; i++) {
                ts64 = wmmx->wR[i ? CRm : CRn].s64;
                if (ts64 < -0x80000000ll) {
                    ts64 = -0x80000000ll;
                    wmmx->wCSSF |= 1 << (i * 4);
                    wmmx->wCASF |= 0x8000ul << (i * 16);
                } else if (ts64 > 0x7fffffffll) {
                    ts64 = 0x7fffffffll;
                    wmmx->wCSSF |= 1 << (i * 4);
                } else {
                    if (!ts64) wmmx->wCASF |= 0x4000ul << (i * 16);
                    if (ts64 < 0) wmmx->wCASF |= 0x8000ul << (i * 16);
                }

                ret.v32[ACCESS_REG_32(i)] = ts64;
            }
            wmmx->wR[CRd].v64 = ret.v64;
            pxa270wmmxPrvControlRegsChanged(wmmx);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        default:
            return false;
    }
    return true;
}

static bool pxa270wmmxPrvDataProcessingUnpack(struct Pxa270wmmx *wmmx, bool hi, uint_fast8_t op1,
                                              uint_fast8_t CRd, uint_fast8_t CRn,
                                              uint_fast8_t CRm) {
    uint_fast8_t i, tf8;
    uint_fast32_t tf32;
    uint_fast16_t tf16;
    int_fast64_t ts64;
    int_fast32_t ts32;
    int_fast16_t ts16;
    union REG64 ret;

    switch (op1) {
        case 0b0010:  // WUNPACKESx.b
            wmmx->wCASF = 0;
            for (i = 0; i < 4; i++) {
                ret.s16[ACCESS_REG_16(i)] = ts16 = wmmx->wR[CRn].s8[ACCESS_REG_8(i + (hi ? 4 : 0))];
                if (!ts16) wmmx->wCASF |= 0x40ul << (i * 8);
                if (ts16 < 0) wmmx->wCASF |= 0x80ul << (i * 8);
            }
            wmmx->wR[CRd].v64 = ret.v64;
            pxa270wmmxPrvControlRegsChanged(wmmx);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b0110:  // WUNPACKESx.h
            wmmx->wCASF = 0;
            for (i = 0; i < 2; i++) {
                ret.s32[ACCESS_REG_32(i)] = ts32 =
                    wmmx->wR[CRn].s16[ACCESS_REG_16(i + (hi ? 2 : 0))];
                if (!ts32) wmmx->wCASF |= 0x4000ul << (i * 16);
                if (ts32 < 0) wmmx->wCASF |= 0x8000ul << (i * 16);
            }
            wmmx->wR[CRd].v64 = ret.v64;
            pxa270wmmxPrvControlRegsChanged(wmmx);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b1010:  // WUNPACKESx.w
            wmmx->wCASF = 0;
            wmmx->wR[CRd].s64 = ts64 = wmmx->wR[CRn].s32[ACCESS_REG_32(hi ? 1 : 0)];
            if (!ts64) wmmx->wCASF |= 0x40000000ul;
            if (ts64 < 0) wmmx->wCASF |= 0x80000000ul;
            pxa270wmmxPrvControlRegsChanged(wmmx);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b0000:  // WUNPACKEUx.b
            wmmx->wCASF = 0;
            for (i = 0; i < 4; i++) {
                ret.v16[ACCESS_REG_16(i)] = tf8 = wmmx->wR[CRn].v8[ACCESS_REG_8(i + (hi ? 4 : 0))];
                if (!tf8) wmmx->wCASF |= 0x40ul << (i * 8);
            }
            wmmx->wR[CRd].v64 = ret.v64;
            pxa270wmmxPrvControlRegsChanged(wmmx);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b0100:  // WUNPACKEUx.h
            wmmx->wCASF = 0;
            for (i = 0; i < 2; i++) {
                ret.v32[ACCESS_REG_32(i)] = tf16 =
                    wmmx->wR[CRn].v16[ACCESS_REG_16(i + (hi ? 2 : 0))];
                if (!tf16) wmmx->wCASF |= 0x4000ul << (i * 16);
            }
            wmmx->wR[CRd].v64 = ret.v64;
            pxa270wmmxPrvControlRegsChanged(wmmx);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b1000:  // WUNPACKEUx.w
            wmmx->wCASF = 0;
            wmmx->wR[CRd].v64 = tf32 = wmmx->wR[CRn].v32[ACCESS_REG_32(hi ? 1 : 0)];
            if (!tf32) wmmx->wCASF |= 0x40000000ul;
            pxa270wmmxPrvControlRegsChanged(wmmx);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b0001:  // WUNPACKIx.b
            wmmx->wCASF = 0;
            for (i = 0; i < 8; i++) {
                ret.v8[ACCESS_REG_8(i)] = tf8 =
                    wmmx->wR[(i & 1) ? CRm : CRn].v8[ACCESS_REG_8(i / 2 + (hi ? 4 : 0))];
                if (!tf8) wmmx->wCASF |= 0x4ul << (i * 4);
                if (tf8 & 0x80) wmmx->wCASF |= 0x8ul << (i * 4);
            }
            wmmx->wR[CRd].v64 = ret.v64;
            pxa270wmmxPrvControlRegsChanged(wmmx);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b0101:  // WUNPACKIx.h
            wmmx->wCASF = 0;
            for (i = 0; i < 4; i++) {
                ret.v16[ACCESS_REG_16(i)] = tf16 =
                    wmmx->wR[(i & 1) ? CRm : CRn].v16[ACCESS_REG_16(i / 2 + (hi ? 2 : 0))];
                if (!tf16) wmmx->wCASF |= 0x40ul << (i * 8);
                if (tf16 & 0x8000) wmmx->wCASF |= 0x80ul << (i * 8);
            }
            wmmx->wR[CRd].v64 = ret.v64;
            pxa270wmmxPrvControlRegsChanged(wmmx);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b1001:  // WUNPACKIx.w
            wmmx->wCASF = 0;
            for (i = 0; i < 2; i++) {
                ret.v32[ACCESS_REG_32(i)] = tf32 =
                    wmmx->wR[(i & 1) ? CRm : CRn].v32[ACCESS_REG_32(hi ? 1 : 0)];
                if (!tf32) wmmx->wCASF |= 0x4000ul << (i * 16);
                if (tf32 & 0x80000000ul) wmmx->wCASF |= 0x8000ul << (i * 16);
            }
            wmmx->wR[CRd].v64 = ret.v64;
            pxa270wmmxPrvControlRegsChanged(wmmx);
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        default:
            return false;
    }
    return true;
}

static bool pxa270wmmxPrvDataProcessingMultiply(struct Pxa270wmmx *wmmx, uint_fast8_t op1,
                                                uint_fast8_t CRd, uint_fast8_t CRn,
                                                uint_fast8_t CRm) {
    uint64_t sum = wmmx->wR[CRd].v64;
    uint_fast8_t i;

    switch (op1) {
        case 0b0000:  // WMULUL		//When L is specified the U and S qualifiers produce the
                      // same result
        case 0b0010:  // WMULSL
            for (i = 0; i < 4; i++)
                wmmx->wR[CRd].v16[ACCESS_REG_16(i)] =
                    wmmx->wR[CRn].v16[ACCESS_REG_16(i)] * wmmx->wR[CRm].v16[ACCESS_REG_16(i)];
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b0001:  // WMULUM
            for (i = 0; i < 4; i++)
                wmmx->wR[CRd].v16[ACCESS_REG_16(i)] =
                    ((uint_fast32_t)wmmx->wR[CRn].v16[ACCESS_REG_16(i)] *
                     (uint_fast32_t)wmmx->wR[CRm].v16[ACCESS_REG_16(i)]) >>
                    16;
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b0011:  // WMULSM
            for (i = 0; i < 4; i++)
                wmmx->wR[CRd].s16[ACCESS_REG_16(i)] =
                    ((int_fast32_t)wmmx->wR[CRn].s16[ACCESS_REG_16(i)] *
                     (int_fast32_t)wmmx->wR[CRm].s16[ACCESS_REG_16(i)]) >>
                    16;
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b0101:  // WMACUZ
            sum = 0;
            // fallthrough
        case 0b0100:  // WMACU
            for (i = 0; i < 4; i++) {
                sum += (uint_fast32_t)wmmx->wR[CRn].v16[ACCESS_REG_16(i)] *
                       (uint_fast32_t)wmmx->wR[CRm].v16[ACCESS_REG_16(i)];
            }
            wmmx->wR[CRd].v64 = sum;
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b0111:  // WMACSZ
            sum = 0;
            // fallthrough
        case 0b0110:  // WMACS
            for (i = 0; i < 4; i++) {
                sum += (int64_t)((int_fast32_t)wmmx->wR[CRn].s16[ACCESS_REG_16(i)] *
                                 (int_fast32_t)wmmx->wR[CRm].s16[ACCESS_REG_16(i)]);
            }
            wmmx->wR[CRd].v64 = sum;
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b1000:  // WMADDU
            for (i = 0; i < 2; i++) {
                wmmx->wR[CRd].v32[ACCESS_REG_32(i)] =
                    (uint_fast32_t)wmmx->wR[CRn].v16[ACCESS_REG_16(i * 2 + 0)] *
                        (uint_fast32_t)wmmx->wR[CRm].v16[ACCESS_REG_16(i * 2 + 0)] +
                    (uint_fast32_t)wmmx->wR[CRn].v16[ACCESS_REG_16(i * 2 + 1)] *
                        (uint_fast32_t)wmmx->wR[CRm].v16[ACCESS_REG_16(i * 2 + 1)];
            }
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b1010:  // WMADDS
            for (i = 0; i < 2; i++) {
                wmmx->wR[CRd].s32[ACCESS_REG_32(i)] =
                    (int_fast32_t)wmmx->wR[CRn].s16[ACCESS_REG_16(i * 2 + 0)] *
                        (int_fast32_t)wmmx->wR[CRm].s16[ACCESS_REG_16(i * 2 + 0)] +
                    (int_fast32_t)wmmx->wR[CRn].s16[ACCESS_REG_16(i * 2 + 1)] *
                        (int_fast32_t)wmmx->wR[CRm].s16[ACCESS_REG_16(i * 2 + 1)];
            }
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        default:
            return false;
    }
    return true;
}

static bool pxa270wmmxPrvDataProcessingDifference(struct Pxa270wmmx *wmmx, uint_fast8_t op1,
                                                  uint_fast8_t CRd, uint_fast8_t CRn,
                                                  uint_fast8_t CRm) {
    uint_fast32_t sum = wmmx->wR[CRd].v32[ACCESS_REG_32(0)];
    uint_fast8_t i, t;

    switch (op1) {
        case 0b0001:  // WSADBZ
            sum = 0;
            // fallthrough
        case 0b0000:  // WSADB
            for (i = 0; i < 8; i++) {
                t = wmmx->wR[CRn].v8[ACCESS_REG_8(i)] - wmmx->wR[CRm].v8[ACCESS_REG_8(i)];
                if (t & 0x80) t = -t;
                sum += (uint8_t)t;
            }
            wmmx->wR[CRd].v32[ACCESS_REG_32(1)] = 0;
            wmmx->wR[CRd].v32[ACCESS_REG_32(0)] = sum;
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b0101:  // WSADHZ
            sum = 0;
            // fallthrough
        case 0b0100:  // WSADH
            for (i = 0; i < 4; i++) {
                t = wmmx->wR[CRn].v16[ACCESS_REG_16(i)] - wmmx->wR[CRm].v16[ACCESS_REG_16(i)];
                if (t & 0x8000) t = -t;
                sum += (uint16_t)t;
            }
            wmmx->wR[CRd].v32[ACCESS_REG_32(1)] = 0;
            wmmx->wR[CRd].v32[ACCESS_REG_32(0)] = sum;
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        default:
            return false;
    }
    return true;
}

static bool pxa270wmmxPrvDataProcessingMinMax(struct Pxa270wmmx *wmmx, uint_fast8_t op1,
                                              uint_fast8_t CRd, uint_fast8_t CRn,
                                              uint_fast8_t CRm) {
    uint_fast8_t i;

    switch (op1) {
        case 0b0000:  // WMAXUB
            for (i = 0; i < 8; i++)
                wmmx->wR[CRd].v8[ACCESS_REG_8(i)] =
                    wmmx->wR[CRn].v8[ACCESS_REG_8(i)] > wmmx->wR[CRm].v8[ACCESS_REG_8(i)]
                        ? wmmx->wR[CRn].v8[ACCESS_REG_8(i)]
                        : wmmx->wR[CRm].v8[ACCESS_REG_8(i)];
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b0100:  // WMAXUH
            for (i = 0; i < 4; i++)
                wmmx->wR[CRd].v16[ACCESS_REG_16(i)] =
                    wmmx->wR[CRn].v16[ACCESS_REG_16(i)] > wmmx->wR[CRm].v16[ACCESS_REG_16(i)]
                        ? wmmx->wR[CRn].v16[ACCESS_REG_16(i)]
                        : wmmx->wR[CRm].v16[ACCESS_REG_16(i)];
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b1000:  // WMAXUW
            for (i = 0; i < 2; i++)
                wmmx->wR[CRd].v32[ACCESS_REG_32(i)] =
                    wmmx->wR[CRn].v32[ACCESS_REG_32(i)] > wmmx->wR[CRm].v32[ACCESS_REG_32(i)]
                        ? wmmx->wR[CRn].v32[ACCESS_REG_32(i)]
                        : wmmx->wR[CRm].v32[ACCESS_REG_32(i)];
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b0010:  // WMAXSB
            for (i = 0; i < 8; i++)
                wmmx->wR[CRd].s8[ACCESS_REG_8(i)] =
                    wmmx->wR[CRn].s8[ACCESS_REG_8(i)] > wmmx->wR[CRm].s8[ACCESS_REG_8(i)]
                        ? wmmx->wR[CRn].s8[ACCESS_REG_8(i)]
                        : wmmx->wR[CRm].s8[ACCESS_REG_8(i)];
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b0110:  // WMAXSH
            for (i = 0; i < 4; i++)
                wmmx->wR[CRd].s16[ACCESS_REG_16(i)] =
                    wmmx->wR[CRn].s16[ACCESS_REG_16(i)] > wmmx->wR[CRm].s16[ACCESS_REG_16(i)]
                        ? wmmx->wR[CRn].s16[ACCESS_REG_16(i)]
                        : wmmx->wR[CRm].s16[ACCESS_REG_16(i)];
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b1010:  // WMAXSW
            for (i = 0; i < 2; i++)
                wmmx->wR[CRd].s32[ACCESS_REG_32(i)] =
                    wmmx->wR[CRn].s32[ACCESS_REG_32(i)] > wmmx->wR[CRm].s32[ACCESS_REG_32(i)]
                        ? wmmx->wR[CRn].s32[ACCESS_REG_32(i)]
                        : wmmx->wR[CRm].s32[ACCESS_REG_32(i)];
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b0001:  // WMINUB
            for (i = 0; i < 8; i++)
                wmmx->wR[CRd].v8[ACCESS_REG_8(i)] =
                    wmmx->wR[CRn].v8[ACCESS_REG_8(i)] < wmmx->wR[CRm].v8[ACCESS_REG_8(i)]
                        ? wmmx->wR[CRn].v8[ACCESS_REG_8(i)]
                        : wmmx->wR[CRm].v8[ACCESS_REG_8(i)];
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b0101:  // WMINUH
            for (i = 0; i < 4; i++)
                wmmx->wR[CRd].v16[ACCESS_REG_16(i)] =
                    wmmx->wR[CRn].v16[ACCESS_REG_16(i)] < wmmx->wR[CRm].v16[ACCESS_REG_16(i)]
                        ? wmmx->wR[CRn].v16[ACCESS_REG_16(i)]
                        : wmmx->wR[CRm].v16[ACCESS_REG_16(i)];
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b1001:  // WMINUW
            for (i = 0; i < 2; i++)
                wmmx->wR[CRd].v32[ACCESS_REG_32(i)] =
                    wmmx->wR[CRn].v32[ACCESS_REG_32(i)] < wmmx->wR[CRm].v32[ACCESS_REG_32(i)]
                        ? wmmx->wR[CRn].v32[ACCESS_REG_32(i)]
                        : wmmx->wR[CRm].v32[ACCESS_REG_32(i)];
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b0011:  // WMINSB
            for (i = 0; i < 8; i++)
                wmmx->wR[CRd].s8[ACCESS_REG_8(i)] =
                    wmmx->wR[CRn].s8[ACCESS_REG_8(i)] < wmmx->wR[CRm].s8[ACCESS_REG_8(i)]
                        ? wmmx->wR[CRn].s8[ACCESS_REG_8(i)]
                        : wmmx->wR[CRm].s8[ACCESS_REG_8(i)];
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b0111:  // WMINSH
            for (i = 0; i < 4; i++)
                wmmx->wR[CRd].s16[ACCESS_REG_16(i)] =
                    wmmx->wR[CRn].s16[ACCESS_REG_16(i)] < wmmx->wR[CRm].s16[ACCESS_REG_16(i)]
                        ? wmmx->wR[CRn].s16[ACCESS_REG_16(i)]
                        : wmmx->wR[CRm].s16[ACCESS_REG_16(i)];
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b1011:  // WMINSW
            for (i = 0; i < 2; i++)
                wmmx->wR[CRd].s32[ACCESS_REG_32(i)] =
                    wmmx->wR[CRn].s32[ACCESS_REG_32(i)] < wmmx->wR[CRm].s32[ACCESS_REG_32(i)]
                        ? wmmx->wR[CRn].s32[ACCESS_REG_32(i)]
                        : wmmx->wR[CRm].s32[ACCESS_REG_32(i)];
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        default:
            return false;
    }
    return true;
}

static bool pxa270wmmxPrvDataProcessingAccumulate(struct Pxa270wmmx *wmmx, uint_fast8_t op1,
                                                  uint_fast8_t CRd, uint_fast8_t CRn,
                                                  uint_fast8_t CRm) {
    uint64_t sum = 0;
    uint_fast8_t i;

    switch (op1) {
        case 0b0000:  // WACC.b
            for (i = 0; i < 8; i++) sum += wmmx->wR[CRn].v8[ACCESS_REG_8(i)];
            wmmx->wR[CRd].v64 = sum;
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b0100:  // WACC.h
            for (i = 0; i < 4; i++) sum += wmmx->wR[CRn].v16[ACCESS_REG_16(i)];
            wmmx->wR[CRd].v64 = sum;
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        case 0b1000:  // WACC.w
            for (i = 0; i < 2; i++) sum += wmmx->wR[CRn].v32[ACCESS_REG_32(i)];
            wmmx->wR[CRd].v64 = sum;
            pxa270wmmxPrvDataRegsChanged(wmmx);
            break;

        default:
            return false;
    }
    return true;
}

static bool pxa270wmmxPrvDataProcessingShuffle(struct Pxa270wmmx *wmmx, uint_fast8_t op1,
                                               uint_fast8_t CRd, uint_fast8_t CRn,
                                               uint_fast8_t CRm) {
    uint_fast8_t i, which = (op1 << 4) + CRm;
    uint_fast16_t tf16;
    union REG64 ret;

    wmmx->wCASF = 0;
    for (i = 0; i < 4; i++, which >>= 2) {
        ret.v16[ACCESS_REG_16(i)] = tf16 = wmmx->wR[CRn].v16[ACCESS_REG_16(which & 3)];
        if (!tf16) wmmx->wCASF |= 0x40ul << (i * 8);
        if (tf16 & 0x8000) wmmx->wCASF |= 0x80ul << (i * 8);
    }
    wmmx->wR[CRd].v64 = ret.v64;
    pxa270wmmxPrvControlRegsChanged(wmmx);
    pxa270wmmxPrvDataRegsChanged(wmmx);
    return true;
}

static bool pxa270wmmxPrvDataProcessingAddition(struct Pxa270wmmx *wmmx, uint_fast8_t op1,
                                                uint_fast8_t CRd, uint_fast8_t CRn,
                                                uint_fast8_t CRm) {
    uint_fast8_t i;
    int_fast16_t sf16;
    int_fast32_t sf32;
    int_fast64_t sf64;

    wmmx->wCASF = 0;
    switch (op1) {
        case 0b0000:  // WADD.b
            for (i = 0; i < 8; i++) {
                wmmx->wR[CRd].s8[ACCESS_REG_8(i)] = sf16 =
                    (int_fast16_t)wmmx->wR[CRn].s8[ACCESS_REG_8(i)] +
                    (int_fast16_t)wmmx->wR[CRm].s8[ACCESS_REG_8(i)];
                if (((int8_t)sf16) < 0) wmmx->wCASF |= 0x8ul << (i * 4);
                if (!(int8_t)sf16) wmmx->wCASF |= 0x4ul << (i * 4);
                if (sf16 & 0x0100) wmmx->wCASF |= 0x2ul << (i * 4);
                sf16 >>= 7;
                sf16 &= 3;
                if (sf16 != 0 && sf16 != 3) wmmx->wCASF |= 0x1ul << (i * 4);
            }
            break;

        case 0b0100:  // WADD.h
            for (i = 0; i < 4; i++) {
                wmmx->wR[CRd].s16[ACCESS_REG_16(i)] = sf32 =
                    (int_fast32_t)wmmx->wR[CRn].s16[ACCESS_REG_16(i)] +
                    (int_fast32_t)wmmx->wR[CRm].s16[ACCESS_REG_16(i)];
                if (((int16_t)sf32) < 0) wmmx->wCASF |= 0x80ul << (i * 8);
                if (!(int16_t)sf32) wmmx->wCASF |= 0x40ul << (i * 8);
                if (sf32 & 0x010000l) wmmx->wCASF |= 0x20ul << (i * 8);
                sf32 >>= 15;
                sf32 &= 3;
                if (sf32 != 0 && sf32 != 3) wmmx->wCASF |= 0x10ul << (i * 8);
            }
            break;

        case 0b1000:  // WADD.w
            for (i = 0; i < 2; i++) {
                wmmx->wR[CRd].s32[ACCESS_REG_32(i)] = sf64 =
                    (int_fast64_t)wmmx->wR[CRn].s32[ACCESS_REG_32(i)] +
                    (int_fast64_t)wmmx->wR[CRm].s32[ACCESS_REG_32(i)];
                if (((int32_t)sf64) < 0) wmmx->wCASF |= 0x8000ul << (i * 16);
                if (!(int32_t)sf64) wmmx->wCASF |= 0x4000ul << (i * 16);
                if (sf64 & 0x0100000000ll) wmmx->wCASF |= 0x2000ul << (i * 16);
                sf64 >>= 31;
                sf64 &= 3;
                if (sf64 != 0 && sf64 != 3) wmmx->wCASF |= 0x1000ul << (i * 16);
            }
            break;

        case 0b0001:  // WADDUS.b
            for (i = 0; i < 8; i++) {
                wmmx->wR[CRd].s8[ACCESS_REG_8(i)] = sf16 =
                    (int_fast16_t)wmmx->wR[CRn].s8[ACCESS_REG_8(i)] +
                    (int_fast16_t)wmmx->wR[CRm].s8[ACCESS_REG_8(i)];

                if (sf16 >> 8) {
                    wmmx->wR[CRd].s8[ACCESS_REG_8(i)] = 0xff;
                    wmmx->wCASF |= 0x8ul << (i * 4);
                    wmmx->wCSSF |= 1 << i;
                } else {
                    if (((int8_t)sf16) < 0) wmmx->wCASF |= 0x8ul << (i * 4);
                    if (!(int8_t)sf16) wmmx->wCASF |= 0x4ul << (i * 4);
                    if (sf16 & 0x0100) wmmx->wCASF |= 0x2ul << (i * 4);
                    sf16 >>= 7;
                    sf16 &= 3;
                    if (sf16 != 0 && sf16 != 3) wmmx->wCASF |= 0x1ul << (i * 4);
                }
            }
            break;

        case 0b0101:  // WADDUS.h
            for (i = 0; i < 4; i++) {
                wmmx->wR[CRd].s16[ACCESS_REG_16(i)] = sf32 =
                    (int_fast32_t)wmmx->wR[CRn].s16[ACCESS_REG_16(i)] +
                    (int_fast32_t)wmmx->wR[CRm].s16[ACCESS_REG_16(i)];

                if (sf32 >> 16) {
                    wmmx->wR[CRd].s16[ACCESS_REG_16(i)] = 0xffff;
                    wmmx->wCASF |= 0x80ul << (i * 8);
                    wmmx->wCSSF |= 1 << (2 * i);
                } else {
                    if (((int16_t)sf32) < 0) wmmx->wCASF |= 0x80ul << (i * 8);
                    if (!(int16_t)sf32) wmmx->wCASF |= 0x40ul << (i * 8);
                    if (sf32 & 0x010000l) wmmx->wCASF |= 0x20ul << (i * 8);
                    sf32 >>= 15;
                    sf32 &= 3;
                    if (sf32 != 0 && sf32 != 3) wmmx->wCASF |= 0x10ul << (i * 8);
                }
            }
            break;

        case 0b1001:  // WADDUS.w
            for (i = 0; i < 2; i++) {
                wmmx->wR[CRd].s32[ACCESS_REG_32(i)] = sf64 =
                    (int_fast64_t)wmmx->wR[CRn].s32[ACCESS_REG_32(i)] +
                    (int_fast64_t)wmmx->wR[CRm].s32[ACCESS_REG_32(i)];

                if (sf64 >> 32) {
                    wmmx->wR[CRd].s32[ACCESS_REG_32(i)] = 0xffffffl;
                    wmmx->wCASF |= 0x8000ul << (i * 16);
                    wmmx->wCSSF |= 1 << (4 * i);
                } else {
                    if (((int32_t)sf64) < 0) wmmx->wCASF |= 0x8000ul << (i * 16);
                    if (!(int32_t)sf64) wmmx->wCASF |= 0x4000ul << (i * 16);
                    if (sf64 & 0x0100000000ll) wmmx->wCASF |= 0x2000ul << (i * 16);
                    sf64 >>= 31;
                    sf64 &= 3;
                    if (sf64 != 0 && sf64 != 3) wmmx->wCASF |= 0x1000ul << (i * 16);
                }
            }
            break;

        case 0b0011:  // WADDSS.b
            for (i = 0; i < 8; i++) {
                wmmx->wR[CRd].s8[ACCESS_REG_8(i)] = sf16 =
                    (int_fast16_t)wmmx->wR[CRn].s8[ACCESS_REG_8(i)] +
                    (int_fast16_t)wmmx->wR[CRm].s8[ACCESS_REG_8(i)];

                if (sf16 > 0x7f) {
                    wmmx->wR[CRd].s8[ACCESS_REG_8(i)] = 0x7f;
                    wmmx->wCSSF |= 1 << i;
                } else if (sf16 < -0x80) {
                    wmmx->wR[CRd].s8[ACCESS_REG_8(i)] = -0x80;
                    wmmx->wCASF |= 0x8ul << (i * 4);
                    wmmx->wCSSF |= 1 << i;
                } else {
                    if (((int8_t)sf16) < 0) wmmx->wCASF |= 0x8ul << (i * 4);
                    if (!(int8_t)sf16) wmmx->wCASF |= 0x4ul << (i * 4);
                    if (sf16 & 0x0100) wmmx->wCASF |= 0x2ul << (i * 4);
                    sf16 >>= 7;
                    sf16 &= 3;
                    if (sf16 != 0 && sf16 != 3) wmmx->wCASF |= 0x1ul << (i * 4);
                }
            }
            break;

        case 0b0111:  // WADDSS.h
            for (i = 0; i < 4; i++) {
                wmmx->wR[CRd].s16[ACCESS_REG_16(i)] = sf32 =
                    (int_fast32_t)wmmx->wR[CRn].s16[ACCESS_REG_16(i)] +
                    (int_fast32_t)wmmx->wR[CRm].s16[ACCESS_REG_16(i)];

                if (sf32 > 0x7fff) {
                    wmmx->wR[CRd].s16[ACCESS_REG_16(i)] = 0x7fff;
                    wmmx->wCSSF |= 1 << (2 * i);
                } else if (sf32 < -0x8000) {
                    wmmx->wR[CRd].s16[ACCESS_REG_16(i)] = -0x8000;
                    wmmx->wCASF |= 0x80ul << (i * 8);
                    wmmx->wCSSF |= 1 << (2 * i);
                } else {
                    if (((int16_t)sf32) < 0) wmmx->wCASF |= 0x80ul << (i * 8);
                    if (!(int16_t)sf32) wmmx->wCASF |= 0x40ul << (i * 8);
                    if (sf32 & 0x010000l) wmmx->wCASF |= 0x20ul << (i * 8);
                    sf32 >>= 15;
                    sf32 &= 3;
                    if (sf32 != 0 && sf32 != 3) wmmx->wCASF |= 0x10ul << (i * 8);
                }
            }
            break;

        case 0b1011:  // WADDSS.w
            for (i = 0; i < 2; i++) {
                wmmx->wR[CRd].s32[ACCESS_REG_32(i)] = sf64 =
                    (int_fast64_t)wmmx->wR[CRn].s32[ACCESS_REG_32(i)] +
                    (int_fast64_t)wmmx->wR[CRm].s32[ACCESS_REG_32(i)];

                if (sf64 > 0x7fffffffl) {
                    wmmx->wR[CRd].s32[ACCESS_REG_32(i)] = 0x7fffffffl;
                    wmmx->wCSSF |= 1 << (4 * i);
                } else if (sf64 < -0x80000000l) {
                    wmmx->wR[CRd].s32[ACCESS_REG_32(i)] = -0x80000000l;
                    wmmx->wCASF |= 0x8000ul << (i * 16);
                    wmmx->wCSSF |= 1 << (4 * i);
                } else {
                    if (((int32_t)sf64) < 0) wmmx->wCASF |= 0x8000ul << (i * 16);
                    if (!(int32_t)sf64) wmmx->wCASF |= 0x4000ul << (i * 16);
                    if (sf64 & 0x0100000000ll) wmmx->wCASF |= 0x2000ul << (i * 16);
                    sf64 >>= 31;
                    sf64 &= 3;
                    if (sf64 != 0 && sf64 != 3) wmmx->wCASF |= 0x1000ul << (i * 16);
                }
            }
            break;

        default:
            return false;
    }
    pxa270wmmxPrvControlRegsChanged(wmmx);
    pxa270wmmxPrvDataRegsChanged(wmmx);
    return true;
}

static bool pxa270wmmxPrvDataProcessingSubtraction(struct Pxa270wmmx *wmmx, uint_fast8_t op1,
                                                   uint_fast8_t CRd, uint_fast8_t CRn,
                                                   uint_fast8_t CRm) {
    uint_fast8_t i;
    int_fast16_t sf16;
    int_fast32_t sf32;
    int_fast64_t sf64;

    wmmx->wCASF = 0;
    switch (op1) {
        case 0b0000:  // WSUB.b
            for (i = 0; i < 8; i++) {
                wmmx->wR[CRd].s8[ACCESS_REG_8(i)] = sf16 =
                    (int_fast16_t)wmmx->wR[CRn].s8[ACCESS_REG_8(i)] -
                    (int_fast16_t)wmmx->wR[CRm].s8[ACCESS_REG_8(i)];
                if (((int8_t)sf16) < 0) wmmx->wCASF |= 0x8ul << (i * 4);
                if (!(int8_t)sf16) wmmx->wCASF |= 0x4ul << (i * 4);
                if (!(sf16 & 0x0100)) wmmx->wCASF |= 0x2ul << (i * 4);
                sf16 >>= 7;
                sf16 &= 3;
                if (sf16 != 0 && sf16 != 3) wmmx->wCASF |= 0x1ul << (i * 4);
            }
            break;

        case 0b0100:  // WSUB.h
            for (i = 0; i < 4; i++) {
                wmmx->wR[CRd].s16[ACCESS_REG_16(i)] = sf32 =
                    (int_fast32_t)wmmx->wR[CRn].s16[ACCESS_REG_16(i)] -
                    (int_fast32_t)wmmx->wR[CRm].s16[ACCESS_REG_16(i)];
                if (((int16_t)sf32) < 0) wmmx->wCASF |= 0x80ul << (i * 8);
                if (!(int16_t)sf32) wmmx->wCASF |= 0x40ul << (i * 8);
                if (!(sf32 & 0x010000l)) wmmx->wCASF |= 0x20ul << (i * 8);
                sf32 >>= 15;
                sf32 &= 3;
                if (sf32 != 0 && sf32 != 3) wmmx->wCASF |= 0x10ul << (i * 8);
            }
            break;

        case 0b1000:  // WSUB.w
            for (i = 0; i < 2; i++) {
                wmmx->wR[CRd].s32[ACCESS_REG_32(i)] = sf64 =
                    (int_fast64_t)wmmx->wR[CRn].s32[ACCESS_REG_32(i)] -
                    (int_fast64_t)wmmx->wR[CRm].s32[ACCESS_REG_32(i)];
                if (((int32_t)sf64) < 0) wmmx->wCASF |= 0x8000ul << (i * 16);
                if (!(int32_t)sf64) wmmx->wCASF |= 0x4000ul << (i * 16);
                if (!(sf64 & 0x0100000000ll)) wmmx->wCASF |= 0x2000ul << (i * 16);
                sf64 >>= 31;
                sf64 &= 3;
                if (sf64 != 0 && sf64 != 3) wmmx->wCASF |= 0x1000ul << (i * 16);
            }
            break;

        case 0b0001:  // WSUBUS.b
            for (i = 0; i < 8; i++) {
                wmmx->wR[CRd].s8[ACCESS_REG_8(i)] = sf16 =
                    (int_fast16_t)wmmx->wR[CRn].s8[ACCESS_REG_8(i)] -
                    (int_fast16_t)wmmx->wR[CRm].s8[ACCESS_REG_8(i)];

                if (sf16 >> 8) {
                    wmmx->wR[CRd].s8[ACCESS_REG_8(i)] = 0xff;
                    wmmx->wCASF |= 0x8ul << (i * 4);
                    wmmx->wCSSF |= 1 << i;
                } else {
                    if (((int8_t)sf16) < 0) wmmx->wCASF |= 0x8ul << (i * 4);
                    if (!(int8_t)sf16) wmmx->wCASF |= 0x4ul << (i * 4);
                    if (!(sf16 & 0x0100)) wmmx->wCASF |= 0x2ul << (i * 4);
                    sf16 >>= 7;
                    sf16 &= 3;
                    if (sf16 != 0 && sf16 != 3) wmmx->wCASF |= 0x1ul << (i * 4);
                }
            }
            break;

        case 0b0101:  // WSUBUS.h
            for (i = 0; i < 4; i++) {
                wmmx->wR[CRd].s16[ACCESS_REG_16(i)] = sf32 =
                    (int_fast32_t)wmmx->wR[CRn].s16[ACCESS_REG_16(i)] -
                    (int_fast32_t)wmmx->wR[CRm].s16[ACCESS_REG_16(i)];

                if (sf32 >> 16) {
                    wmmx->wR[CRd].s16[ACCESS_REG_16(i)] = 0xffff;
                    wmmx->wCASF |= 0x80ul << (i * 8);
                    wmmx->wCSSF |= 1 << (2 * i);
                } else {
                    if (((int16_t)sf32) < 0) wmmx->wCASF |= 0x80ul << (i * 8);
                    if (!(int16_t)sf32) wmmx->wCASF |= 0x40ul << (i * 8);
                    if (!(sf32 & 0x010000l)) wmmx->wCASF |= 0x20ul << (i * 8);
                    sf32 >>= 15;
                    sf32 &= 3;
                    if (sf32 != 0 && sf32 != 3) wmmx->wCASF |= 0x10ul << (i * 8);
                }
            }
            break;

        case 0b1001:  // WSUBUS.w
            for (i = 0; i < 2; i++) {
                wmmx->wR[CRd].s32[ACCESS_REG_32(i)] = sf64 =
                    (int_fast64_t)wmmx->wR[CRn].s32[ACCESS_REG_32(i)] -
                    (int_fast64_t)wmmx->wR[CRm].s32[ACCESS_REG_32(i)];

                if (sf64 >> 32) {
                    wmmx->wR[CRd].s32[ACCESS_REG_32(i)] = 0xffffffl;
                    wmmx->wCASF |= 0x8000ul << (i * 16);
                    wmmx->wCSSF |= 1 << (4 * i);
                } else {
                    if (((int32_t)sf64) < 0) wmmx->wCASF |= 0x8000ul << (i * 16);
                    if (!(int32_t)sf64) wmmx->wCASF |= 0x4000ul << (i * 16);
                    if (!(sf64 & 0x0100000000ll)) wmmx->wCASF |= 0x2000ul << (i * 16);
                    sf64 >>= 31;
                    sf64 &= 3;
                    if (sf64 != 0 && sf64 != 3) wmmx->wCASF |= 0x1000ul << (i * 16);
                }
            }
            break;

        case 0b0011:  // WSUBSS.b
            for (i = 0; i < 8; i++) {
                wmmx->wR[CRd].s8[ACCESS_REG_8(i)] = sf16 =
                    (int_fast16_t)wmmx->wR[CRn].s8[ACCESS_REG_8(i)] -
                    (int_fast16_t)wmmx->wR[CRm].s8[ACCESS_REG_8(i)];

                if (sf16 > 0x7f) {
                    wmmx->wR[CRd].s8[ACCESS_REG_8(i)] = 0x7f;
                    wmmx->wCSSF |= 1 << i;
                } else if (sf16 < -0x80) {
                    wmmx->wR[CRd].s8[ACCESS_REG_8(i)] = -0x80;
                    wmmx->wCASF |= 0x8ul << (i * 4);
                    wmmx->wCSSF |= 1 << i;
                } else {
                    if (((int8_t)sf16) < 0) wmmx->wCASF |= 0x8ul << (i * 4);
                    if (!(int8_t)sf16) wmmx->wCASF |= 0x4ul << (i * 4);
                    if (!(sf16 & 0x0100)) wmmx->wCASF |= 0x2ul << (i * 4);
                    sf16 >>= 7;
                    sf16 &= 3;
                    if (sf16 != 0 && sf16 != 3) wmmx->wCASF |= 0x1ul << (i * 4);
                }
            }
            break;

        case 0b0111:  // WSUBSS.h
            for (i = 0; i < 4; i++) {
                wmmx->wR[CRd].s16[ACCESS_REG_16(i)] = sf32 =
                    (int_fast32_t)wmmx->wR[CRn].s16[ACCESS_REG_16(i)] -
                    (int_fast32_t)wmmx->wR[CRm].s16[ACCESS_REG_16(i)];

                if (sf32 > 0x7fff) {
                    wmmx->wR[CRd].s16[ACCESS_REG_16(i)] = 0x7fff;
                    wmmx->wCSSF |= 1 << (2 * i);
                } else if (sf32 < -0x8000) {
                    wmmx->wR[CRd].s16[ACCESS_REG_16(i)] = -0x8000;
                    wmmx->wCASF |= 0x80ul << (i * 8);
                    wmmx->wCSSF |= 1 << (2 * i);
                } else {
                    if (((int16_t)sf32) < 0) wmmx->wCASF |= 0x80ul << (i * 8);
                    if (!(int16_t)sf32) wmmx->wCASF |= 0x40ul << (i * 8);
                    if (!(sf32 & 0x010000l)) wmmx->wCASF |= 0x20ul << (i * 8);
                    sf32 >>= 15;
                    sf32 &= 3;
                    if (sf32 != 0 && sf32 != 3) wmmx->wCASF |= 0x10ul << (i * 8);
                }
            }
            break;

        case 0b1011:  // WSUBSS.w
            for (i = 0; i < 2; i++) {
                wmmx->wR[CRd].s32[ACCESS_REG_32(i)] = sf64 =
                    (int_fast64_t)wmmx->wR[CRn].s32[ACCESS_REG_32(i)] -
                    (int_fast64_t)wmmx->wR[CRm].s32[ACCESS_REG_32(i)];

                if (sf64 > 0x7fffffffl) {
                    wmmx->wR[CRd].s32[ACCESS_REG_32(i)] = 0x7fffffffl;
                    wmmx->wCSSF |= 1 << (4 * i);
                } else if (sf64 < -0x80000000l) {
                    wmmx->wR[CRd].s32[ACCESS_REG_32(i)] = -0x80000000l;
                    wmmx->wCASF |= 0x8000ul << (i * 16);
                    wmmx->wCSSF |= 1 << (4 * i);
                } else {
                    if (((int32_t)sf64) < 0) wmmx->wCASF |= 0x8000ul << (i * 16);
                    if (!(int32_t)sf64) wmmx->wCASF |= 0x4000ul << (i * 16);
                    if (!(sf64 & 0x0100000000ll)) wmmx->wCASF |= 0x2000ul << (i * 16);
                    sf64 >>= 31;
                    sf64 &= 3;
                    if (sf64 != 0 && sf64 != 3) wmmx->wCASF |= 0x1000ul << (i * 16);
                }
            }
            break;

        default:
            return false;
    }
    pxa270wmmxPrvControlRegsChanged(wmmx);
    pxa270wmmxPrvDataRegsChanged(wmmx);
    return true;
}

static bool pxa270wmmxPrvDataProcessing0(struct ArmCpu *cpu, void *userData, bool two, uint8_t op1,
                                         uint8_t CRd, uint8_t CRn, uint8_t CRm, uint8_t op2) {
    struct Pxa270wmmx *wmmx = (struct Pxa270wmmx *)userData;

    if (two) return false;

    switch (op2) {
        case 0b000:
            return pxa270wmmxPrvDataProcessingMisc(wmmx, op1, CRd, CRn, CRm);

        case 0b001:
            return pxa270wmmxPrvDataProcessingAlign(wmmx, op1, CRd, CRn, CRm);

        case 0b010:
            return pxa270wmmxPrvDataProcessingShift(wmmx, false, op1, CRd, CRn, CRm);

        case 0b011:
            return pxa270wmmxPrvDataProcessingCompare(wmmx, op1, CRd, CRn, CRm);

        case 0b100:
            return pxa270wmmxPrvDataProcessingPack(wmmx, op1, CRd, CRn, CRm);

        case 0b110:
            return pxa270wmmxPrvDataProcessingUnpack(wmmx, false, op1, CRd, CRn, CRm);

        case 0b111:
            return pxa270wmmxPrvDataProcessingUnpack(wmmx, true, op1, CRd, CRn, CRm);

        default:
            return false;
    }
    return true;
}

static bool pxa270wmmxPrvDataProcessing1(struct ArmCpu *cpu, void *userData, bool two, uint8_t op1,
                                         uint8_t CRd, uint8_t CRn, uint8_t CRm, uint8_t op2) {
    struct Pxa270wmmx *wmmx = (struct Pxa270wmmx *)userData;

    if (two) return false;

    switch (op2) {
        case 0b000:
            return pxa270wmmxPrvDataProcessingMultiply(wmmx, op1, CRd, CRn, CRm);

        case 0b001:
            return pxa270wmmxPrvDataProcessingDifference(wmmx, op1, CRd, CRn, CRm);

        case 0b010:
            return pxa270wmmxPrvDataProcessingShift(wmmx, true, op1, CRd, CRn, CRm);

        case 0b011:
            return pxa270wmmxPrvDataProcessingMinMax(wmmx, op1, CRd, CRn, CRm);

        case 0b100:
            return pxa270wmmxPrvDataProcessingAddition(wmmx, op1, CRd, CRn, CRm);

        case 0b101:
            return pxa270wmmxPrvDataProcessingSubtraction(wmmx, op1, CRd, CRn, CRm);

        case 0b110:
            return pxa270wmmxPrvDataProcessingAccumulate(wmmx, op1, CRd, CRn, CRm);

        case 0b111:
            return pxa270wmmxPrvDataProcessingShuffle(wmmx, op1, CRd, CRn, CRm);

        default:
            return false;
    }
}

static void pxa270wmmxPrvSetCoreReg(struct ArmCpu *cpu, uint_fast8_t reg, uint32_t val) {
    if (reg == 15) {
        val &= 0xf0000000ul;
        val |= cpuGetRegExternal(cpu, ARM_REG_NUM_CPSR) & ~0xf0000000ul;
        reg = ARM_REG_NUM_CPSR;
    }

    cpuSetReg(cpu, reg, val);
}

static bool pxa270wmmxPrvRegXferTmcrTmrc(struct Pxa270wmmx *wmmx, struct ArmCpu *cpu, bool read,
                                         uint_fast8_t op1, uint_fast8_t Rx, uint_fast8_t CRn) {
    uint32_t val = 0;

    switch (CRn) {
        case 0b0000:  // wCID
            if (read)
                val = 0x69051000ul;
            else
                return false;
            break;

        case 0b0001:  // wCON
            if (read)
                val = wmmx->wCon;
            else
                wmmx->wCon = val & 3;
            break;

        case 0b0010:  // wCSSF
            if (read)
                val = wmmx->wCSSF;
            else
                wmmx->wCSSF = val & 0xff;
            break;

        case 0b0011:  // wCASF:
            if (read)
                val = wmmx->wCASF;
            else
                wmmx->wCASF = val;
            break;

        case 0b1000:  // wCGR[0..3]
        case 0b1001:
        case 0b1010:
        case 0b1011:
            if (read)
                val = wmmx->wCGR[CRn - 8];
            else
                wmmx->wCGR[CRn - 8] = val;
            break;

        default:
            return false;
    }

    if (read) pxa270wmmxPrvSetCoreReg(cpu, Rx, val);

    return true;
}

static bool pxa270wmmxPrvRegXferTmia(struct Pxa270wmmx *wmmx, struct ArmCpu *cpu, uint_fast8_t op2,
                                     uint_fast8_t wRd, uint_fast8_t Rs, uint_fast8_t Rm) {
    uint32_t vM = cpuGetRegExternal(cpu, Rm), vS = cpuGetRegExternal(cpu, Rs);

    switch (op2) {
        case 0b0000:  // TMIA
            wmmx->wR[wRd].s64 += (int64_t)(int32_t)vM * (int64_t)(int32_t)vS;
            break;

        case 0b1000:  // TMIAPH
            wmmx->wR[wRd].s64 += (int64_t)(int16_t)vM * (int64_t)(int16_t)vS +
                                 (int64_t)(int16_t)(vM >> 16) * (int64_t)(int16_t)(vS >> 16);
            break;

        case 0b1100:  // TMIABB
        case 0b1101:  // TMIABT
        case 0b1110:  // TMIATB
        case 0b1111:  // TMIATT
            if (op2 & 1) vS >>= 16;
            if (op2 & 2) vM >>= 16;
            wmmx->wR[wRd].s64 += (int64_t)(int16_t)vM * (int64_t)(int16_t)vS;
            break;

        default:
            return false;
    }
    return true;
}

static bool pxa270wmmxPrvRegXferTorc(struct Pxa270wmmx *wmmx, struct ArmCpu *cpu, uint_fast8_t op1,
                                     uint_fast8_t Rd) {
    uint32_t val = wmmx->wCASF;

    switch (op1) {
        case 0b000:  // TORCB
            val |= val << 4;
            // fallthrough

        case 0b010:  // TORCH
            val |= val << 8;
            // fallthrough

        case 0b100:  // TORCW
            val |= val << 16;
            val &= 0xf0000000ul;
            pxa270wmmxPrvSetCoreReg(cpu, Rd, val);
            return true;

        default:
            return false;
    }
}

static bool pxa270wmmxPrvRegXferTandc(struct Pxa270wmmx *wmmx, struct ArmCpu *cpu, uint_fast8_t op1,
                                      uint_fast8_t Rd) {
    uint32_t val = wmmx->wCASF;

    switch (op1) {
        case 0b000:  // TORCB
            val &= val << 4;
            // fallthrough

        case 0b010:  // TORCH
            val &= val << 8;
            // fallthrough

        case 0b100:  // TORCW
            val &= val << 16;
            val &= 0xf0000000ul;
            pxa270wmmxPrvSetCoreReg(cpu, Rd, val);
            return true;

        default:
            return false;
    }
}

static bool pxa270wmmxPrvRegXferTextrc(struct Pxa270wmmx *wmmx, struct ArmCpu *cpu,
                                       uint_fast8_t op1, uint_fast8_t Rd, uint_fast8_t imm3) {
    uint32_t val = wmmx->wCASF;

    switch (op1) {
        case 0b000:  // TEXTRCB
            val >>= imm3 * 4;
            break;

        case 0b010:  // TEXTRCH
            val >>= (imm3 & 3) * 8 + 4;
            break;

        case 0b100:  // TEXTRCW
            val >>= (imm3 & 1) * 16 + 12;
            break;

        default:
            return false;
    }
    val <<= 28;
    pxa270wmmxPrvSetCoreReg(cpu, Rd, val);
    return true;
}

static bool pxa270wmmxPrvRegXferTextrm(struct Pxa270wmmx *wmmx, struct ArmCpu *cpu,
                                       uint_fast8_t op1, uint_fast8_t Rd, uint_fast8_t wRn,
                                       bool sext, uint_fast8_t imm3) {
    uint32_t val;

    switch (op1) {
        case 0b000:  // TEXTRMB
            if (sext)
                val = (int32_t)wmmx->wR[wRn].s8[ACCESS_REG_8(imm3)];
            else
                val = wmmx->wR[wRn].v8[ACCESS_REG_8(imm3)];
            break;

        case 0b010:  // TEXTRMH
            imm3 &= 3;
            if (sext)
                val = (int32_t)wmmx->wR[wRn].s16[ACCESS_REG_16(imm3)];
            else
                val = wmmx->wR[wRn].v16[ACCESS_REG_16(imm3)];
            break;

        case 0b100:  // TEXTRMW
            val = wmmx->wR[wRn].v32[ACCESS_REG_32(imm3 & 1)];
            break;

        default:
            return false;
    }
    pxa270wmmxPrvSetCoreReg(cpu, Rd, val);
    return true;
}

static bool pxa270wmmxPrvRegXferTbcst(struct Pxa270wmmx *wmmx, struct ArmCpu *cpu, uint_fast8_t op1,
                                      uint_fast8_t wRd, uint_fast8_t Rn) {
    uint32_t v = cpuGetRegExternal(cpu, Rn);
    uint_fast8_t i;

    switch (op1) {
        case 0b000:  // TBCSTB
            for (i = 0; i < 8; i++) wmmx->wR[wRd].v8[ACCESS_REG_8(i)] = v;
            break;

        case 0b010:  // TBCSTH
            for (i = 0; i < 4; i++) wmmx->wR[wRd].v16[ACCESS_REG_16(i)] = v;
            break;

        case 0b100:  // TBCSTW
            for (i = 0; i < 2; i++) wmmx->wR[wRd].v32[ACCESS_REG_32(i)] = v;
            break;

        default:
            return false;
    }
    pxa270wmmxPrvDataRegsChanged(wmmx);
    return true;
}

static bool pxa270wmmxPrvRegXferTinsr(struct Pxa270wmmx *wmmx, struct ArmCpu *cpu, uint_fast8_t op1,
                                      uint_fast8_t wRd, uint_fast8_t Rn, uint_fast8_t at) {
    uint32_t v = cpuGetRegExternal(cpu, Rn);

    switch (op1) {
        case 0b000:  // TBCSTB
            wmmx->wR[wRd].v8[ACCESS_REG_8(at & 7)] = v;
            break;

        case 0b010:  // TBCSTH
            wmmx->wR[wRd].v16[ACCESS_REG_16(at & 3)] = v;
            break;

        case 0b100:  // TBCSTW
            wmmx->wR[wRd].v32[ACCESS_REG_32(at & 1)] = v;
            break;

        default:
            return false;
    }
    pxa270wmmxPrvDataRegsChanged(wmmx);
    return true;
}

static bool pxa270wmmxPrvRegXferTmovmsk(struct Pxa270wmmx *wmmx, struct ArmCpu *cpu,
                                        uint_fast8_t op1, uint_fast8_t Rd, uint_fast8_t wRn) {
    uint_fast8_t val = 0, i;

    switch (op1) {
        case 0b000:  // TMOVMSKB
            for (i = 0; i < 8; i++) {
                if (wmmx->wR[wRn].s8[ACCESS_REG_8(i)] < 0) val |= 1 << i;
            }
            break;

        case 0b010:  // TMOVMSKH
            for (i = 0; i < 4; i++) {
                if (wmmx->wR[wRn].s16[ACCESS_REG_16(i)] < 0) val |= 1 << i;
            }
            break;

        case 0b100:  // TMOVMSKW
            for (i = 0; i < 2; i++) {
                if (wmmx->wR[wRn].s32[ACCESS_REG_32(i)] < 0) val |= 1 << i;
            }
            break;

        default:
            return false;
    }

    pxa270wmmxPrvSetCoreReg(cpu, Rd, val);
    return true;
}

static bool pxa270wmmxPrvRegXfer(struct Pxa270wmmx *wmmx, bool cp1, struct ArmCpu *cpu, bool two,
                                 bool read, uint8_t op1, uint8_t Rx, uint8_t CRn, uint8_t CRm,
                                 uint8_t op2) {
    if (two) return false;

    switch (op1) {
        case 0b000:  // TMCR / TMRC
            if (op2 || !cp1 || CRm || (CRn & 4)) return false;
            return pxa270wmmxPrvRegXferTmcrTmrc(wmmx, cpu, read, op1, Rx, CRn);

        case 0b001:  // TMIA...
            if (read) return false;
            return pxa270wmmxPrvRegXferTmia(wmmx, cpu, CRn, op2 + (cp1 ? 8 : 0), Rx, CRm);

        default:
            return false;
    }
    if (read) switch (op2) {  // dispatch MRC

            case 0b001:
                if (CRm) return false;
                if (cp1) return pxa270wmmxPrvRegXferTandc(wmmx, cpu, op1, Rx);
                return pxa270wmmxPrvRegXferTmovmsk(wmmx, cpu, op1, Rx, CRn);

            case 0b010:
                if ((!cp1) || CRm) return false;
                return pxa270wmmxPrvRegXferTorc(wmmx, cpu, op1, Rx);

            case 0b011:
                if (op1 & 1) return false;
                if (!cp1)
                    return pxa270wmmxPrvRegXferTextrm(wmmx, cpu, op1, Rx, CRn, !!(CRm & 8),
                                                      CRm & 7);
                if (CRm & 8) return false;
                return pxa270wmmxPrvRegXferTextrc(wmmx, cpu, op1, Rx, CRm);

            default:
                return false;
        }
    else if (cp1)
        return false;
    else
        switch (op1) {  // dispatch MCR

            case 0b010:
                if (CRm) return false;
                return pxa270wmmxPrvRegXferTbcst(wmmx, cpu, op1, CRn, Rx);

            case 0b011:
                if (CRm & 8) return false;
                return pxa270wmmxPrvRegXferTinsr(wmmx, cpu, op1, CRn, Rx, CRm);

            default:
                return false;
        }
}

static bool pxa270wmmxPrvTwoReg(struct Pxa270wmmx *wmmx, bool cp1, struct ArmCpu *cpu, bool read,
                                uint8_t op, uint8_t RdLo, uint8_t RdHi, uint8_t wR) {
    if (cp1 || op) return false;

    if (read) {
        pxa270wmmxPrvSetCoreReg(cpu, RdLo, wmmx->wR[wR].v32[ACCESS_REG_32(0)]);
        pxa270wmmxPrvSetCoreReg(cpu, RdHi, wmmx->wR[wR].v32[ACCESS_REG_32(1)]);
    } else {
        wmmx->wR[wR].v32[ACCESS_REG_32(0)] = cpuGetRegExternal(cpu, RdLo);
        wmmx->wR[wR].v32[ACCESS_REG_32(1)] = cpuGetRegExternal(cpu, RdHi);
        pxa270wmmxPrvDataRegsChanged(wmmx);
    }

    return false;
}

static bool pxa270wmmxPrvMemAccessControl(struct Pxa270wmmx *wmmx, struct ArmCpu *cpu, bool store,
                                          uint32_t addr, uint_fast8_t reg) {
    uint32_t val = 0;

    if (!store && !cpuMemOpExternal(cpu, &val, addr, 4, false)) return false;

    switch (reg) {
        case 0b0000:  // wCID
            if (store)
                val = 0x69051000ul;
            else
                return false;
            break;

        case 0b0001:  // wCON
            if (store)
                val = wmmx->wCon;
            else
                wmmx->wCon = val & 3;
            break;

        case 0b0010:  // wCSSF
            if (store)
                val = wmmx->wCSSF;
            else
                wmmx->wCSSF = val & 0xff;
            break;

        case 0b0011:  // wCASF:
            if (store)
                val = wmmx->wCASF;
            else
                wmmx->wCASF = val;
            break;

        case 0b1000:  // wCGR[0..3]
        case 0b1001:
        case 0b1010:
        case 0b1011:
            if (store)
                val = wmmx->wCGR[reg - 8];
            else
                wmmx->wCGR[reg - 8] = val;
            break;

        default:
            return false;
    }

    if (store) pxa270wmmxPrvControlRegsChanged(wmmx);

    return !store || cpuMemOpExternal(cpu, &val, addr, 4, true);
}

static bool pxa270wmmxPrvMemAccessDataD(struct Pxa270wmmx *wmmx, struct ArmCpu *cpu, bool store,
                                        uint32_t ea, uint_fast8_t wR) {
    if (!cpuMemOpExternal(cpu, &wmmx->wR[wR].v32[ACCESS_REG_32(0)], ea + 0, 4, store)) return false;

    if (!cpuMemOpExternal(cpu, &wmmx->wR[wR].v32[ACCESS_REG_32(1)], ea + 4, 4, store)) return false;

    if (!store) pxa270wmmxPrvDataRegsChanged(wmmx);
    return true;
}

static bool pxa270wmmxPrvMemAccessDataW(struct Pxa270wmmx *wmmx, struct ArmCpu *cpu, bool store,
                                        uint32_t ea, uint_fast8_t wR) {
    if (!cpuMemOpExternal(cpu, &wmmx->wR[wR].v32[ACCESS_REG_32(0)], ea, 4, store)) return false;

    if (!store) {
        wmmx->wR[wR].v32[ACCESS_REG_32(1)] = 0;
        pxa270wmmxPrvDataRegsChanged(wmmx);
    }

    return true;
}

static bool pxa270wmmxPrvMemAccessDataH(struct Pxa270wmmx *wmmx, struct ArmCpu *cpu, bool store,
                                        uint32_t ea, uint_fast8_t wR) {
    if (!cpuMemOpExternal(cpu, &wmmx->wR[wR].v16[ACCESS_REG_16(0)], ea, 2, store)) return false;

    if (!store) {
        uint_fast8_t i;

        for (i = 1; i < 4; i++) wmmx->wR[wR].v16[ACCESS_REG_16(i)] = 0;
        pxa270wmmxPrvDataRegsChanged(wmmx);
    }

    return true;
}

static bool pxa270wmmxPrvMemAccessDataB(struct Pxa270wmmx *wmmx, struct ArmCpu *cpu, bool store,
                                        uint32_t ea, uint_fast8_t wR) {
    if (!cpuMemOpExternal(cpu, &wmmx->wR[wR].v8[ACCESS_REG_8(0)], ea, 1, store)) return false;

    if (!store) {
        uint_fast8_t i;

        for (i = 1; i < 8; i++) wmmx->wR[wR].v8[ACCESS_REG_8(i)] = 0;
        pxa270wmmxPrvDataRegsChanged(wmmx);
    }

    return true;
}

static bool pxa270wmmxPrvMemAccess(struct Pxa270wmmx *wmmx, bool cp1, struct ArmCpu *cpu, bool two,
                                   bool N, bool store, uint8_t wR, uint_fast8_t addrReg,
                                   uint32_t addBefore, uint32_t addAfter, uint8_t *option) {
    uint32_t ea = cpuGetRegExternal(cpu, addrReg);

    if (!two) {  // access to data regs

        if (cp1) {
            ea += addBefore * 4;

            if (N) {
                if (!pxa270wmmxPrvMemAccessDataD(wmmx, cpu, store, ea, wR)) return false;
            } else {
                if (!pxa270wmmxPrvMemAccessDataW(wmmx, cpu, store, ea, wR)) return false;
            }

            if (addAfter) pxa270wmmxPrvSetCoreReg(cpu, addrReg, ea - addBefore * 4 + addAfter * 4);
        } else {
            ea += addBefore;

            if (N) {
                if (!pxa270wmmxPrvMemAccessDataH(wmmx, cpu, store, ea, wR)) return false;
            } else {
                if (!pxa270wmmxPrvMemAccessDataB(wmmx, cpu, store, ea, wR)) return false;
            }

            if (addAfter) pxa270wmmxPrvSetCoreReg(cpu, addrReg, ea - addBefore + addAfter);
        }

        pxa270wmmxPrvDataRegsChanged(wmmx);
        return true;
    }

    if (cp1 || N) return false;

    // access to control regs

    if (!pxa270wmmxPrvMemAccessControl(wmmx, cpu, store, ea + addBefore * 4, wR)) return false;

    if (addAfter) pxa270wmmxPrvSetCoreReg(cpu, addrReg, ea + addAfter * 4);

    return true;
}

static bool pxa270wmmxPrvRegXfer0(struct ArmCpu *cpu, void *userData, bool two, bool MRC,
                                  uint8_t op1, uint8_t Rx, uint8_t CRn, uint8_t CRm, uint8_t op2) {
    return pxa270wmmxPrvRegXfer((struct Pxa270wmmx *)userData, false, cpu, two, MRC, op1, Rx, CRn,
                                CRm, op2);
}

static bool pxa270wmmxPrvRegXfer1(struct ArmCpu *cpu, void *userData, bool two, bool MRC,
                                  uint8_t op1, uint8_t Rx, uint8_t CRn, uint8_t CRm, uint8_t op2) {
    return pxa270wmmxPrvRegXfer((struct Pxa270wmmx *)userData, true, cpu, two, MRC, op1, Rx, CRn,
                                CRm, op2);
}

static bool pxa270wmmxPrvMemAccess0(struct ArmCpu *cpu, void *userData, bool two, bool N,
                                    bool store, uint8_t CRd, uint_fast8_t addrReg,
                                    uint32_t addBefore, uint32_t addAfter, uint8_t *option) {
    return pxa270wmmxPrvMemAccess((struct Pxa270wmmx *)userData, false, cpu, two, N, store, CRd,
                                  addrReg, addBefore, addAfter, option);
}

static bool pxa270wmmxPrvMemAccess1(struct ArmCpu *cpu, void *userData, bool two, bool N,
                                    bool store, uint8_t CRd, uint_fast8_t addrReg,
                                    uint32_t addBefore, uint32_t addAfter, uint8_t *option) {
    return pxa270wmmxPrvMemAccess((struct Pxa270wmmx *)userData, true, cpu, two, N, store, CRd,
                                  addrReg, addBefore, addAfter, option);
}

static bool pxa270wmmxPrvTwoReg0(struct ArmCpu *cpu, void *userData, bool MRRC, uint8_t op,
                                 uint8_t Rd, uint8_t Rn, uint8_t CRm) {
    return pxa270wmmxPrvTwoReg((struct Pxa270wmmx *)userData, false, cpu, MRRC, op, Rd, Rn, CRm);
}

static bool pxa270wmmxPrvTwoReg1(struct ArmCpu *cpu, void *userData, bool MRRC, uint8_t op,
                                 uint8_t Rd, uint8_t Rn, uint8_t CRm) {
    return pxa270wmmxPrvTwoReg((struct Pxa270wmmx *)userData, true, cpu, MRRC, op, Rd, Rn, CRm);
}

struct Pxa270wmmx *pxa270wmmxInit(struct ArmCpu *cpu) {
    struct Pxa270wmmx *wmmx = (struct Pxa270wmmx *)malloc(sizeof(*wmmx));
    struct ArmCoprocessor cp0 = {
        .regXfer = pxa270wmmxPrvRegXfer0,
        .dataProcessing = pxa270wmmxPrvDataProcessing0,
        .memAccess = pxa270wmmxPrvMemAccess0,
        .twoRegF = pxa270wmmxPrvTwoReg0,
        .userData = wmmx,
    };
    struct ArmCoprocessor cp1 = {
        .regXfer = pxa270wmmxPrvRegXfer1,
        .dataProcessing = pxa270wmmxPrvDataProcessing1,
        .memAccess = pxa270wmmxPrvMemAccess1,
        .twoRegF = pxa270wmmxPrvTwoReg1,
        .userData = wmmx,
    };

    if (!wmmx) ERR("cannot alloc WMMX");

    memset(wmmx, 0, sizeof(*wmmx));

    cpuCoprocessorRegister(cpu, 0, &cp0);
    cpuCoprocessorRegister(cpu, 1, &cp1);

    return wmmx;
}
