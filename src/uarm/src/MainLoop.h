#ifndef _MAIN_LOOP_H_
#define _MAIN_LOOP_H_

#include "Average.h"
#include "SoC.h"

#define MAIN_LOOP_FPS 60

class MainLoop {
   public:
    MainLoop(SoC* soc, uint64_t configuredCyclesPerSecond);

    void Cycle(uint64_t now);

    uint64_t GetTimesliceSizeUsec() const;

    uint32_t GetCurrentIps() const;
    uint32_t GetCurrentIpsMax() const;

   private:
    uint64_t CalculateCyclesPerSecond(uint64_t safetyMargin);

   private:
    SoC* soc{nullptr};
    uint64_t configuredCyclesPerSecond{0};

    uint64_t realTimeUsec{0};
    uint64_t lastCyclesPerSecond{0};

    double virtualTimeUsec{0};
    double lastDeltaUsec{0};

    uint32_t currentIps{0};
    uint32_t currentIpsMax{0};

    Average<uint64_t> cyclesPerSecondAverage;
};

#endif  // _MAIN_LOOP_H_
