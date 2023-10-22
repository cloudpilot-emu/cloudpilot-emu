#ifndef _MAIN_LOOP_H_
#define _MAIN_LOOP_H_

#include "Average.h"
#include "SoC.h"

class MainLoop {
   public:
    MainLoop(SoC* soc, uint64_t configuredCyclesPerSecond);

    void Cycle();

   private:
    uint64_t CalculateCyclesPerSecond();

   private:
    SoC* soc;
    uint64_t configuredCyclesPerSecond;

    uint64_t realTimeUsec;
    uint64_t lastSpeedDumpAtUsec;
    uint64_t lastCyclesPerSecond;
    double virtualTimeUsec;

    Average<uint64_t> cyclesPerSecondAverage;
};

#endif  // _MAIN_LOOP_H_
