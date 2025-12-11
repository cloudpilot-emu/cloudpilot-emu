#include "EmSession.h"

#include <functional>

#include "CallbackManager.h"
#include "Chars.h"
#include "Debugger.h"
#include "EmBankSRAM.h"
#include "EmCPU.h"
#include "EmHAL.h"
#include "EmLowMem.h"
#include "EmMemory.h"
#include "EmPalmOS.h"
#include "EmPatchMgr.h"
#include "EmSystemState.h"
#include "ExternalStorage.h"
#include "Logging.h"
#include "MetaMemory.h"
#include "Miscellaneous.h"
#include "NetworkProxy.h"
#include "Platform.h"
#include "ROMStubs.h"
#include "SessionImage.h"
#include "SuspendManager.h"
#include "savestate/ChunkHelper.h"
#include "savestate/Savestate.h"
#include "savestate/SavestateLoader.h"
#include "savestate/SavestateProbe.h"

namespace {
    constexpr uint32 SAVESTATE_VERSION = 3;

    constexpr int MIN_CYCLES_BETWEEN_BUTTON_EVENTS = 400000;

    constexpr uint32 YIELD_MEMMGR_LIMIT = 10000000;
    constexpr uint32 EXECUTE_SUBROUTINE_LIMIT = 100000000;

    constexpr double DEFAULT_CLOCK_FACTOR = 0.5;

    EmSession _gSession;

    uint32 CurrentDate() {
        uint32 year, month, day;

        Platform::GetDate(year, month, day);

        return (year << 16) | (month << 8) | day;
    }
}  // namespace

EmSession* gSession = &_gSession;

bool EmSession::Initialize(EmDevice* device, uint8* romImage, size_t romLength) {
    if (isInitialized) {
        Deinitialize();
    }
    isInitialized = true;

    this->romImage = unique_ptr<uint8[]>(romImage);
    romSize = romLength;

    onSystemClockChangeHandle =
        EmHAL::onSystemClockChange.AddHandler(bind(&EmSession::RecalculateClocksPerSecond, this));

    this->device.reset(device);

    cpu.reset(device->CreateCPU(this));

    if (!Memory::Initialize(romImage, romLength, *device)) {
        return false;
    }
    EmPalmOS::Initialize();

    systemCycles = 0;
    extraCycles = 0;
    Reset(ResetType::soft);

    gSystemState.Initialize();

    RecalculateClocksPerSecond();

    dateCheckedAt = 0;
    lastDate = CurrentDate();

    gDebugger.Reset();
    gDebugger.ResetBreakMode();
    gDebugger.ResetAppRegion();

    UpdateUARTModeSync();

    return true;
}

void EmSession::Deinitialize() {
    if (!isInitialized) return;

    gNetworkProxy.Reset();
    SuspendManager::Reset();
    EmPalmOS::Dispose();
    CallbackManager::Clear();
    Memory::Dispose();
    MetaMemory::Clear();

    cpu.reset();
    device.reset();

    EmHAL::onSystemClockChange.RemoveHandler(onSystemClockChangeHandle);
    gSystemState.Reset();

    bankResetScheduled = false;
    resetScheduled = false;
    nestLevel = 0;
    subroutineReturn = false;

    systemCycles = 0;
    extraCycles = 0;
    holdingBootKeys = false;

    dateCheckedAt = 0;
    lastDate = 0;

    romSize = 0;
    romImage.reset();
    savestate.Reset();

    gExternalStorage.UnmountAll();

    isInitialized = false;
}

bool EmSession::SaveImage(SessionImage& image) {
    EmAssert(romImage);

    image.SetRomImage(romImage.get(), romSize)
        .SetMemoryImage(GetMemoryPtr(), GetMemorySize())
        .SetDeviceId(device->GetIDString());

    if (savestate.Save(*this)) {
        image.SetSavestate(savestate.GetBuffer(), savestate.GetSize());
    } else {
        image.SetSavestate(nullptr, 0);
        logPrintf("failed to save savestate");

        return false;
    }

    return image.Serialize();
}

