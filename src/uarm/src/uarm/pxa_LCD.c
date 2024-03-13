//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "pxa_LCD.h"

#include <stdlib.h>
#include <string.h>

#include "mem.h"
#include "pxa_IC.h"
#include "util.h"

#define PXA_LCD_BASE 0x44000000UL
#define PXA_LCD_SIZE 0x00001000UL

#define LCD_STATE_IDLE 0
#define LCD_STATE_DMA_0_START 1
#define LCD_STATE_DMA_0_END 2

#define UNMASKABLE_INTS 0x7C8E

struct PxaLcd {
    struct SocIc *ic;
    struct ArmMem *mem;

    // registers
    uint32_t lccr0, lccr1, lccr2, lccr3, lccr4, lccr5, liicr, trgbr, tcr;
    uint32_t fbr[7], fdadr[7], fsadr[7], fidr[7], ldcmd[7];
    uint16_t lcsr;  // yes, 16-bit :)

    // for our use
    uint16_t intMask;

    uint8_t state : 6;
    uint8_t intWasPending : 1;
    uint8_t enbChanged : 1;

    uint8_t palette[512];
    uint32_t palette_mapped[512];

    uint16_t width;
    uint16_t height;

    uint32_t *front_buffer;
    uint32_t *back_buffer;

    uint32_t i_pixel;
    bool frame_pending;

    uint32_t frameNum;
};

static uint32_t unpack_rgb16(uint16_t rgb16) {
    uint8_t r = (rgb16 >> 11) & 0x1f;
    uint8_t g = (rgb16 >> 5) & 0x3f;
    uint8_t b = (rgb16 >> 0) & 0x1f;

    r = (r << 3) | (r >> 2);
    g = (g << 2) | (g >> 4);
    b = (b << 3) | (b >> 2);

    return 0xff000000 | (b << 16) | (g << 8) | r;
}

static void pxaLcdPrvUpdateInts(struct PxaLcd *lcd) {
    uint_fast16_t ints = lcd->lcsr & lcd->intMask;

    if ((ints && !lcd->intWasPending) || (!ints && lcd->intWasPending)) {
        lcd->intWasPending = !!ints;
        socIcInt(lcd->ic, PXA_I_LCD, !!ints);
    }
}

static bool pxaLcdPrvMemAccessF(void *userData, uint32_t pa, uint_fast8_t size, bool write,
                                void *buf) {
    struct PxaLcd *lcd = (struct PxaLcd *)userData;
    uint_fast16_t v16;
    uint32_t val = 0;

    if (size != 4) {
        fprintf(stderr, "%s: Unexpected %s of %u bytes to 0x%08lx\n", __func__,
                write ? "write" : "read", size, (unsigned long)pa);
        return false;  // we do not support non-word accesses
    }

    pa = (pa - PXA_LCD_BASE) >> 2;

    if (write) {
        val = *(uint32_t *)buf;

        switch (pa) {
            case 0:
                if ((lcd->lccr0 ^ val) & 0x0001)  // something changed about enablement - handle it
                    lcd->enbChanged = 1;

                lcd->lccr0 = val;
                // recalc intMask
                v16 = UNMASKABLE_INTS;
                if (val & 0x00200000UL)  // output fifo underrun
                    v16 |= 0x0040;
                if (val & 0x00100000UL)  // branch int
                    v16 |= 0x0200;
                if (val & 0x00000800UL)  // quick disable
                    v16 |= 0x0001;
                if (val & 0x00000040UL)  // end of frame
                    v16 |= 0x0080;
                if (val & 0x00000020UL)  // input fifo underrun
                    v16 |= 0x0030;
                if (val & 0x00000010UL)  // start of frame
                    v16 |= 0x0002;
                lcd->intMask = v16;
                pxaLcdPrvUpdateInts(lcd);
                break;

            case 1:
                lcd->lccr1 = val;
                break;

            case 2:
                lcd->lccr2 = val;
                break;

            case 3:
                lcd->lccr3 = val;
                break;

            case 4:
                lcd->lccr4 = val;
                break;

            case 5:
                lcd->lccr5 = val;
                break;

            case 68:
            case 69:
                pa -= 69;
                pa += 13;
                pa += 1;
                // fallthrough
            case 8:
            case 9:
            case 10:
            case 11:
            case 13:
                lcd->fbr[pa - 8] = val;
                break;

            case 14:
                lcd->lcsr &= ~val;
                pxaLcdPrvUpdateInts(lcd);
                break;

            case 15:
                lcd->liicr = val;
                break;

            case 16:
                lcd->trgbr = val;
                break;

            case 17:
                lcd->tcr = val;
                break;

            case 128:
            case 132:
            case 136:
            case 140:
            case 144:
            case 148:
            case 152:
                lcd->fdadr[(pa - 128) / 4] = val;
                break;
        }
    } else {
        switch (pa) {
            case 0:
                val = lcd->lccr0;
                break;

            case 1:
                val = lcd->lccr1;
                break;

            case 2:
                val = lcd->lccr2;
                break;

            case 3:
                val = lcd->lccr3;
                break;

            case 4:
                val = lcd->lccr4;
                break;

            case 5:
                val = lcd->lccr5;
                break;

            case 68:
            case 69:
                pa -= 69;
                pa += 13;
                pa += 1;
                // fallthrough
            case 8:
            case 9:
            case 10:
            case 11:
            case 13:
                val = lcd->fbr[pa - 8];
                break;

            case 14:
                val = lcd->lcsr;
                break;

            case 15:
                val = lcd->liicr;
                break;

            case 16:
                val = lcd->trgbr;
                break;

            case 17:
                val = lcd->tcr;
                break;

            case 128:
            case 132:
            case 136:
            case 140:
            case 144:
            case 148:
            case 152:
                val = lcd->fdadr[(pa - 128) / 4];
                break;

            case 129:
            case 133:
            case 137:
            case 141:
            case 145:
            case 149:
            case 153:
                val = lcd->fsadr[(pa - 129) / 4];
                break;

            case 130:
            case 134:
            case 138:
            case 142:
            case 146:
            case 150:
            case 154:
                val = lcd->fidr[(pa - 130) / 4];
                break;

            case 131:
            case 135:
            case 139:
            case 143:
            case 147:
            case 151:
            case 155:
                val = lcd->ldcmd[(pa - 131) / 4];
                break;
        }
        *(uint32_t *)buf = val;
    }

    return true;
}

