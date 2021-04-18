#include "EmSession.h"

#include <functional>

#include "CallbackManager.h"
#include "ChunkHelper.h"
#include "EmBankSRAM.h"
#include "EmCPU.h"
#include "EmHAL.h"
#include "EmLowMem.h"
#include "EmMemory.h"
#include "EmPalmOS.h"
#include "EmPatchMgr.h"
#include "EmSystemState.h"
#include "Logging.h"
#include "MetaMemory.h"
#include "Miscellaneous.h"
#include "ROMStubs.h"
#include "Savestate.h"
#include "SavestateLoader.h"
#include "SavestateProbe.h"
#include "SessionImage.h"

namespace {
    constexpr uint32 SAVESTATE_VERSION = 2;

    constexpr int MIN_CYCLES_BETWEEN_EVENTS = 10000;
    constexpr int MIN_CYCLES_BETWEEN_BUTTON_EVENTS = 400000;

    constexpr uint32 RUN_TO_SYSCALL_LIMIT = 10000000;
    constexpr uint32 YIELD_MEMMGR_LIMIT = 10000000;
    constexpr uint32 EXECUTE_SUBROUTINE_LIMIT = 50000000;

    constexpr double DEFAULT_CLOCK_FACTOR = 0.5;

    EmSession _gSession;

    uint32 CurrentDay() {
        uint32 year, month, day;

        Platform::GetDate(year, month, day);

        return day;
    }
}  // namespace

EmSession* gSession = &_gSession;

bool EmSession::Initialize(EmDevice* device, const uint8* romImage, size_t romLength) {
    if (isInitialized) {
        Deinitialize();
    }

    this->romImage = make_unique<uint8[]>(romLength);
    romSize = romLength;
    memcpy(this->romImage.get(), romImage, romSize);

    onSystemClockChangeHandle =
        EmHAL::onSystemClockChange.AddHandler(bind(&EmSession::RecalculateClocksPerSecond, this));

    this->device.reset(device);

    cpu.reset(device->CreateCPU(this));

    if (!Memory::Initialize(romImage, romLength, device->MinRAMSize())) return false;
    EmPalmOS::Initialize();

    systemCycles = 0;
    extraCycles = 0;
    Reset(ResetType::soft);

    gSystemState.Reset();

    RecalculateClocksPerSecond();

    dayCheckedAt = 0;
    dayAtLastClockSync = CurrentDay();

    isInitialized = true;
    return true;
}

void EmSession::Deinitialize() {
    if (!isInitialized) return;

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
    waitingForSyscall = false;
    syscallDispatched = false;

    nestLevel = 0;
    subroutineReturn = false;

    systemCycles = 0;
    extraCycles = 0;
    holdingBootKeys = false;

    dayCheckedAt = 0;
    dayAtLastClockSync = 0;

    romSize = 0;
    romImage.reset();
    savestate.Reset();

    isInitialized = false;
}

pair<size_t, unique_ptr<uint8[]>> EmSession::SaveImage() {
    EmAssert(romImage);

    SessionImageSerializer image;

    image.SetRomImage(romSize, romImage.get()).SetRamImage(GetMemorySize(), GetMemoryPtr());

    if (savestate.Save(*this)) {
        image.SetSavestate(savestate.GetSize(), savestate.GetBuffer());
    } else {
        logging::printf("failed to save savestate");
    }

    return image.Serialize(device->GetIDString());
}

bool EmSession::LoadImage(SessionImage& image) {
    if (!image.IsValid()) return false;

    EmDevice* device = new EmDevice(image.GetDeviceId());
    if (device->GetIDString() != device->GetIDString()) {
        logging::printf("device id mismatch");
        return false;
    }

    auto [romSize, romImage] = image.GetRomImage();
    if (!Initialize(device, static_cast<uint8*>(romImage), romSize)) {
        logging::printf("failed to initialize session");
        return false;
    }

    auto [ramSize, ramImage] = image.GetRamImage();
    if (ramSize != GetMemorySize()) {
        logging::printf("memory size mismatch, not restoring RAM");

        return true;
    } else
        memcpy(this->GetMemoryPtr(), ramImage, ramSize);

    auto [savestateSize, savestateImage] = image.GetSavestate();
    if (savestateSize == 0) return true;

    if (!Load(savestateSize, static_cast<uint8*>(savestateImage)))
        logging::printf("failed to restore savestate");

    return true;
}

