#include "patch_dispatch.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CPU.h"
#include "Logging.h"
#include "cputil.h"
#include "savestate/savestateAll.h"
#include "syscall.h"

#define MAX_PENDING_TAILPATCH 4
#define MAX_PATCHES 50
#define PATCH_TABLE_SIZE 0xc00  // 3 * 0x400

#define SAVESTATE_VERSION 0

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

struct SerializedTailpatch {
    uint32_t registersAtInvocation[16];
    uint32_t returnAddress;

    uint32_t syscall;

    template <typename T>
    void DoSaveLoad(T& chunkHelper) {
        chunkHelper.DoBuffer32(registersAtInvocation, sizeof(registersAtInvocation) >> 2)
            .Do32(returnAddress)
            .Do32(syscall);
    }
};

struct PatchDispatch {
    int8_t table;
    uint8_t countdown;

    uint8_t patchTable[PATCH_TABLE_SIZE];

    struct Patch patches[MAX_PATCHES];
    uint8_t nPatches;

    struct PendingTailpatch pendingTailpatches[MAX_PENDING_TAILPATCH];
    struct SerializedTailpatch serializedTailpatches[MAX_PENDING_TAILPATCH];
    uint8_t nPendingTailpatches;

    struct ArmCpu* cpu;

    template <typename T>
    void DoSaveLoad(T& chunkHelper);
};

struct PatchDispatch* initPatchDispatch() {
    struct PatchDispatch* pd = (struct PatchDispatch*)malloc(sizeof(*pd));

    memset(pd, 0, sizeof(*pd));
    memset(pd->patchTable, 0xff, 0xc00);

    pd->table = -1;

    return pd;
}

static void updateSlowPath(struct PatchDispatch* pd) {
    if (pd->countdown || pd->nPendingTailpatches) {
        cpuSetSlowPath(pd->cpu, SLOW_PATH_REASON_PATCH_PENDING);
    } else {
        cpuClearSlowPath(pd->cpu, SLOW_PATH_REASON_PATCH_PENDING);
    }
}

void destroyPatchDispatch(struct PatchDispatch* pd) { free(pd); }

void patchDispatchOnLoadR12FromR9(struct PatchDispatch* pd, int32_t offset) {
    pd->table = -1;
    pd->countdown = 0;

    if (offset >= 0) return updateSlowPath(pd);
    offset = -offset;

    if (offset & 0x03 || offset > 12) return updateSlowPath(pd);

    pd->table = offset;
    pd->countdown = 2;

    updateSlowPath(pd);
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

        updateSlowPath(pd);
    }
}

void patchOnBeforeExecute(struct PatchDispatch* pd, uint32_t* registers) {
    if (pd->countdown != 0) {
        pd->countdown--;
        updateSlowPath(pd);
    }

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

        if (i != static_cast<size_t>(pd->nPendingTailpatches - 1))
            pd->pendingTailpatches[i] = pd->pendingTailpatches[pd->nPendingTailpatches];

        pd->nPendingTailpatches--;
        updateSlowPath(pd);
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

void patchDispatchSetCpu(struct PatchDispatch* pd, struct ArmCpu* cpu) { pd->cpu = cpu; }

template <typename T>
void patchDispatchSave(PatchDispatch* pd, T& savestate) {
    auto chunk = savestate.GetChunk(ChunkType::patchDispatch, SAVESTATE_VERSION);
    if (!chunk) ERR("unable to allocate chunk");

    for (size_t i = 0; i < pd->nPendingTailpatches; i++) {
        SerializedTailpatch& serializedTailpatch(pd->serializedTailpatches[i]);
        PendingTailpatch& pendingTailpatch(pd->pendingTailpatches[i]);

        memcpy(&serializedTailpatch.registersAtInvocation[0],
               &pendingTailpatch.registersAtInvocation[0],
               sizeof(serializedTailpatch.registersAtInvocation));

        serializedTailpatch.returnAddress = pendingTailpatch.returnAddress;
        serializedTailpatch.syscall = pendingTailpatch.patch->syscall;
    }

    SaveChunkHelper helper(*chunk);
    pd->DoSaveLoad(helper);
}

template <typename T>
void patchDispatchLoad(PatchDispatch* pd, T& loader) {
    auto chunk = loader.GetChunk(ChunkType::patchDispatch, SAVESTATE_VERSION, "patchDispatch");
    if (!chunk) return;

    LoadChunkHelper helper(*chunk);
    pd->DoSaveLoad(helper);

    const uint8_t loadedTalpatchesCount = pd->nPendingTailpatches;
    pd->nPendingTailpatches = 0;

    for (size_t i = 0; i < loadedTalpatchesCount; i++) {
        SerializedTailpatch& serializedTailpatch(pd->serializedTailpatches[i]);
        Patch* patch = nullptr;

        for (size_t j = 0; j < pd->nPatches; j++) {
            if (pd->patches[j].syscall != serializedTailpatch.syscall) continue;

            patch = &pd->patches[j];
            break;
        }

        if (!patch) {
            logPrintf("skipping tailpatch for unpatched syscall 0x%08x\n",
                      serializedTailpatch.syscall);
            continue;
        }

        PendingTailpatch& pendingTailpatch(pd->pendingTailpatches[pd->nPendingTailpatches++]);

        memcpy(&pendingTailpatch.registersAtInvocation[0],
               &serializedTailpatch.registersAtInvocation[0],
               sizeof(pendingTailpatch.registersAtInvocation));

        pendingTailpatch.returnAddress = serializedTailpatch.returnAddress;
        pendingTailpatch.patch = patch;
    }

    updateSlowPath(pd);
}

template <typename T>
void PatchDispatch::DoSaveLoad(T& chunkHelper) {
    for (size_t i = 0; i < MAX_PENDING_TAILPATCH; i++)
        serializedTailpatches[i].DoSaveLoad(chunkHelper);

    chunkHelper.Do(typename T::Pack8() << table << countdown << nPendingTailpatches);
}

template void patchDispatchSave<Savestate<ChunkType>>(PatchDispatch* pd,
                                                      Savestate<ChunkType>& savestate);
template void patchDispatchSave<SavestateProbe<ChunkType>>(PatchDispatch* pd,
                                                           SavestateProbe<ChunkType>& savestate);
template void patchDispatchLoad<SavestateLoader<ChunkType>>(PatchDispatch* pd,
                                                            SavestateLoader<ChunkType>& loader);