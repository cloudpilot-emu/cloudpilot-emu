#include "MainLoop.h"

#include <algorithm>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <iostream>

#include "util.h"

using namespace std;

namespace {
    constexpr int AVERAGE_TIMESLICES = 30;
    constexpr uint32_t BIN_SIZE = 1000000;
    constexpr uint32_t CYCLES_PER_SECOND_LIMIT_DEFAULT = 100000000;
    constexpr uint32_t MAX_LOAD_MAX = 97;
    constexpr uint32_t MAX_LOAD_CATCHUP_MARGIN = 20;
    constexpr uint32_t TIMESLICE_SIZE_USEC = 1000000 / MAIN_LOOP_FPS;
    constexpr uint32_t LAG_THRESHOLD_CATCHUP_USEC = (3 * TIMESLICE_SIZE_USEC / 2);
    constexpr uint32_t LAG_THRESHOLD_SKIP_USEC = 3 * TIMESLICE_SIZE_USEC;
    constexpr int PRESERVE_TIMESLICES_FOR_CATCHUP = 3;
}  // namespace

MainLoop::MainLoop(SoC* soc)
    : soc(soc),
      lastCyclesPerSecond(CYCLES_PER_SECOND_LIMIT_DEFAULT),
      cyclesPerSecondAverage(AVERAGE_TIMESLICES),
      maxLoad(MAX_LOAD_MAX),
      cyclesPerSecondLimit(CYCLES_PER_SECOND_LIMIT_DEFAULT) {
    uint64_t now = timestampUsec();

    realTimeUsec = now;
    virtualTimeUsec = now;
    cyclesPerSecondAverage.Add(cyclesPerSecondLimit);
}

void MainLoop::Cycle(uint64_t now) {
    double deltaUsec = now - virtualTimeUsec;

    if (deltaUsec > LAG_THRESHOLD_SKIP_USEC) {
        deltaUsec = TIMESLICE_SIZE_USEC;
        virtualTimeUsec = now - deltaUsec;
        cyclesPerSecondAverage.Reset(1);

        // cerr << "too much lag, skipping forward" << endl << flush;
    } else if (lastDeltaUsec < LAG_THRESHOLD_CATCHUP_USEC &&
               deltaUsec >= LAG_THRESHOLD_CATCHUP_USEC) {
        cyclesPerSecondAverage.Reset(PRESERVE_TIMESLICES_FOR_CATCHUP);
    }

    lastDeltaUsec = deltaUsec;
    realTimeUsec = now;

    double cyclesPerSecond = CalculateCyclesPerSecond(
        deltaUsec >= LAG_THRESHOLD_CATCHUP_USEC ? (maxLoad - MAX_LOAD_CATCHUP_MARGIN) : maxLoad);
    const uint64_t cyclesEmulated =
        socRun(soc, deltaUsec * cyclesPerSecond / 1E6, round(cyclesPerSecond));

    virtualTimeUsec += cyclesEmulated / cyclesPerSecond * 1E6;
    uint64_t now2 = timestampUsec();

    if (now2 != now) cyclesPerSecondAverage.Add((cyclesEmulated * 1000000) / (now2 - now));

    currentIps = cyclesPerSecond;
    currentIpsMax = cyclesPerSecondAverage.Calculate();
}

uint64_t MainLoop::GetTimesliceSizeUsec() const { return TIMESLICE_SIZE_USEC; }

uint32_t MainLoop::GetCurrentIps() const { return currentIps; }

uint32_t MainLoop::GetCurrentIpsMax() const { return currentIpsMax; }

void MainLoop::SetMaxLoad(uint32_t maxLoad) {
    if (maxLoad < MAX_LOAD_CATCHUP_MARGIN) maxLoad = MAX_LOAD_CATCHUP_MARGIN;
    if (maxLoad > MAX_LOAD_MAX) maxLoad = MAX_LOAD_MAX;

    this->maxLoad = maxLoad;
}

void MainLoop::SetCyclesPerSecondLimit(uint32_t cyclesPerSecondLimit) {
    if (cyclesPerSecondLimit < 10) cyclesPerSecondLimit = 10;

    this->cyclesPerSecondLimit = cyclesPerSecondLimit;
}

uint64_t MainLoop::CalculateCyclesPerSecond(uint64_t safetyMargin) {
    const uint64_t avg = (cyclesPerSecondAverage.Calculate() * safetyMargin) / 100;
    const uint64_t avgBinned = max((avg / BIN_SIZE) * BIN_SIZE, static_cast<uint64_t>(BIN_SIZE));

    if (avgBinned < lastCyclesPerSecond || avg > lastCyclesPerSecond + BIN_SIZE + BIN_SIZE / 2)
        lastCyclesPerSecond = avgBinned;

    if (lastCyclesPerSecond > cyclesPerSecondLimit) lastCyclesPerSecond = cyclesPerSecondLimit;

    return lastCyclesPerSecond;
}
