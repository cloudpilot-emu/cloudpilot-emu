#include "pace_patch.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint32_t decodeBlBlx(void* rom, uint32_t addr) {
    const uint32_t instruction = *(uint32_t*)(rom + addr);
    const uint8_t prefix = instruction >> 24;

    int32_t offset = instruction << 8;
    offset >>= 6;
    offset += 8;

    switch (prefix) {
        case 0xeb:
            return addr + offset;

        case 0xfa:
            return addr + offset + 1;

        case 0xfb:
            return addr + offset + 3;

        default:
            return 0;
    }
}

struct PacePatch* initPacePatch(uint32_t romBase, void* rom, size_t romSize) {
    static const uint8_t paceStartPattern[] = {
        0xF1, 0x4F, 0x2D, 0xE9, 0x00, 0x00, 0x9D, 0xE5, 0x44, 0xA0, 0x90, 0xE5, 0x48, 0x40, 0x90,
        0xE5, 0x04, 0x50, 0x80, 0xE2, 0x24, 0x60, 0x80, 0xE2, 0xB2, 0x70, 0xDA, 0xE0, 0x44, 0xB0,
        0x8F, 0xE2, 0x01, 0x00, 0x00, 0xEA, 0xC0, 0x0C, 0x14, 0xE3, 0x47, 0x17, 0x00, 0x1A, 0x07,
        0x84, 0xA0, 0xE1, 0x27, 0x84, 0x88, 0xE1, 0xF0, 0x0C, 0x08, 0xE2, 0x20, 0x05, 0x9B, 0xE7,
        0xB2, 0x70, 0xDA, 0xE0, 0x00, 0xF0, 0x8B, 0xE0, 0x00, 0x00, 0x9D, 0xE5, 0x02, 0xA0, 0x4A,
        0xE2, 0x44, 0xA0, 0x80, 0xE5, 0x48, 0x40, 0x80, 0xE5, 0x00, 0x80, 0x80, 0xE5, 0x18, 0x90,
        0x9D, 0xE5, 0x00, 0x00, 0xA0, 0xE3, 0xF2, 0x4F, 0xBD, 0xE8, 0x1E, 0xFF, 0x2F, 0xE1};

    struct PacePatch* patch = malloc(sizeof(*patch));
    memset(patch, 0, sizeof(*patch));

    if (!rom) return patch;

    uint32_t paceLocation = 0;
    for (uint32_t i = 0; i < romSize - sizeof(paceStartPattern); i += 4) {
        if (memcmp(rom + i, paceStartPattern, sizeof(paceStartPattern)) != 0) continue;

        if (paceLocation != 0) {
            fprintf(stderr,
                    "PACE signature found in both %#010x and %#010x, PACE will not be patched\n",
                    paceLocation, i);
            return patch;
        }

        paceLocation = i;
    }

    if (!paceLocation) {
        fprintf(stderr, "unable to locate PACE, will not be patched\n");
        return patch;
    }

    const uint32_t enterPace = romBase + paceLocation;
    fprintf(stderr, "found PACE entry at %#010x\n", enterPace);

    const uint32_t calloutSyscall = decodeBlBlx(rom, enterPace + 0x25894 - 0x1fd28);

    if (!calloutSyscall) {
        fprintf(stderr, "unable to locate callout for syscall, PACE will not be patched\n");
        return patch;
    }
    fprintf(stderr, "found PACE callout for syscall at %#010x\n", calloutSyscall);

    ((uint32_t*)rom)[(paceLocation >> 2)] = INSTR_PACE_ENTER;
    ((uint32_t*)rom)[(paceLocation >> 2) + 1] = INSTR_PACE_RESUME;
    ((uint32_t*)rom)[(paceLocation >> 2) + 2] = INSTR_PACE_RETURN_FROM_CALLOUT;

    patch->enterPace = enterPace;
    patch->resumePace = patch->enterPace + 4;
    patch->returnFromCallout = patch->enterPace + 8;
    patch->calloutSyscall = calloutSyscall;

    fprintf(stderr, "patching PACE\n");

    return patch;
}