bool EmSession::LoadImage(SessionImage& image) {
    EmDevice* device = new EmDevice(image.GetDeviceId());
    if (device->GetIDString() != device->GetIDString()) {
        logPrintf("device id mismatch");
        return false;
    }

    unique_ptr<uint8[]> romImage = make_unique<uint8[]>(image.GetRomImageSize());
    memcpy(romImage.get(), image.GetRomImage(), image.GetRomImageSize());

    if (!Initialize(device, romImage.get(), image.GetRomImageSize())) {
        logPrintf("failed to initialize session");
        return false;
    }

    romImage.release();

    size_t memorySize = image.GetMemoryImageSize();
    void* memoryImage = image.GetMemoryImage();
    uint32 version = image.GetVersion();

    if (memorySize > 0) {
        if (version >= 4) {
            if (!EmMemory::LoadMemoryV4(memoryImage, memorySize)) {
                logPrintf("failed to restore memory (V4)");
                return false;
            }
        } else if (version >= 2) {
            if (!EmMemory::LoadMemoryV2(memoryImage, memorySize)) {
                logPrintf("failed to restore memory (V2)");
                return false;
            }
        } else if (!EmMemory::LoadMemoryV1(memoryImage, memorySize)) {
            logPrintf("failed to restore memory (V1)");
            return false;
        }
    }

    size_t savestateSize = image.GetSavestateSize();
    void* savestate = image.GetSavestate();

    if (savestateSize > 0 && !Load(savestateSize, static_cast<uint8*>(savestate))) {
        logPrintf("failed to restore savestate");
        return false;
    }

    gExternalStorage.Remount();

    return true;
}

template <typename T>
void EmSession::Save(T& savestate) {
    EmAssert(nestLevel == 0);
    EmAssert(device);
    EmAssert(cpu);
    EmAssert(!subroutineReturn);

    typename T::chunkT* chunk = savestate.GetChunk(ChunkType::session);
    if (!chunk) return;

    chunk->Put32(SAVESTATE_VERSION);
    chunk->PutString(device->GetIDString(), 16);

    SaveChunkHelper helper(*chunk);
    DoSaveLoad(helper, SAVESTATE_VERSION);

    cpu->Save(savestate);
    EmPatchMgr::Save(savestate);
    gSystemState.Save(savestate);
    Memory::Save(savestate);
    gExternalStorage.Save(savestate);
}

template void EmSession::Save(Savestate<ChunkType>& savestate);
template void EmSession::Save(SavestateProbe<ChunkType>& savestate);

void EmSession::Load(SavestateLoader<ChunkType>& loader) {
    EmAssert(device);
    EmAssert(cpu);

    Chunk* chunk = loader.GetChunk(ChunkType::session);
    if (!chunk) {
        loader.NotifyError();
        return;
    }

    uint32 version = chunk->Get32();
    if (version > SAVESTATE_VERSION) {
        logPrintf("unable to restore session: savestate version mismatch");
        loader.NotifyError();

        return;
    }

    if (chunk->GetString(16) != device->GetIDString()) {
        logPrintf("unable to restore session: device ID does not match savestate");
    }

    nestLevel = 0;
    extraCycles = 0;
    subroutineReturn = false;

    buttonEventQueue.Clear();

    LoadChunkHelper helper(*chunk);
    DoSaveLoad(helper, version);

    lastButtonEventReadAt = systemCycles;

    gSystemState.Load(loader);
    cpu->Load(loader);
    EmPatchMgr::Load(loader);
    Memory::Load(loader);
    gExternalStorage.Load(loader);

    SetCurrentDate();
    dateCheckedAt = systemCycles;
    if (version <= 2) lastDate = 0;

    RecalculateClocksPerSecond();
}

bool EmSession::Save() {
    if (SuspendManager::IsSuspended()) {
        logPrintf("unable to save state while the emulator is suspended");
        return false;
    }

    return savestate.Save(*this);
}

bool EmSession::Load(size_t size, uint8* buffer) {
    SavestateLoader<ChunkType> loader;

    if (!loader.Load(buffer, size, *this)) {
        Reset(ResetType::soft);

        return false;
    }

    Memory::ResetBankHandlers();

    return true;
}

Savestate<ChunkType>& EmSession::GetSavestate() { return savestate; }

pair<size_t, uint8*> EmSession::GetRomImage() {
    EmAssert(romImage);

    return pair(romSize, romImage.get());
}

template <typename T>
void EmSession::DoSaveLoad(T& helper, uint32 version) {
    helper.Do(typename T::BoolPack() << bankResetScheduled << resetScheduled << holdingBootKeys)
        .Do(typename T::Pack8() << *reinterpret_cast<uint8*>(&resetType)
                                << *reinterpret_cast<uint8*>(&bootKeysType))
        .Do64(systemCycles);

    if (version > 2) {
        helper.Do32(lastDate);
    }
}

void EmSession::ScheduleReset(ResetType resetType) {
    resetScheduled = true;
    this->resetType = resetType;

    EmASSERT(cpu);
    cpu->CheckAfterCycle();
}

