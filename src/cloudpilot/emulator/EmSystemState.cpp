#include "EmSystemState.h"

#include "ChunkHelper.h"
#include "Logging.h"
#include "Savestate.h"
#include "SavestateLoader.h"
#include "SavestateProbe.h"

EmSystemState gSystemState;

namespace {
    constexpr uint32 SAVESTATE_VERSION = 2;
}

void EmSystemState::Initialize() {
    uiInitialized = false;
    screenLowWatermark = screenHighWatermark = 0;
    screenState = ScreenState::needsFullRefresh;
    setupComplete = false;
    osVersion = 0;
    hotsyncUserName = "";
}

void EmSystemState::Reset() {
    uiInitialized = false;
    screenLowWatermark = screenHighWatermark = 0;
    screenState = ScreenState::needsFullRefresh;
    setupComplete = false;
}

template <typename T>
void EmSystemState::Save(T& savestate) {
    typename T::chunkT* chunk = savestate.GetChunk(ChunkType::systemState);
    if (!chunk) return;

    chunk->Put32(SAVESTATE_VERSION);

    SaveChunkHelper helper(*chunk);
    DoSaveLoad(helper, SAVESTATE_VERSION);
}

template void EmSystemState::Save(Savestate& savestate);
template void EmSystemState::Save(SavestateProbe& savestate);

void EmSystemState::Load(SavestateLoader& loader) {
    Chunk* chunk = loader.GetChunk(ChunkType::systemState);
    if (!chunk) return;

    const uint32 version = chunk->Get32();
    if (version > SAVESTATE_VERSION) {
        logging::printf("error restoring SystemState: savestate version mismatch");
        loader.NotifyError();

        return;
    }

    setupComplete = true;

    LoadChunkHelper helper(*chunk);
    DoSaveLoad(helper, version);

    screenLowWatermark = screenHighWatermark = 0;
    screenState = ScreenState::needsFullRefresh;
}

template <typename T>
void EmSystemState::DoSaveLoad(T& helper, uint32 version) {
    helper.Do32(osVersion).DoBool(uiInitialized).DoString(hotsyncUserName, dlkMaxUserNameLength);

    if (version > 1) {
        helper.DoBool(setupComplete);
    }
}

void EmSystemState::SetOSVersion(uint32 version) { osVersion = version; }

uint32 EmSystemState::OSVersion(void) const { return osVersion; }

uint32 EmSystemState::OSMajorMinorVersion(void) const {
    return OSMajorVersion() * 10 + OSMinorVersion();
}

uint32 EmSystemState::OSMajorVersion(void) const { return sysGetROMVerMajor(osVersion); }

uint32 EmSystemState::OSMinorVersion(void) const { return sysGetROMVerMinor(osVersion); }

void EmSystemState::SetUIInitialized() { uiInitialized = true; }

bool EmSystemState::IsUIInitialized() const { return uiInitialized; }

void EmSystemState::SetSetupComplete() { setupComplete = true; }

bool EmSystemState::IsSetupComplete() const { return setupComplete; }

void EmSystemState::SetHotsyncUserName(string hotsyncUserName) {
    if (hotsyncUserName.size() > dlkMaxUserNameLength) return;

    this->hotsyncUserName = hotsyncUserName;
}
const string& EmSystemState::GetHotsyncUserName() const { return hotsyncUserName; };

bool EmSystemState::IsScreenDirty() const { return screenState != ScreenState::clean; }

void EmSystemState::MarkScreenClean() {
    screenState = ScreenState::clean;
    screenLowWatermark = screenHighWatermark = 0;

    onMarkScreenClean.Dispatch();
}

emuptr EmSystemState::GetScreenHighWatermark() const { return screenHighWatermark; }
emuptr EmSystemState::GetScreenLowWatermark() const { return screenLowWatermark; }
bool EmSystemState::ScreenRequiresFullRefresh() const {
    return screenState == ScreenState::needsFullRefresh;
}
