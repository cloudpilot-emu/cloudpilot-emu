#include "peephole.h"

#include <stdio.h>
#include <string.h>

static const uint32_t sig_adc_udivmod_first[] = {
    0xE3A02000, 0xE070C1A1, 0x3A000020, 0xE070C421, 0x3A00000F, 0xE1A00400, 0xE38224FF,
    0xE070C221, 0x3A000017, 0xE070C421, 0x3A000009, 0xE1A00400, 0xE38228FF, 0xE070C421,
    0x21A00400, 0x23822CFF, 0xE070C221, 0x3A00000E, 0xE270C000};

static const uint32_t sig_adc_udivmod_second[] = {
    0x21A00420, 0xE070C3A1, 0x20411380, 0xE0A22002, 0xE070C321, 0x20411300, 0xE0A22002,
    0xE070C2A1, 0x20411280, 0xE0A22002, 0xE070C221, 0x20411200, 0xE0A22002, 0xE070C1A1,
    0x20411180, 0xE0A22002, 0xE070C121, 0x20411100, 0xE0A22002, 0xE070C0A1, 0x20411080,
    0xE0A22002, 0xE070C001, 0x20411000, 0xE0B22002, 0x2AFFFFE5, 0xE1A00002, 0xE12FFF1E};

static const uint32_t sig_adc_sdivmod_first[] = {
    0xE2102480, 0x42600000, 0xE0323041, 0x22611000, 0xE070C1A1, 0x3A000020, 0xE070C421, 0x3A00000F,
    0xE1A00400, 0xE38224FF, 0xE070C221, 0x3A000017, 0xE070C421, 0x3A000009, 0xE1A00400, 0xE38228FF,
    0xE070C421, 0x21A00400, 0x23822CFF, 0xE070C221, 0x3A00000E, 0xE270C000};

static const uint32_t sig_adc_sdivmod_second[] = {
    0x21A00420, 0xE070C3A1, 0x20411380, 0xE0A22002, 0xE070C321, 0x20411300, 0xE0A22002, 0xE070C2A1,
    0x20411280, 0xE0A22002, 0xE070C221, 0x20411200, 0xE0A22002, 0xE070C1A1, 0x20411180, 0xE0A22002,
    0xE070C121, 0x20411100, 0xE0A22002, 0xE070C0A1, 0x20411080, 0xE0A22002, 0xE070C001, 0x20411000,
    0xE0B22002, 0x2AFFFFE5, 0xE0320FC3, 0xE0800FA3, 0x22611000, 0xE12FFF1E};

static const uint32_t sig_adc_udiv10[] = {0xE240100A, 0xE0400120, 0xE0800220, 0xE0800420,
                                          0xE0800820, 0xE1A001A0, 0xE0802100, 0xE0511082,
                                          0x52800001, 0x4281100A, 0xE12FFF1E};

static const uint32_t sig_adc_sdiv10[] = {
    0xE1B03000, 0x42600000, 0xE240100A, 0xE0400120, 0xE0800220, 0xE0800420, 0xE0800820, 0xE1A001A0,
    0xE0802100, 0xE0511082, 0x52800001, 0x4281100A, 0xE1B03003, 0x42600000, 0x42611000, 0xE12FFF1E};

static const uint32_t sig_adc_memcpy[] = {
    0xE92D4010, 0xE2522020, 0x3A000005, 0x28B15018, 0x28A05018, 0x28B15018, 0x28A05018,
    0x22522020, 0x2AFFFFF9, 0xE1B0CE02, 0x28B15018, 0x28A05018, 0x48B10018, 0x48A00018,
    0xE8BD4010, 0xE1B0CF02, 0x24913004, 0x24803004, 0x012FFF1E, 0xE1B02F82, 0x44D12001,
    0x24D13001, 0x24D1C001, 0x44C02001, 0x24C03001, 0x24C0C001, 0xE12FFF1E};

const size_t OFFSET_PEEPHOLE_ADS_UDIVMOD_DIVISION_BY_ZERO = sizeof(sig_adc_udivmod_first);
const size_t OFFSET_PEEPHOLE_ADS_SDIVMOD_DIVISION_BY_ZERO = sizeof(sig_adc_sdivmod_first);

static void optimizeAt_ADC_udivmod(uint32_t* code, size_t size) {
    if (size < sizeof(sig_adc_udivmod_first) + sizeof(sig_adc_udivmod_second) + 4) return;

    if (memcmp(code, sig_adc_udivmod_first, sizeof(sig_adc_udivmod_first)) != 0) return;
    if ((code[sizeof(sig_adc_udivmod_first) / 4] >> 24) != 0x2a) return;
    if (memcmp(code + sizeof(sig_adc_udivmod_first) / 4 + 1, sig_adc_udivmod_second,
               sizeof(sig_adc_udivmod_second)) != 0)
        return;

    *code = INSTR_PEEPHOLE_ADS_UDIVMOD;
}

static void optimizeAt_ADC_sdivmod(uint32_t* code, size_t size) {
    if (size < sizeof(sig_adc_sdivmod_first) + sizeof(sig_adc_sdivmod_second) + 4) return;

    if (memcmp(code, sig_adc_sdivmod_first, sizeof(sig_adc_sdivmod_first)) != 0) return;
    if ((code[sizeof(sig_adc_sdivmod_first) / 4] >> 24) != 0x2a) return;
    if (memcmp(code + sizeof(sig_adc_sdivmod_first) / 4 + 1, sig_adc_sdivmod_second,
               sizeof(sig_adc_sdivmod_second)) != 0)
        return;

    *code = INSTR_PEEPHOLE_ADS_SDIVMOD;
}

static void optimizeAt_ADC_udiv10(uint32_t* code, size_t size) {
    if (size < sizeof(sig_adc_udiv10)) return;
    if (memcmp(code, sig_adc_udiv10, sizeof(sig_adc_udiv10)) != 0) return;

    *code = INSTR_PEEPHOLE_ADS_UDIV10;
}

static void optimizeAt_ADC_sdiv10(uint32_t* code, size_t size) {
    if (size < sizeof(sig_adc_sdiv10)) return;
    if (memcmp(code, sig_adc_sdiv10, sizeof(sig_adc_sdiv10)) != 0) return;

    *code = INSTR_PEEPHOLE_ADS_SDIV10;
}

static void optimizeAt_ADC_memcpy(uint32_t* code, size_t size) {
    if (size < sizeof(sig_adc_memcpy)) return;
    if (memcmp(code, sig_adc_memcpy, sizeof(sig_adc_memcpy)) != 0) return;

    *code = INSTR_PEEPHOLE_ADS_MEMCPY;
}

void peepholeOptimize(uint32_t* code, size_t size) {
    for (size = size & ~0x03; size > 0; size -= 4, code++) {
        optimizeAt_ADC_udivmod(code, size);
        optimizeAt_ADC_sdivmod(code, size);
        optimizeAt_ADC_udiv10(code, size);
        optimizeAt_ADC_sdiv10(code, size);
        optimizeAt_ADC_memcpy(code, size);
    }
}