void EmSession::Reset(ResetType resetType) {
    EmAssert(cpu);
    EmAssert(nestLevel == 0);

    gNetworkProxy.Reset();
    SuspendManager::Reset();
    Memory::Reset(resetType != ResetType::sys);
    cpu->Reset(resetType != ResetType::sys);
    EmPalmOS::Reset();
    gSystemState.Reset();

    bankResetScheduled = false;
    resetScheduled = false;

    if (resetType != ResetType::sys) {
        buttonEventQueue.Clear();
        lastButtonEventReadAt = 0;

        switch (resetType) {
            case ResetType::hard:
                holdingBootKeys = true;
                bootKeysType = ResetType::hard;

                EmHAL::DispatchButtonEvent(
                    ButtonEvent(ButtonEvent::Button::power, ButtonEvent::Type::press));

                break;

            case ResetType::noext:
                holdingBootKeys = true;
                bootKeysType = ResetType::noext;

                EmHAL::DispatchButtonEvent(
                    ButtonEvent(ButtonEvent::Button::rockerUp, ButtonEvent::Type::press));

                break;

            default:
                break;
        }
    }
}

bool EmSession::IsPowerOn() { return !EmHAL::GetAsleep(); }

bool EmSession::IsCpuStopped() {
    EmAssert(cpu);

    return cpu->Stopped();
}

void EmSession::ReleaseBootKeys() {
    if (!holdingBootKeys) return;

    switch (bootKeysType) {
        case ResetType::hard:
            EmHAL::DispatchButtonEvent(
                ButtonEvent(ButtonEvent::Button::power, ButtonEvent::Type::release));

            break;

        case ResetType::noext:
            EmHAL::DispatchButtonEvent(
                ButtonEvent(ButtonEvent::Button::rockerUp, ButtonEvent::Type::release));

            break;

        default:
            break;
    }

    holdingBootKeys = false;
}

void EmSession::ExecuteSpecial() {
    if (resetScheduled) {
        resetScheduled = false;
        bankResetScheduled = false;

        this->Reset(resetType);
    }

    if (bankResetScheduled) {
        bankResetScheduled = false;

        Memory::ResetBankHandlers();
    }
}

bool EmSession::CheckForBreak() const { return (IsNested() && subroutineReturn); }

void EmSession::ScheduleResetBanks() {
    bankResetScheduled = true;

    EmASSERT(cpu);
    cpu->CheckAfterCycle();
}

void EmSession::ScheduleSubroutineReturn() {
    subroutineReturn = true;

    EmAssert(cpu);
    cpu->CheckAfterCycle();
}

EmDevice& EmSession::GetDevice() { return *device; }

uint32 EmSession::RunEmulation(uint32 maxCycles) {
    EmAssert(cpu);

    if (maxCycles == 0) return 0;

    // ExtraCycles are cycles that have been executed between two calls to RunEmulation.
    // They are already accounted for in systemCycles, but the main loop needs to
    // take them into accout as well, so correct systemCycles back to the last value
    // known to the main loop here.
    uint64 cyclesBefore = systemCycles - extraCycles;

    if (SuspendManager::IsSuspended()) {
        uint32 cycles = maxCycles + extraCycles;
        extraCycles = 0;
        return cycles;
    }

    uint32 cycles = cpu->Execute(maxCycles);
    systemCycles += cycles;

    CheckDayForRollover();

    extraCycles = 0;

    return systemCycles - cyclesBefore;
}

void EmSession::ExecuteSubroutine() {
    EmAssert(cpu);
    EmAssert(nestLevel >= 0);

    EmValueChanger<bool> clearSubroutineReturn(subroutineReturn, false);
    EmValueChanger<int> increaseNestLevel(nestLevel, nestLevel + 1);
    EmValueChanger<bool> resetDeadMansSwitch(deadMansSwitch, false);

    uint32 cycles = 0;

    while (!subroutineReturn) {
        cycles += cpu->Execute(EXECUTE_SUBROUTINE_LIMIT);

        if (deadMansSwitch) {
            cycles = 0;
            deadMansSwitch = false;
        }

        EmAssert(cycles < EXECUTE_SUBROUTINE_LIMIT);
    }
}

void EmSession::YieldMemoryMgr() {
    EmAssert(gCPU);

    CEnableFullAccess munge;

    UInt32 memSemaphoreIDP = EmLowMem_GetGlobal(memSemaphoreID);
    EmAliascj_xsmb<PAS> memSemaphoreID(memSemaphoreIDP);

    uint32 cycles = 0;

    while (memSemaphoreID.xsmuse != 0 && !EmPatchMgr::IsExecutingPatch() &&
           !SuspendManager::IsSuspended() && !gDebugger.IsStopped()) {
        EmAssert(cycles < YIELD_MEMMGR_LIMIT);

        cycles += gCPU68K->Execute(0);
    }

    if (!IsNested()) {
        extraCycles += cycles;
        systemCycles += cycles;
    }
}