template <typename T>
void EmSession::Save(T& savestate) {
    EmAssert(nestLevel == 0);
    EmAssert(device);
    EmAssert(cpu);
    EmAssert(!subroutineReturn);
    EmAssert(!waitingForSyscall);
    EmAssert(!syscallDispatched);

    typename T::chunkT* chunk = savestate.GetChunk(ChunkType::session);
    if (!chunk) return;

    chunk->Put32(SAVESTATE_VERSION);
    chunk->PutString(device->GetIDString(), 16);

    SaveChunkHelper helper(*chunk);
    DoSaveLoad(helper);

    cpu->Save(savestate);
    EmPatchMgr::Save(savestate);
    gSystemState.Save(savestate);
    Memory::Save(savestate);
}

template void EmSession::Save(Savestate& savestate);
template void EmSession::Save(SavestateProbe& savestate);

void EmSession::Load(SavestateLoader& loader) {
    EmAssert(device);
    EmAssert(cpu);

    Chunk* chunk = loader.GetChunk(ChunkType::session);
    if (!chunk) return;

    if (chunk->Get32() > SAVESTATE_VERSION) {
        logging::printf("unable to restore session: savestate version mismatch");
        loader.NotifyError();

        return;
    }

    if (chunk->GetString(16) != device->GetIDString()) {
        logging::printf("unable to restore session: device ID does not match savestate");
    }

    nestLevel = 0;
    extraCycles = 0;
    subroutineReturn = false;
    waitingForSyscall = false;
    syscallDispatched = false;

    penEventQueue.Clear();
    keyboardEventQueue.Clear();
    penEventQueueIncoming.Clear();
    keyboardEventQueueIncoming.Clear();
    buttonEventQueue.Clear();

    LoadChunkHelper helper(*chunk);
    DoSaveLoad(helper);

    lastEventPromotedAt = systemCycles;
    lastButtonEventReadAt = systemCycles;

    cpu->Load(loader);
    EmPatchMgr::Load(loader);
    gSystemState.Load(loader);
    Memory::Load(loader);

    if (gSystemState.IsUIInitialized()) {
        SetCurrentDate();
    }

    dayCheckedAt = systemCycles;
    dayAtLastClockSync = CurrentDay();

    RecalculateClocksPerSecond();
}

bool EmSession::Save() { return savestate.Save(*this); }

bool EmSession::Load(size_t size, uint8* buffer) {
    SavestateLoader loader;

    if (!loader.Load(buffer, size, *this)) {
        Reset(ResetType::soft);

        return false;
    }

    return true;
}

Savestate& EmSession::GetSavestate() { return savestate; }

pair<size_t, uint8*> EmSession::GetRomImage() {
    EmAssert(romImage);

    return pair(romSize, romImage.get());
}

