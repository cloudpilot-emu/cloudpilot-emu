#include "MainLoop.h"

#include <algorithm>
#include <cmath>
#include <ctime>
#include <iostream>

using namespace std;

namespace {
    constexpr double MAX_LAG_USEC = 1 / 30. * 1E6;
    constexpr int AVERAGE_TIMESLICES = 60;
    constexpr uint64_t SPEED_DUMP_INTERVAL_USEC = 1000000;
    constexpr uint64_t BIN_SIZE = 1000000;
    constexpr uint64_t SAFETY_MARGIN_PCT = 95;

    uint64_t timestampUsec() {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);

        return ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
    }
}  // namespace

MainLoop::MainLoop(SoC* soc, uint64_t configuredCyclesPerSecond)
    : soc(soc),
      configuredCyclesPerSecond(configuredCyclesPerSecond),
      lastCyclesPerSecond(configuredCyclesPerSecond),
      cyclesPerSecondAverage(AVERAGE_TIMESLICES) {
    uint64_t now = timestampUsec();

    realTimeUsec = lastSpeedDumpAtUsec = now;
    virtualTimeUsec = static_cast<double>(now);
    cyclesPerSecondAverage.Add(configuredCyclesPerSecond);
}

void MainLoop::Cycle() {
    const uint64_t now = timestampUsec();

    double delta = static_cast<double>(now) - virtualTimeUsec;
    if (delta > MAX_LAG_USEC && cyclesPerSecondAverage.GetCount() > 0) {
        delta = MAX_LAG_USEC;
        virtualTimeUsec = static_cast<double>(now) - MAX_LAG_USEC;
        cyclesPerSecondAverage.Reset(1);

        cerr << "time skip" << endl << flush;
    }

    realTimeUsec = now;

    double cyclesPerSecond = static_cast<double>(CalculateCyclesPerSecond());

    const uint64_t cyclesEmulated = socRun(soc, round(delta * cyclesPerSecond / 1E6), 2);

    virtualTimeUsec += static_cast<double>(cyclesEmulated) / cyclesPerSecond * 1E6;
    cyclesPerSecondAverage.Add((cyclesEmulated * 1000000) / (timestampUsec() - now));

    if (now - lastSpeedDumpAtUsec > SPEED_DUMP_INTERVAL_USEC) {
        cerr << "current emulation speed " << static_cast<uint64_t>(cyclesPerSecond) << " IPS , "
             << cyclesPerSecondAverage.Calculate() << " IPS theoretical max" << endl
             << flush;
        lastSpeedDumpAtUsec = now;
    }
}

uint64_t MainLoop::CalculateCyclesPerSecond() {
    const uint64_t avg = (cyclesPerSecondAverage.Calculate() * SAFETY_MARGIN_PCT) / 100;
    const uint64_t avgBinned = max((avg / BIN_SIZE) * BIN_SIZE, BIN_SIZE);

    if (avgBinned < lastCyclesPerSecond || avg > lastCyclesPerSecond + BIN_SIZE + BIN_SIZE / 2)
        lastCyclesPerSecond = avgBinned;

    if (lastCyclesPerSecond > configuredCyclesPerSecond)
        lastCyclesPerSecond = configuredCyclesPerSecond;

    return lastCyclesPerSecond;
}
