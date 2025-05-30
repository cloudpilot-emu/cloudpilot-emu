#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include <cstdint>

#include "Logging.h"
#include "cputil.h"
#include "savestate/savestateAll.h"

#define SCHEDULER_TASK_TIMER 0
#define SCHEDULER_TASK_RTC 1
#define SCHEDULER_TASK_LCD 2
#define SCHEDULER_TASK_I2S 3
#define SCHEDULER_TASK_PCM 4
#define SCHEDULER_TASK_AUX_1 5
#define SCHEDULER_TASK_AUX_2 6
#define SCHEDULER_TASK_AUX_3 7

constexpr uint64_t operator""_sec(unsigned long long seconds) { return seconds * 1000000000ull; }
constexpr uint64_t operator""_msec(unsigned long long mseconds) { return mseconds * 1000000ull; }
constexpr uint64_t operator""_usec(unsigned long long useconds) { return useconds * 1000ull; }
constexpr uint64_t operator""_nsec(unsigned long long nseconds) { return nseconds; }

template <typename T>
class Scheduler {
   public:
    explicit Scheduler(T& dispatchDelegate);

    void ScheduleTask(uint32_t taskType, uint64_t period, uint32_t batchTicks);
    void RescheduleTask(uint32_t taskType, uint32_t batchTicks);
    void RescheduleTaskAtLeast(uint32_t taskType, uint32_t batchTicks);
    inline void UnscheduleTask(uint32_t taskType);

    uint64_t CyclesToNextUpdate(uint64_t cyclesPerSecond);

    void Advance(uint64_t cycles, uint64_t cyclesPerSecond);

    uint64_t GetTime() const;

    template <typename U>
    void Save(U& savestate);

    void Load(SavestateLoader<ChunkType>& loader);

   private:
    template <bool atLeast>
    inline void RescheduleTaskImpl(uint32_t taskType, uint32_t batchTicks);

    template <typename U>
    void DoSaveLoad(U& ChunkHelper);

   private:
    static constexpr int32_t SCHEDULER_TASK_MAX = SCHEDULER_TASK_AUX_3;
    static constexpr int32_t SCHEDULER_TASK_NONE = 0xff;
    static constexpr uint32_t SAVESTATE_VERSION = 0;

    struct Task {
        uint32_t batchedTicks{0};

        uint64_t period{0};
        uint64_t lastUpdate{0};
        uint64_t nextUpdate{0};
    };

   private:
    void UpdateNextUpdate();

   private:
    T& dispatchDelegate;

    Task tasks[SCHEDULER_TASK_MAX + 1];
    uint32_t queueBuffer[SCHEDULER_TASK_MAX + 2];
    uint32_t* queue{&queueBuffer[1]};

    uint64_t accTime{0};
    uint64_t nextUpdate{1_sec};
};

///////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
///////////////////////////////////////////////////////////////////////////////

template <typename T>
Scheduler<T>::Scheduler(T& dispatchDelegate) : dispatchDelegate(dispatchDelegate) {
    for (int i = -1; i <= SCHEDULER_TASK_MAX; i++) queue[i] = SCHEDULER_TASK_NONE;
}

template <typename T>
void Scheduler<T>::ScheduleTask(uint32_t taskType, uint64_t period, uint32_t batchTicks) {
    Task& task{tasks[taskType]};

    task.period = period;
    task.lastUpdate = accTime;
    task.batchedTicks = 0;

    RescheduleTask(taskType, batchTicks);
}

template <typename T>
void Scheduler<T>::RescheduleTask(uint32_t taskType, uint32_t batchTicks) {
    RescheduleTaskImpl<false>(taskType, batchTicks);
}

template <typename T>
void Scheduler<T>::RescheduleTaskAtLeast(uint32_t taskType, uint32_t batchTicks) {
    RescheduleTaskImpl<true>(taskType, batchTicks);
}

