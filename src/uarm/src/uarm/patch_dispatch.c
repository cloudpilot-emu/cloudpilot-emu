#include "patch_dispatch.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "syscall.h"
#include "util.h"

#define MAX_PENDING_TAILPATCH 32
#define MAX_PATCHES 50
#define PATCH_TABLE_SIZE 0xc00  // 3 * 0x400

struct Patch {
    uint32_t syscall;

    void* ctx;
    HeadpatchF headpatch;
    TailpatchF tailpatch;
};

struct PendingTailpatch {
    uint32_t registersAtInvocation[16];
    uint32_t returnAddress;

    const struct Patch* patch;
};

struct PatchDispatch {
    int8_t table;
    uint8_t countdown;

    uint8_t patchTable[PATCH_TABLE_SIZE];

    struct Patch patches[MAX_PATCHES];
    size_t nPatches;

    struct PendingTailpatch pendingTailpatches[MAX_PENDING_TAILPATCH];
    size_t nPendingTailpatches;
};

struct PatchDispatch* initPatchDispatch() {
    struct PatchDispatch* pd = malloc(sizeof(*pd));

    memset(pd, 0, sizeof(*pd));
    memset(pd->patchTable, 0xff, 0xc00);

    pd->table = -1;

    return pd;
}

void destroyPatchDispatch(struct PatchDispatch* pd) { free(pd); }

void patchDispatchOnLoadR12FromR9(struct PatchDispatch* pd, int32_t offset) {
    pd->table = -1;
    pd->countdown = 0;

    if (offset >= 0) return;
    offset = -offset;

    if (offset & 0x03 || offset > 12) return;

    pd->table = offset;
    pd->countdown = 2;
}

void patchDispatchOnLoadPcFromR12(struct PatchDispatch* pd, int32_t offset, uint32_t* registers) {
    if (pd->countdown != 1 || offset < 0 || offset & 0x03 || offset > 0xfff) return;

#ifdef TRACE_SYSCALLS
    const char* syscallName = getSyscallName(packSyscall(pd->table, offset));
    fprintf(stderr, "syscall from %#10x: %s\n", registers[14],
            syscallName ? syscallName : "[unknown]");
#endif

    const uint32_t key = (((pd->table >> 2) - 1) << 10) | (offset >> 2);
    const uint8_t patchIdx = pd->patchTable[key];
    if (patchIdx == 0xff) return;

    const struct Patch* patch = &pd->patches[patchIdx];

    if (patch->headpatch) patch->headpatch(patch->ctx, patch->syscall, registers);
    if (patch->tailpatch) {
        if (pd->nPendingTailpatches == MAX_PENDING_TAILPATCH) {
            fprintf(stderr, "too many pending tailpatches, skipping tailpatch for %#10x\n",
                    packSyscall(pd->table, offset));
            return;
        }

        uint8_t tailpatchIdx = pd->nPendingTailpatches++;
        struct PendingTailpatch* tailpatch = &pd->pendingTailpatches[tailpatchIdx];

        tailpatch->patch = patch;
        tailpatch->returnAddress = registers[14] & ~0x01;
        memcpy(tailpatch->registersAtInvocation, registers,
               sizeof(tailpatch->registersAtInvocation));
    }
}

void patchOnBeforeExecute(struct PatchDispatch* pd, uint32_t* registers) {
    if (pd->countdown != 0) pd->countdown--;
    if (pd->nPendingTailpatches == 0) return;

    for (size_t i = 0; i < pd->nPendingTailpatches; i++) {
        const struct PendingTailpatch* pendingTailpatch = &pd->pendingTailpatches[i];

        if (pendingTailpatch->returnAddress != registers[15] ||
            pendingTailpatch->registersAtInvocation[13] != registers[13])
            continue;

        if (pendingTailpatch->patch->tailpatch)
            pendingTailpatch->patch->tailpatch(pendingTailpatch->patch->ctx,
                                               pendingTailpatch->patch->syscall,
                                               pendingTailpatch->registersAtInvocation, registers);

        if (i != pd->nPendingTailpatches - 1)
            pd->pendingTailpatches[i] = pd->pendingTailpatches[pd->nPendingTailpatches];

        pd->nPendingTailpatches--;
    }
}

void patchDispatchAddPatch(struct PatchDispatch* pd, uint32_t syscall, HeadpatchF headpatch,
                           TailpatchF tailpatch, void* ctx) {
    const uint32_t key = (((syscall >> 14) - 1) << 10) | ((syscall & 0xfff) >> 2);
    if (key >= PATCH_TABLE_SIZE) return;

    struct Patch* patch;
    if (pd->patchTable[key] != 0xff) {
        fprintf(stderr, "WARNING: overwriting existing patch for %#10x", syscall);
        patch = &pd->patches[pd->patchTable[key]];
    } else {
        uint8_t patchIdx = pd->nPatches++;
        if (patchIdx >= MAX_PATCHES) ERR("WARNING: max number of patches exceeded\n");

        pd->patchTable[key] = patchIdx;
        patch = &pd->patches[patchIdx];
    }

    patch->syscall = syscall;
    patch->ctx = ctx;
    patch->headpatch = headpatch;
    patch->tailpatch = tailpatch;
}