template <typename T>
void EmSession::DoSaveLoad(T& helper) {
    helper.Do(typename T::BoolPack() << bankResetScheduled << resetScheduled << holdingBootKeys)
        .Do(typename T::Pack8() << *reinterpret_cast<uint8*>(&resetType)
                                << *reinterpret_cast<uint8*>(&bootKeysType))
        .Do64(systemCycles);
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

    if (resetType != ResetType::sys) systemCycles = 0;

    Memory::Reset(resetType != ResetType::sys);
    cpu->Reset(resetType != ResetType::sys);
    EmPalmOS::Reset();
    gSystemState.Reset();

    bankResetScheduled = false;
    resetScheduled = false;

    waitingForSyscall = false;
    syscallDispatched = false;

    penEventQueue.Clear();
    keyboardEventQueue.Clear();
    penEventQueueIncoming.Clear();
    keyboardEventQueueIncoming.Clear();
    lastEventPromotedAt = 0;

    if (resetType != ResetType::sys) {
        buttonEventQueue.Clear();
        lastButtonEventReadAt = 0;

        switch (resetType) {
            case ResetType::hard:
                holdingBootKeys = true;
                bootKeysType = ResetType::hard;

                EmHAL::ButtonEvent(
                    ButtonEvent(ButtonEvent::Button::power, ButtonEvent::Type::press));

                break;

            case ResetType::noext:
                holdingBootKeys = true;
                bootKeysType = ResetType::noext;

                EmHAL::ButtonEvent(
                    ButtonEvent(ButtonEvent::Button::rockerUp, ButtonEvent::Type::press));

                break;

            default:
                break;
        }
    }
}

bool EmSession::IsNested() const {
    EmAssert(nestLevel >= 0);
    return nestLevel > 0;
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
            EmHAL::ButtonEvent(ButtonEvent(ButtonEvent::Button::power, ButtonEvent::Type::release));

            break;

        case ResetType::noext:
            EmHAL::ButtonEvent(
                ButtonEvent(ButtonEvent::Button::rockerUp, ButtonEvent::Type::release));

            break;

        default:
            break;
    }

    holdingBootKeys = false;
}

bool EmSession::ExecuteSpecial(bool checkForResetOnly) {
    if (resetScheduled) {
        resetScheduled = false;
        bankResetScheduled = false;

        this->Reset(resetType);
    }

    if (bankResetScheduled) {
        bankResetScheduled = false;

        Memory::ResetBankHandlers();
    }

    if (checkForResetOnly) return false;

    return false;
}

bool EmSession::CheckForBreak() const {
    return (IsNested() && subroutineReturn) || (waitingForSyscall && syscallDispatched);
}

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

    PumpEvents();

    uint32 cycles = cpu->Execute(maxCycles);
    systemCycles += cycles;

    if (cpu->Stopped() && IsPowerOn())
        logging::printf("WARNING: CPU in stopped state after RunEmulation");

    CheckDayForRollover();

    extraCycles = 0;

    return systemCycles - cyclesBefore;
}

void EmSession::ExecuteSubroutine() {
    EmAssert(cpu);
    EmAssert(nestLevel >= 0);

    EmValueChanger<bool> clearSubroutineReturn(subroutineReturn, false);
    EmValueChanger<int> increaseNestLevel(nestLevel, nestLevel + 1);

    uint32 cycles = 0;

    while (!subroutineReturn) {
        cycles += cpu->Execute(EXECUTE_SUBROUTINE_LIMIT);

        EmAssert(cycles < EXECUTE_SUBROUTINE_LIMIT);
    }
}

bool EmSession::RunToSyscall() {
    EmValueChanger<bool> startRunToSyscall(waitingForSyscall, true);
    EmValueChanger<bool> resetSyscallDispatched(syscallDispatched, false);

    EmAssert(gCPU);

    uint32 cycles = 0;

    while (!syscallDispatched && cycles < RUN_TO_SYSCALL_LIMIT) {
        cycles += cpu->Execute(RUN_TO_SYSCALL_LIMIT);

        if (cpu->Stopped()) break;
    }

    systemCycles += cycles;

    return syscallDispatched;
}

void EmSession::NotifySyscallDispatched() {
    syscallDispatched = true;

    EmAssert(gCPU);
    cpu->CheckAfterCycle();
}

bool EmSession::WaitingForSyscall() const { return waitingForSyscall; }