template <typename T>
template <bool atLeast>
void Scheduler<T>::RescheduleTaskImpl(uint32_t taskType, uint32_t batchTicks) {
    if (batchTicks == 0) return UnscheduleTask(taskType);

    Task& task{tasks[taskType]};

    if constexpr (atLeast) {
        const uint64_t accumulatedTicks = (accTime - task.lastUpdate) / task.period;
        if (accumulatedTicks > batchTicks) batchTicks = accumulatedTicks;
    }

    if (task.batchedTicks == 0) {
        task.lastUpdate =
            ((accTime - task.nextUpdate) / task.period) * task.period + task.nextUpdate;
    }

    task.batchedTicks = batchTicks;
    uint64_t nextUpdate = task.nextUpdate = task.lastUpdate + batchTicks * task.period;

    int32_t idx = -1;
    bool inserted = false;
    bool deleted = false;
    const uint32_t nextTaskTypeBeforeReschedule = queue[taskType];

    do {
        uint32_t nextTaskType = queue[idx];

        if (nextTaskType == taskType) {
            queue[idx] = nextTaskTypeBeforeReschedule;
            nextTaskType = queue[idx];

            deleted = true;
            if (inserted) break;
        }

        if (!inserted &&
            (nextTaskType > SCHEDULER_TASK_MAX || tasks[nextTaskType].nextUpdate > nextUpdate)) {
            queue[idx] = taskType;
            queue[taskType] = nextTaskType;

            inserted = true;
            if (deleted) break;
        }

        idx = nextTaskType;
    } while (idx <= SCHEDULER_TASK_MAX);

    UpdateNextUpdate();
}

template <typename T>
void Scheduler<T>::UnscheduleTask(uint32_t taskType) {
    tasks[taskType].batchedTicks = 0;

    for (int32_t idx = -1; idx <= SCHEDULER_TASK_MAX; idx = queue[idx]) {
        const uint32_t nextTaskType = queue[idx];

        if (nextTaskType == taskType) {
            queue[idx] = queue[nextTaskType];
            break;
        }
    }

    UpdateNextUpdate();
}

template <typename T>
uint64_t Scheduler<T>::CyclesToNextUpdate(uint64_t cyclesPerSecond) {
    return ((nextUpdate - accTime) * cyclesPerSecond) / 1_sec + 1;
}

template <typename T>
void Scheduler<T>::Advance(uint64_t cycles, uint64_t cyclesPerSecond) {
    accTime += ((cycles * 1_sec) / cyclesPerSecond);
    if (accTime < nextUpdate) return;

    while (true) {
        const uint32_t taskType = queue[-1];
        if (taskType > SCHEDULER_TASK_MAX) break;

        Task& task{tasks[taskType]};

        if (task.nextUpdate > accTime) break;

        const uint32_t batchTicks = dispatchDelegate.DispatchTicks(taskType, task.batchedTicks);
        task.lastUpdate += task.batchedTicks * task.period;

        RescheduleTaskImpl<false>(taskType, batchTicks);
    }

    UpdateNextUpdate();
}

template <typename T>
uint64_t Scheduler<T>::GetTime() const {
    return accTime;
}

template <typename T>
void Scheduler<T>::UpdateNextUpdate() {
    if (queue[-1] <= SCHEDULER_TASK_MAX) {
        nextUpdate = tasks[queue[-1]].nextUpdate;
    } else {
        nextUpdate = accTime + 1_sec;
    }
}

template <typename T>
template <typename U>
void Scheduler<T>::Save(U& savestate) {
    auto chunk = savestate.GetChunk(ChunkType::scheduler, SAVESTATE_VERSION);
    if (!chunk) ERR("unable to allocate chunk");

    SaveChunkHelper helper(*chunk);
    DoSaveLoad(helper);
}

template <typename T>
void Scheduler<T>::Load(SavestateLoader<ChunkType>& loader) {
    Chunk* chunk = loader.GetChunk(ChunkType::scheduler, SAVESTATE_VERSION, "scheduler");
    if (!chunk) return;

    LoadChunkHelper helper(*chunk);
    DoSaveLoad(helper);

    UpdateNextUpdate();
}

template <typename T>
template <typename U>
void Scheduler<T>::DoSaveLoad(U& chunkHelper) {
    for (size_t i = 0; i <= SCHEDULER_TASK_MAX; i++) {
        Task& task(tasks[i]);

        chunkHelper.Do32(task.batchedTicks)
            .Do64(task.period)
            .Do64(task.lastUpdate)
            .Do64(task.nextUpdate)
            .Do32(queue[i]);
    }

    chunkHelper.Do32(queue[-1]).Do64(accTime);
}

#endif  // _SCHEDULER_H_