static uint32_t pxaLcdPrvGetWord(struct PxaLcd *lcd, uint32_t addr) {
    uint32_t v;

    if (!memAccess(lcd->mem, addr, 4, false, &v)) return 0;

    return v;
}

static void pxaLcdPrvDma(struct PxaLcd *lcd, void *dest, uint32_t addr, int32_t len) {
    uint8_t *d = (uint8_t *)dest;
    uint32_t t;

    // we assume aligntment here both on part of dest and of addr

    while (len > 0) {
        t = pxaLcdPrvGetWord(lcd, addr);
        if (len-- > 0) *d++ = t;
        if (len-- > 0) *d++ = t >> 8;
        if (len-- > 0) *d++ = t >> 16;
        if (len-- > 0) *d++ = t >> 24;
        addr += 4;
    }
}

static void pxaLcdUpdatePalette(struct PxaLcd *lcd, int32_t len) {
    const uint32_t n_entries = len >> 1;
    uint16_t *entry = (uint16_t *)lcd->palette;
    uint32_t *entry_mapped = (uint32_t *)lcd->palette_mapped;

    for (uint32_t i = 0; i < n_entries; i++, entry++, entry_mapped++)
        *entry_mapped = unpack_rgb16(*entry);
}

static void pxaLcdPrvScreenDataPixel(struct PxaLcd *lcd, uint32_t color) {
    lcd->back_buffer[lcd->i_pixel++] = color;

    if (lcd->i_pixel == lcd->width * lcd->height) {
        lcd->i_pixel = 0;
        lcd->frame_pending = true;

        uint32_t *front_buffer = lcd->front_buffer;
        lcd->front_buffer = lcd->back_buffer;
        lcd->back_buffer = front_buffer;
    }
}

static void pxaLcdPrvScreenDataDma(struct PxaLcd *lcd, uint32_t addr /*PA*/, uint32_t len) {
    uint8_t data[4];
    uint32_t i, j;

    len /= 4;
    while (len--) {
        pxaLcdPrvDma(lcd, data, addr, 4);
        addr += 4;

        switch ((lcd->lccr3 >> 24) & 7) {
            case 0:  // 1BPP

                for (i = 0; i < 4; i++) {
                    for (j = 0; j < 8; j++)
                        pxaLcdPrvScreenDataPixel(lcd, lcd->palette_mapped[((data[i] >> j) & 1)]);
                }
                break;

            case 1:  // 2BPP

                for (i = 0; i < 4; i++) {
                    for (j = 0; j < 8; j += 2)
                        pxaLcdPrvScreenDataPixel(lcd, lcd->palette_mapped[((data[i] >> j) & 3)]);
                }
                break;

            case 2:  // 4BPP

                for (i = 0; i < 4; i++) {
                    for (j = 0; j < 8; j += 4)
                        pxaLcdPrvScreenDataPixel(lcd, lcd->palette_mapped[((data[i] >> j) & 15)]);
                }
                break;

            case 3:  // 8BPP

                for (i = 0; i < 4; i++) pxaLcdPrvScreenDataPixel(lcd, lcd->palette_mapped[data[i]]);

                break;

            case 4:  // 16BPP

                pxaLcdPrvScreenDataPixel(lcd, unpack_rgb16(*(uint16_t *)(&data[0])));
                pxaLcdPrvScreenDataPixel(lcd, unpack_rgb16(*(uint16_t *)(&data[2])));
                break;

            default:

                ;  // BAD
        }
    }
}