void EmSession::YieldMemoryMgr() {
    EmAssert(gCPU);

    CEnableFullAccess munge;

    UInt32 memSemaphoreIDP = EmLowMem_GetGlobal(memSemaphoreID);
    EmAliascj_xsmb<PAS> memSemaphoreID(memSemaphoreIDP);

    uint32 cycles = 0;

    while (memSemaphoreID.xsmuse != 0 && !EmPatchMgr::IsExecutingPatch()) {
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

void EmSession::QueuePenEvent(PenEvent evt) {
    if (!IsPowerOn()) return;

    if (penEventQueueIncoming.GetFree() == 0) penEventQueueIncoming.Get();

    penEventQueueIncoming.Put(evt);
}

bool EmSession::HasPenEvent() { return penEventQueue.GetUsed() > 0; }

PenEvent EmSession::NextPenEvent() { return HasPenEvent() ? penEventQueue.Get() : PenEvent(); }

PenEvent EmSession::PeekPenEvent() { return HasPenEvent() ? penEventQueue.Peek() : PenEvent(); }

void EmSession::QueueKeyboardEvent(KeyboardEvent evt) {
    if (!IsPowerOn()) return;

    if (keyboardEventQueueIncoming.GetFree() == 0) keyboardEventQueueIncoming.Get();

    keyboardEventQueueIncoming.Put(evt);
}

bool EmSession::HasKeyboardEvent() { return keyboardEventQueue.GetUsed() > 0; }

KeyboardEvent EmSession::NextKeyboardEvent() {
    return HasKeyboardEvent() ? keyboardEventQueue.Get() : KeyboardEvent('.');
}

bool EmSession::Wakeup() {
    bool isSafeToWakeup = RunToSyscall();

    if (isSafeToWakeup) {
        if (gSystemState.OSMajorVersion() >= 4) {
            EvtWakeupWithoutNilEvent();
        } else {
            EvtWakeup();
            EmLowMem::ClearNilEvent();
        }
    }

    return isSafeToWakeup;
}

void EmSession::PumpEvents() {
    if (systemCycles - lastEventPromotedAt < MIN_CYCLES_BETWEEN_EVENTS ||
        !gSystemState.IsUIInitialized())
        return;

    if (PromoteKeyboardEvent() || PromotePenEvent()) lastEventPromotedAt = systemCycles;
}

bool EmSession::PromoteKeyboardEvent() {
    if (keyboardEventQueueIncoming.GetUsed() == 0) return false;

    if (!Wakeup()) return false;

    if (keyboardEventQueue.GetFree() == 0) keyboardEventQueue.Get();
    keyboardEventQueue.Put(keyboardEventQueueIncoming.Get());

    return true;
}

bool EmSession::PromotePenEvent() {
    if (penEventQueueIncoming.GetUsed() == 0) return false;

    if (!Wakeup()) return false;

    if (penEventQueue.GetFree() == 0) penEventQueue.Get();
    penEventQueue.Put(penEventQueueIncoming.Get());

    return true;
}

void EmSession::QueueButtonEvent(ButtonEvent evt) {
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

uint32 EmSession::GetMemorySize() const { return gRAMBank_Size; }

uint8* EmSession::GetMemoryPtr() const { return gRAM_Memory; };

uint32 EmSession::GetRandomSeed() const { return EmLowMem::fgLowMem.globals.sysRandomSeed; }

uint8* EmSession::GetDirtyPagesPtr() const { return gRAM_DirtyPages; }

void EmSession::SetHotsyncUserName(string hotsyncUserName) {
    gSystemState.SetHotsyncUserName(hotsyncUserName);

    if (IsCpuStopped()) {
        logging::printf("WARNING: attempt to set hotsync name with stopped CPU");

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
    if (gSystemState.IsUIInitialized() && systemCycles - dayCheckedAt > clocksPerSecond) {
        uint32 currentDay = CurrentDay();

        if (currentDay != dayAtLastClockSync) SetCurrentDate();

        dayAtLastClockSync = currentDay;
        dayCheckedAt = systemCycles;
    }
}
