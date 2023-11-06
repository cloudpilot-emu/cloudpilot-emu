#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

static uint32_t translateThumb(uint16_t instrT) {
    bool vB;
    uint32_t t, instr = 0xE0000000UL /*most likely thing*/;
    uint16_t v16;
    uint_fast8_t v8;

    switch (instrT >> 12) {
        case 0:  // LSL(1) LSR(1) ASR(1) ADD(1) SUB(1) ADD(3) SUB(3)
        case 1:
            if ((instrT & 0x1800) != 0x1800) {  // LSL(1) LSR(1) ASR(1)

                instr |= 0x01B00000UL | ((instrT & 0x7) << 12) | ((instrT >> 3) & 7) |
                         ((instrT >> 6) & 0x60) | ((instrT << 1) & 0xF80);
            } else {
                vB = !!(instrT & 0x0200);  // SUB or ADD ?
                instr |= ((vB ? 5UL : 9UL) << 20) | (((uint32_t)(instrT & 0x38)) << 13) |
                         ((instrT & 0x07) << 12) | ((instrT >> 6) & 0x07);

                if (instrT & 0x0400) {  // ADD(1) SUB(1)

                    instr |= 0x02000000UL;
                } else {  // ADD(3) SUB(3)

                    // nothing to do here
                }
            }
            break;

        case 2:  // MOV(1) CMP(1) ADD(2) SUB(2)
        case 3:
            instr |= instrT & 0x00FF;
            switch ((instrT >> 11) & 3) {
                case 0:  // MOV(1)
                    instr |= 0x03B00000UL | ((instrT & 0x0700) << 4);
                    break;

                case 1:  // CMP(1)
                    instr |= 0x03500000UL | (((uint32_t)(instrT & 0x0700)) << 8);
                    break;

                case 2:  // ADD(2)
                    instr |= 0x02900000UL | ((instrT & 0x0700) << 4) |
                             (((uint32_t)(instrT & 0x0700)) << 8);
                    break;

                case 3:  // SUB(2)
                    instr |= 0x02500000UL | ((instrT & 0x0700) << 4) |
                             (((uint32_t)(instrT & 0x0700)) << 8);
                    break;
            }
            break;

        case 4:  // LDR(3) ADD(4) CMP(3) MOV(3) BX MVN CMP(2) CMN TST ADC SBC NEG MUL LSL(2) LSR(2)
                 // ASR(2) ROR AND EOR ORR BIC

            if (instrT & 0x0800) {  // LDR(3)
                return 0;
            } else if (instrT & 0x0400) {  // ADD(4) CMP(3) MOV(3) BX

                uint8_t vD;

                vD = (instrT & 7) | ((instrT >> 4) & 0x08);
                v8 = (instrT >> 3) & 0xF;

                switch ((instrT >> 8) & 3) {
                    case 0:  // ADD(4)
                        return 0;

                    case 1:  // CMP(3)

                        instr |= 0x01500000UL | (((uint32_t)vD) << 16) | v8;
                        break;

                    case 2:  // MOV(3)
                        return 0;

                    case 3:  // BX
                        return 0;

                    default:
                        goto undefined;
                }
            } else {  // AND EOR LSL(2) LSR(2) ASR(2) ADC SBC ROR TST NEG CMP(2) CMN ORR MUL BIC MVN
                      // (in val_tabl order)
                static const uint32_t val_tabl[16] = {
                    0x00100000UL, 0x00300000UL, 0x01B00010UL, 0x01B00030UL,
                    0x01B00050UL, 0x00B00000UL, 0x00D00000UL, 0x01B00070UL,
                    0x01100000UL, 0x02700000UL, 0x01500000UL, 0x01700000UL,
                    0x01900000UL, 0x00100090UL, 0x01D00000UL, 0x01F00000UL};

                // 00 = none
                // 10 = bit0 val
                // 11 = bit3 val
                // MVN BIC MUL ORR CMN CMP(2) NEG TST ROR SBC ADC ASR(2) LSR(2) LSL(2) EOR AND

                const uint32_t use16 = 0x2AAE280AUL;  // 0010 1010 1010 1110 0010 1000 0000 1010
                const uint32_t use12 = 0xA208AAAAUL;  // 1010 0010 0000 1000 1010 1010 1010 1010
                const uint32_t use8 = 0x0800C3F0UL;   // 0000 1000 0000 0000 1100 0011 1111 0000
                const uint32_t use0 = 0xFFF3BEAFUL;   // 1111 1111 1111 0011 1011 1110 1010 1111
                uint8_t vals[4] = {0};

                vals[2] = (instrT & 7);
                vals[3] = (instrT >> 3) & 7;
                v8 = (instrT >> 6) & 15;
                instr |= val_tabl[v8];
                v8 <<= 1;
                instr |= ((uint32_t)(vals[(use16 >> v8) & 3UL])) << 16;
                instr |= ((uint32_t)(vals[(use12 >> v8) & 3UL])) << 12;
                instr |= ((uint32_t)(vals[(use8 >> v8) & 3UL])) << 8;
                instr |= ((uint32_t)(vals[(use0 >> v8) & 3UL])) << 0;
            }
            break;

        case 5:  // STR(2)  STRH(2) STRB(2) LDRSB LDR(2) LDRH(2) LDRB(2) LDRSH		(in val_tbl
                 // orver)
        {
            static const uint32_t val_tabl[8] = {0x07800000UL, 0x018000B0UL, 0x07C00000UL,
                                                 0x019000D0UL, 0x07900000UL, 0x019000B0UL,
                                                 0x07D00000UL, 0x019000F0UL};
            instr |= ((instrT >> 6) & 7) | ((instrT & 7) << 12) |
                     (((uint32_t)(instrT & 0x38)) << 13) | val_tabl[(instrT >> 9) & 7];
        } break;

        case 6:  // LDR(1) STR(1)	(bit11 set = ldr)

            instr |= ((instrT & 7) << 12) | (((uint32_t)(instrT & 0x38)) << 13) |
                     ((instrT >> 4) & 0x7C) | 0x05800000UL;
            if (instrT & 0x0800) instr |= 0x00100000UL;
            break;

        case 7:  // LDRB(1) STRB(1)	(bit11 set = ldrb)

            instr |= ((instrT & 7) << 12) | (((uint32_t)(instrT & 0x38)) << 13) |
                     ((instrT >> 6) & 0x1F) | 0x05C00000UL;
            if (instrT & 0x0800) instr |= 0x00100000UL;
            break;

        case 8:  // LDRH(1) STRH(1)	(bit11 set = ldrh)

            instr |= ((instrT & 7) << 12) | (((uint32_t)(instrT & 0x38)) << 13) |
                     ((instrT >> 5) & 0x0E) | ((instrT >> 1) & 0x300) | 0x01C000B0UL;
            if (instrT & 0x0800) instr |= 0x00100000UL;
            break;

        case 9:  // LDR(4) STR(3)	(bit11 set = ldr)

            instr |= ((instrT & 0x700) << 4) | ((instrT & 0xFF) << 2) | 0x058D0000UL;
            if (instrT & 0x0800) instr |= 0x00100000UL;
            break;

        case 10:  // ADD(5) ADD(6)	(bit11 set = add(6))
            return 0;

        case 11:  // ADD(7) SUB(4) PUSH POP BKPT

            if ((instrT & 0x0600) == 0x0400) {  // PUSH POP

                instr |= (instrT & 0xFF) | 0x000D0000UL;

                if (instrT & 0x0800) {  // POP

                    if (instrT & 0x0100) instr |= 0x00008000UL;
                    instr |= 0x08B00000UL;
                } else {  // PUSH

                    if (instrT & 0x0100) instr |= 0x00004000UL;
                    instr |= 0x09200000UL;
                }
            } else if (instrT & 0x0100) {
                goto undefined;
            } else
                switch ((instrT >> 9) & 7) {
                    case 0:  // ADD(7) SUB(4)

                        instr |= 0x020DDF00UL | (instrT & 0x7F) |
                                 ((instrT & 0x0080) ? 0x00400000UL : 0x00800000UL);
                        break;

                    case 7:  // BKPT

                        instr |= 0x01200070UL | (instrT & 0x0F) | ((instrT & 0xF0) << 4);
                        break;

                    default:

                        goto undefined;
                }
            break;

        case 12:  // LDMIA STMIA		(bit11 set = ldmia)
            instr |= 0x08800000UL | (((uint32_t)(instrT & 0x700)) << 8) | (instrT & 0xFF);
            if (instrT & 0x0800) instr |= 0x00100000UL;
            if (!((1UL << ((instrT >> 8) & 0x07)) & instrT))
                instr |= 0x00200000UL;  // set W bit if needed
            break;

        case 13:  // B(1), SWI, undefined instr space
            v8 = ((instrT >> 8) & 0x0F);
            if (v8 == 14) {  // undefined instr
                goto undefined;
            } else if (v8 == 15) {  // SWI
                instr |= 0x0F000000UL | (instrT & 0xFF);
            } else {  // B(1)
                instr = (((uint32_t)v8) << 28) | 0x0A000000UL | (instrT & 0xFF);
                if (instrT & 0x80) instr |= 0x00FFFF00UL;
            }
            break;

        case 14:  // B(2) BL BLX(1) undefined instr space
        case 15:
            v16 = (instrT & 0x7FF);
            switch ((instrT >> 11) & 3) {
                case 0:  // B(2)

                    instr |= 0x0A000000UL | v16;
                    if (instrT & 0x0400) instr |= 0x00FFF800UL;
                    break;

                case 1:  // BLX(1)_suffix
                case 2:  // BLX(1)_prefix BL_prefix
                case 3:  // BL_suffix
                    return 0;
            }

            if (instrT & 0x0800)
                goto undefined;  // avoid BLX_suffix and undefined instr space in there
            instr |= 0x0A000000UL | (instrT & 0x7FF);
            if (instrT & 0x0400) instr |= 0x00FFF800UL;
            break;
    }

    return instr;
    ;

undefined:

    return 0xE7F000F0UL | (instrT & 0x0F) |
           ((instrT & 0xFFF0)
            << 4);  // guranteed undefined instr, inside it we store the original thumb instr :)=-)
}

int main() {
    printf("const uint32_t table_thumb2arm[0x10000] = {\n");

    for (uint32_t i = 0; i < 0x10000; i++) {
        uint32_t translation = translateThumb(i);

        if (translation)
            printf("    %#010x", translateThumb(i));
        else
            printf("    0");

        printf(i == 0xffff ? "\n" : ",\n");
    }

    printf("};\n");
}