static void pxaLcdFrame(void *userData) {
    struct PxaLcd *lcd = userData;

    // every other call starts a frame, the others end one [this generates spacing between
    // interrupts so as to not confuse guest OS]

    if (lcd->enbChanged) {
        if (lcd->lccr0 & 0x0001) {  // just got enabled

            // TODO: perhaps check settings?
        } else {  // we just got quick disabled - kill current frame and do no more

            lcd->lcsr |= 0x0080;  // quick disable happened
            lcd->state = LCD_STATE_IDLE;
        }
        lcd->enbChanged = false;
    }

    if (lcd->lccr0 & 0x0001) {  // enabled - do a frame

        uint32_t descrAddr, len;

        if (lcd->lccr0 & 0x400) {  // got disabled

            lcd->lcsr |= 0x0001;  // disable happened
            lcd->state = LCD_STATE_IDLE;
            lcd->lccr0 &= ~1;
        } else
            switch (lcd->state) {
                case LCD_STATE_IDLE:

                    if (lcd->fbr[0] & 1) {  // branch

                        lcd->fbr[0] &= ~1UL;
                        if (lcd->fbr[0] & 2) lcd->lcsr |= 0x0200;
                        descrAddr = lcd->fbr[0] & ~0xFUL;
                    } else
                        descrAddr = lcd->fdadr[0];
                    lcd->fdadr[0] = pxaLcdPrvGetWord(lcd, descrAddr + 0);
                    lcd->fsadr[0] = pxaLcdPrvGetWord(lcd, descrAddr + 4);
                    lcd->fidr[0] = pxaLcdPrvGetWord(lcd, descrAddr + 8);
                    lcd->ldcmd[0] = pxaLcdPrvGetWord(lcd, descrAddr + 12);

                    lcd->state = LCD_STATE_DMA_0_START;
                    break;

                case LCD_STATE_DMA_0_START:

                    if (lcd->ldcmd[0] & 0x00400000UL)
                        lcd->lcsr |= 0x0002;  // set SOF is DMA 0 started
                    len = lcd->ldcmd[0] & 0x000FFFFFUL;

                    if (lcd->ldcmd[0] & 0x04000000UL) {  // pallette data

                        if (len > sizeof(lcd->palette)) len = sizeof(lcd->palette);

                        pxaLcdPrvDma(lcd, lcd->palette, lcd->fsadr[0], len);
                        pxaLcdUpdatePalette(lcd, len);
                    } else {
                        lcd->frameNum++;

                        if (!(lcd->frameNum & 63)) pxaLcdPrvScreenDataDma(lcd, lcd->fsadr[0], len);
                    }

                    lcd->state = LCD_STATE_DMA_0_END;
                    break;

                case LCD_STATE_DMA_0_END:

                    if (lcd->ldcmd[0] & 0x00200000UL)
                        lcd->lcsr |= 0x0100;  // set EOF is DMA 0 finished
                    lcd->state = LCD_STATE_IDLE;
                    break;
            }
    }
    pxaLcdPrvUpdateInts(lcd);
}

uint32_t *pxaLcdGetPendingFrame(struct PxaLcd *lcd) {
    return lcd->frame_pending ? lcd->front_buffer : NULL;
}

void pxaLcdResetPendingFrame(struct PxaLcd *lcd) { lcd->frame_pending = false; }

struct PxaLcd *pxaLcdInit(struct ArmMem *physMem, struct SocIc *ic, struct Clock *clock,
                          uint16_t width, uint16_t height) {
    struct PxaLcd *lcd = (struct PxaLcd *)malloc(sizeof(*lcd));

    if (!lcd) ERR("cannot alloc LCD");

    memset(lcd, 0, sizeof(*lcd));
    lcd->ic = ic;
    lcd->mem = physMem;
    lcd->intMask = UNMASKABLE_INTS;
    lcd->width = width;
    lcd->height = height;

    lcd->front_buffer = (uint32_t *)malloc(width * height * 4);
    lcd->back_buffer = (uint32_t *)malloc(width * height * 4);

    if (!memRegionAdd(physMem, PXA_LCD_BASE, PXA_LCD_SIZE, pxaLcdPrvMemAccessF, lcd))
        ERR("cannot add LCD to MEM\n");

    clockRegisterConsumer(clock, 1000000000ULL / (64 * 3 * 60), pxaLcdFrame, lcd);

    return lcd;
}
