#ifndef _MAIN_LOOP_H_
#define _MAIN_LOOP_H_

#include "Average.h"
#include "SoC.h"

class MainLoop {
   public:
    MainLoop(SoC* soc, uint64_t configuredCyclesPerSecond);

    void Cycle();

    uint64_t GetTimesliceSizeUsec() const;

   private:
    uint64_t CalculateCyclesPerSecond(uint64_t safetyMargin);

   private:
    SoC* soc{nullptr};
    uint64_t configuredCyclesPerSecond{0};

    uint64_t realTimeUsec{0};
    uint64_t lastSpeedDumpAtUsec{0};
    uint64_t lastCyclesPerSecond{0};
    double virtualTimeUsec{0};
    double lastDeltaUsec{0};

    Average<uint64_t> cyclesPerSecondAverage;
};

#endif  // _MAIN_LOOP_H_