void EmSession::HandleInstructionBreak() {
    EmPatchMgr::HandleInstructionBreak();
    CallbackManager::HandleBreakpoint();
}

void EmSession::QueuePenEvent(PenEvent evt) { EmPalmOS::QueuePenEvent(evt); }

void EmSession::QueueKeyboardEvent(KeyboardEvent evt) { EmPalmOS::QueueKeyboardEvent(evt); }

void EmSession::QueueButtonEvent(ButtonEvent evt) {
    if (evt.GetButton() == ButtonEvent::Button::cradle && !device->SupportsHardBtnCradle()) {
        if (evt.GetType() == ButtonEvent::Type::press) QueueKeyboardEvent(hardCradleChr);
        return;
    }

    if (buttonEventQueue.GetFree() == 0) buttonEventQueue.Get();
    buttonEventQueue.Put(evt);
}

bool EmSession::HasButtonEvent() {
    if (holdingBootKeys) return false;

    if ((systemCycles - lastButtonEventReadAt) < MIN_CYCLES_BETWEEN_BUTTON_EVENTS) return false;

    return buttonEventQueue.GetUsed() != 0;
}

ButtonEvent EmSession::NextButtonEvent() {
    lastButtonEventReadAt = systemCycles;

    return buttonEventQueue.GetUsed() > 0
               ? buttonEventQueue.Get()
               : ButtonEvent(ButtonEvent::Button::invalid, ButtonEvent::Type::press);
}

uint32 EmSession::GetMemorySize() const { return EmMemory::GetTotalMemorySize(); }

uint8* EmSession::GetMemoryPtr() const { return EmMemory::GetTotalMemory(); };

uint32 EmSession::GetRandomSeed() const { return EmLowMem::fgLowMem.globals.sysRandomSeed; }

uint8* EmSession::GetDirtyPagesPtr() const { return EmMemory::GetTotalDirtyPages(); }

void EmSession::SetHotsyncUserName(string hotsyncUserName) {
    gSystemState.SetHotsyncUserName(hotsyncUserName);

    if (IsCpuStopped()) {
        logPrintf("WARNING: attempt to set hotsync name with stopped CPU");

        return;
    }

    if (gSystemState.IsUIInitialized() && IsPowerOn()) {
        SetHotSyncUserName(hotsyncUserName.c_str());
    }
}

void EmSession::SetClockFactor(double clockFactor) {
    this->clockFactor = clockFactor;
    RecalculateClocksPerSecond();
}

void EmSession::RecalculateClocksPerSecond() {
    clocksPerSecond = EmHAL::GetSystemClockFrequency() * (clockFactor * DEFAULT_CLOCK_FACTOR);
}

void EmSession::CheckDayForRollover() {
    if (systemCycles - dateCheckedAt > clocksPerSecond) {
        uint32 currentDate = CurrentDate();

        if (currentDate != lastDate) {
            SetCurrentDate();
            EmHAL::onDayRollover.Dispatch();
        }

        lastDate = currentDate;
        dateCheckedAt = systemCycles;
    }
}

void EmSession::UpdateUARTModeSync() {
    EmHAL::SetUARTSync((this->transportIR && this->transportIR->RequiresSync()) ||
                       (this->transportSerial && this->transportSerial->RequiresSync()));
}

void EmSession::TriggerDeadMansSwitch() { deadMansSwitch = true; }

EmTransportSerial* EmSession::GetTransportSerial(EmUARTDeviceType type) {
    switch (type) {
        case kUARTIR:
            return transportIR ? transportIR.get() : &defaultTransportIR;

        case kUARTSerial:
            return transportSerial ? transportSerial.get() : &defaultTransportSerial;

        default:
            return nullptr;
    }
}

bool EmSession::LaunchAppByName(const string& name) { return EmPalmOS::LaunchAppByName(name); }

void EmSession::SetTransportSerial(EmUARTDeviceType type, EmTransportSerial* transport) {
    switch (type) {
        case kUARTIR:
            if (transportIR) {
                transportIR->onRequiresSyncChanged.RemoveHandler(
                    transportIrRequiresSyncChangedHandle);
            }

            transportIR.reset(transport);
            transportIrRequiresSyncChangedHandle = transportIR->onRequiresSyncChanged.AddHandler(
                bind(&EmSession::UpdateUARTModeSync, this));

            break;

        case kUARTSerial:
            if (transportSerial) {
                transportSerial->onRequiresSyncChanged.RemoveHandler(
                    transportSerialRequiresSyncChangedHandle);
            }

            transportSerial.reset(transport);
            transportSerialRequiresSyncChangedHandle =
                transportSerial->onRequiresSyncChanged.AddHandler(
                    bind(&EmSession::UpdateUARTModeSync, this));

            break;

        default:
            break;
    }
}
