#ifndef _SOC_GENERIC_H_
#define _SOC_GENERIC_H_

#include <cstdint>
#include <memory>

#include "SoC.h"
#include "scheduler.h"

template <class T>
class SocGeneric : public SoC {
   public:
    SocGeneric();

    uint64_t Run(uint64_t maxCycles, uint64_t cyclesPerSecond) override;
    bool RunToPaceSyscall(uint16_t syscall, uint64_t maxCycles, uint64_t cyclesPerSecond) override;
    bool ExecuteInjected(uint64_t maxCycles, uint64_t cyclesPerSecond) override;

    void Reset() override;
    void DumpMMU() override;
    void JamKey(enum KeyId key, uint32_t durationMsec) override;
    uint64_t GetTime() override;

    bool Save() override;
    bool Load(size_t savestateSize, void* savestateData) override;

    void Load(SavestateLoader<ChunkType>& loader);

    template <typename U>
    void Save(U& savestate);

   protected:
    void PumpEventQueues();

   protected:
    std::unique_ptr<Scheduler<T>> scheduler;

   private:
    static constexpr unsigned int SOC_GENERIC_SAVESTATE_VERSION = 2;
    static constexpr unsigned int MIN_EVENT_QUEUE_TICKS_BEFORE_PEN_UP = 2;

   private:
    template <unsigned int breakReason, bool injected>
    FORCE_INLINE uint64_t RunUntil(uint64_t maxCycles, uint64_t cyclesPerSecond);

    template <typename U>
    void DoSaveLoad(U& chunkHelper, uint32_t version);

    void PumpPenEventQueue();
    void PumpKeyEventQueue();

   private:
    uint16_t paceBreakSyscall{0};
};

#endif  // _SOC_GENERIC_H_
