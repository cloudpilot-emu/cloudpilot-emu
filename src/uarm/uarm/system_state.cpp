#include "system_state.h"

#include <cstdlib>
#include <cstring>
#include <iostream>

#include "cputil.h"
#include "savestate/savestateAll.h"

#define SAVESTATE_VERSION 0

struct SystemState {
    bool uiInitialized;
    uint32_t osVersion;

    template <typename T>
    void DoSaveLoad(T& chunkHelper);
};

static void dump(SystemState* state) {
    std::cout << "system state " << state->osVersion << " " << state->uiInitialized << std::endl;
}

SystemState* createSystemState() {
    SystemState* systemState = static_cast<SystemState*>(malloc(sizeof(SystemState)));
    memset(systemState, 0, sizeof(SystemState));

    systemState->osVersion = SYSTEM_STATE_OS_VERSION_UNDEFINED;

    return systemState;
}

bool systemStateIsUiInitialized(struct SystemState* state) { return state->uiInitialized; }

void systemStateSetUiInitialized(struct SystemState* state, bool uiInitialized) {
    state->uiInitialized = uiInitialized;
    dump(state);
}

uint32_t systemStateGetOsVersion(struct SystemState* state) { return state->osVersion; }

void systemStateSetOsVersion(struct SystemState* state, uint32_t osVersion) {
    state->osVersion = osVersion;
    dump(state);
}

template <typename T>
void systemStateSave(SystemState* state, T& savestate) {
    auto chunk = savestate.GetChunk(ChunkType::systemState, SAVESTATE_VERSION);
    if (!chunk) ERR("unable to allocate chunk");

    SaveChunkHelper helper(*chunk);
    state->DoSaveLoad(helper);
}

template <typename T>
void systemStateLoad(SystemState* state, T& loader) {
    auto chunk = loader.GetChunk(ChunkType::systemState, SAVESTATE_VERSION, "system state");
    if (!chunk) {
        state->uiInitialized = true;
        state->osVersion = SYSTEM_STATE_OS_VERSION_UNDEFINED;
    } else {
        LoadChunkHelper helper(*chunk);
        state->DoSaveLoad(helper);
    }

    dump(state);
}

template <typename T>
void SystemState::DoSaveLoad(T& chunkHelper) {
    chunkHelper.DoBool(uiInitialized).Do32(osVersion);
}

template void systemStateSave<Savestate<ChunkType>>(SystemState* state,
                                                    Savestate<ChunkType>& savestate);
template void systemStateSave<SavestateProbe<ChunkType>>(SystemState* state,
                                                         SavestateProbe<ChunkType>& savestate);
template void systemStateLoad<SavestateLoader<ChunkType>>(SystemState* state,
                                                          SavestateLoader<ChunkType>& loader);