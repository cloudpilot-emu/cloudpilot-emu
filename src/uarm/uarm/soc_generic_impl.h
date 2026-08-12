#ifndef _SOC_GENERIC_IMPL_H_
#define _SOC_GENERIC_IMPL_H_

// clang-format off
#include "soc_generic.h"
// clang-format on

#include "CPU.h"
#include "SoC.h"
#include "cputil.h"
#include "savestate/savestateAll.h"
#include "system_state.h"
#include "vSD.h"

template <class T>
SocGeneric<T>::SocGeneric() : scheduler(std::make_unique<Scheduler<T>>(static_cast<T&>(*this))) {}

template <class T>
uint64_t SocGeneric<T>::Run(uint64_t maxCycles, uint64_t cyclesPerSecond) {
    paceBreakSyscall = 0;

    return RunUntil<0, false>(maxCycles, cyclesPerSecond);
}

template <class T>
bool SocGeneric<T>::RunToPaceSyscall(uint16_t syscall, uint64_t maxCycles,
                                     uint64_t cyclesPerSecond) {
    if (syscall == paceBreakSyscall) return true;

    auto cpu = this->cpu;

    cpuSetBreakPaceSyscall(cpu, syscall);
    const uint64_t cycles =
        RunUntil<SLOW_PATH_REASON_PACE_SYSCALL_BREAK, false>(maxCycles, cyclesPerSecond);
    cpuSetBreakPaceSyscall(cpu, 0);

    injectedTimeNsec += (cycles * 1000000) / (cyclesPerSecond / 1000);

    if ((cpuGetSlowPathReason(cpu) & SLOW_PATH_REASON_PACE_SYSCALL_BREAK) == 0) return false;

    paceBreakSyscall = syscall;

    return true;
}

template <class T>
bool SocGeneric<T>::ExecuteInjected(uint64_t maxCycles, uint64_t cyclesPerSecond) {
    auto cpu = this->cpu;

    const uint64_t cycles =
        RunUntil<SLOW_PATH_REASON_INJECTED_CALL_DONE, true>(maxCycles, cyclesPerSecond);

    injectedTimeNsec += (cycles * 1000000) / (cyclesPerSecond / 1000);

    return cpuGetSlowPathReason(cpu) & SLOW_PATH_REASON_INJECTED_CALL_DONE;
}

template <class T>
bool SocGeneric<T>::Save() {
    return savestate->Save(*this);
}

template <class T>
bool SocGeneric<T>::Load(size_t savestateSize, void* savestateData) {
    if (savestateSize == 0 || !savestateData) return true;

    Savestate<ChunkType> savestate;
    SavestateLoader<ChunkType> loader;

    savestate.Save(*this);

    if (!loader.Load(savestateData, savestateSize, *this)) {
        if (!loader.Load(savestate.GetBuffer(), savestate.GetSize(), *this))
            ERR("failed to restore initial state after load failure");

        return false;
    }

    return true;
}

template <class T>
void SocGeneric<T>::Load(SavestateLoader<ChunkType>& loader) {
    cpuLoad(cpu, loader);
    scheduler->Load(loader);
    vsdLoad(vSD, loader);
    systemStateLoad(systemState, loader);

    uint32_t version;
    Chunk* chunk = loader.GetChunkOrFail(ChunkType::socGeneric, SOC_GENERIC_SAVESTATE_VERSION,
                                         "soc_generic", version);
    if (!chunk) return;

    LoadChunkHelper helper(*chunk);
    DoSaveLoad(helper, version);

    static_cast<T*>(this)->OnLoad(loader);
}

template <class T>
template <typename U>
void SocGeneric<T>::Save(U& savestate) {
    cpuSave(cpu, savestate);
    scheduler->Save(savestate);
    vsdSave(vSD, savestate);
    systemStateSave(systemState, savestate);

    auto* chunk = savestate.GetChunk(ChunkType::socGeneric, SOC_GENERIC_SAVESTATE_VERSION);
    if (!chunk) ERR("unable to allocate chunk");

    strncpy(cardId, sdCardGetId(), sizeof(cardId));
    cardId[sizeof(cardId) - 1] = '\0';

    SaveChunkHelper helper(*chunk);
    DoSaveLoad(helper, SOC_GENERIC_SAVESTATE_VERSION);

    static_cast<T*>(this)->OnSave(savestate);
}

template <class T>
template <typename U>
void SocGeneric<T>::DoSaveLoad(U& chunkHelper, uint32_t version) {
    chunkHelper.Do(typename U::BoolPack() << cardInserted << pcmEnabled << sleeping)
        .DoBuffer(cardId, sizeof(cardId));

    uint64_t cyclesTotal = 0;
    if (version == 1) chunkHelper.Do64(cyclesTotal);
}

template <class T>
template <unsigned int breakReason, bool injected>
uint64_t SocGeneric<T>::RunUntil(uint64_t maxCycles, uint64_t cyclesPerSecond) {
    auto cpu = this->cpu;
    Scheduler<T>* scheduler = this->scheduler.get();

    uint64_t cycles = 0;

    while (cycles < maxCycles) {
        uint64_t cyclesToAdvance = scheduler->CyclesToNextUpdate(cyclesPerSecond);

        if (cyclesToAdvance + cycles > maxCycles) cyclesToAdvance = maxCycles - cycles;

        uint64_t cyclesAdvanced;

        if constexpr (injected) {
            cyclesAdvanced = cpuCycle<T::MEMORY_SYSTEM_KIND, injected>(cpu, cyclesToAdvance);
        } else {
            cyclesAdvanced = sleeping
                                 ? cyclesToAdvance
                                 : cpuCycle<T::MEMORY_SYSTEM_KIND, injected>(cpu, cyclesToAdvance);
        }

        scheduler->Advance(cyclesAdvanced, cyclesPerSecond);
        cycles += cyclesAdvanced;

        if constexpr (breakReason > 0) {
            if (cpuGetSlowPathReason(cpu) & breakReason) break;
        }
    }

    return cycles;
}

template <class T>
uint64_t SocGeneric<T>::GetTime() {
    return scheduler->GetTime();
}

#endif  // _SOC_GENERIC_IMPL_H_
