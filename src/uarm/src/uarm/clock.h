#ifndef _CLOCK_H_
#define _CLOCK_H_

#include <cstdint>

#define CLOCK_CLIENT_TIMER 0
#define CLOCK_CLIENT_RTC 1
#define CLOCK_CLIENT_LCD 2
#define CLOCK_CLIENT_AUX_1 3
#define CLOCK_CLIENT_AUX_2 4
#define CLOCK_CLIENT_AUX_3 5
#define CLOCK_CLIENT_AUX_4 6
#define CLOCK_CLIENT_AUX_5 7

constexpr uint64_t operator""_sec(uint64_t seconds) { return seconds * 1000000000ull; }
constexpr uint64_t operator""_msec(uint64_t mseconds) { return mseconds * 1000000ull; }
constexpr uint64_t operator""_usec(uint64_t useconds) { return useconds * 1000ull; }
constexpr uint64_t operator""_nsec(uint64_t nseconds) { return nseconds; }

template <typename T>
class Clock {
   public:
    explicit Clock(T& dispatchDelegate);

    void ScheduleClient(uint32_t clientType, uint64_t period, uint32_t batchTicks);
    inline void RescheduleClient(uint32_t clientType, uint32_t batchTicks);
    inline void UnscheduleClient(uint32_t clientType);

    uint64_t CyclesToNextUpdate(uint64_t cyclesPerSecond);

    void Advance(uint64_t cycles, uint64_t cyclesPerSecond);

   private:
    static constexpr int32_t CLOCK_CLIENT_MAX = CLOCK_CLIENT_AUX_5;
    static constexpr int32_t CLOCK_CLIENT_NONE = 0xff;

    struct Client {
        uint32_t batchedTicks{0};

        uint64_t period{0};
        uint64_t lastUpdate{0};
        uint64_t nextUpdate{0};
    };

   private:
    void UpdateNextUpdate();

   private:
    T& dispatchDelegate;

    Client clients[CLOCK_CLIENT_MAX + 1];
    uint32_t queueBuffer[CLOCK_CLIENT_MAX + 2];
    uint32_t* queue{&queueBuffer[1]};

    uint64_t accTime{0};
    uint64_t nextUpdate{1_sec};
};

///////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
///////////////////////////////////////////////////////////////////////////////

template <typename T>
Clock<T>::Clock(T& dispatchDelegate) : dispatchDelegate(dispatchDelegate) {
    for (int i = -1; i <= CLOCK_CLIENT_MAX; i++) queue[i] = CLOCK_CLIENT_NONE;
}

template <typename T>
void Clock<T>::ScheduleClient(uint32_t clientType, uint64_t period, uint32_t batchTicks) {
    Client& client{clients[clientType]};

    client.period = period;
    client.lastUpdate = accTime;
    client.batchedTicks = 0;

    RescheduleClient(clientType, batchTicks);
}

template <typename T>
void Clock<T>::RescheduleClient(uint32_t clientType, uint32_t batchTicks) {
    if (batchTicks == 0) return UnscheduleClient(clientType);

    Client& client{clients[clientType]};

    if (client.batchedTicks > 0) {
        const uint64_t accumulatedTicks = (accTime - client.lastUpdate) / client.period;
        if (accumulatedTicks > batchTicks) batchTicks = accumulatedTicks;
    }

    client.batchedTicks = batchTicks;
    uint64_t nextUpdate = client.nextUpdate = client.lastUpdate + batchTicks * client.period;

    int32_t idx = -1;
    bool inserted = false;
    bool deleted = false;
    const uint32_t nextClientTypeBeforeReschedule = queue[clientType];

    do {
        uint32_t nextClientType = queue[idx];

        if (nextClientType == clientType) {
            queue[idx] = nextClientTypeBeforeReschedule;
            nextClientType = queue[idx];

            deleted = true;
            if (inserted) break;
        }

        if (!inserted && (nextClientType > CLOCK_CLIENT_MAX ||
                          clients[nextClientType].nextUpdate > nextUpdate)) {
            queue[idx] = clientType;
            queue[clientType] = nextClientType;

            inserted = true;
            if (deleted) break;
        }

        idx = nextClientType;
    } while (idx <= CLOCK_CLIENT_MAX);

    UpdateNextUpdate();
}

template <typename T>
void Clock<T>::UnscheduleClient(uint32_t clientType) {
    clients[clientType].batchedTicks = 0;

    for (int32_t idx = -1; idx <= CLOCK_CLIENT_MAX; idx = queue[idx]) {
        const uint32_t nextClientType = queue[idx];

        if (nextClientType == clientType) {
            queue[idx] = queue[nextClientType];
            break;
        }
    }

    UpdateNextUpdate();
}

template <typename T>
uint64_t Clock<T>::CyclesToNextUpdate(uint64_t cyclesPerSecond) {
    return ((nextUpdate - accTime) * cyclesPerSecond) / 1_sec + 1;
}

template <typename T>
void Clock<T>::Advance(uint64_t cycles, uint64_t cyclesPerSecond) {
    accTime += ((cycles * 1_sec) / cyclesPerSecond);
    if (accTime < nextUpdate) return;

    while (true) {
        const uint32_t clientType = queue[-1];
        if (clientType > CLOCK_CLIENT_MAX) break;

        Client& client{clients[clientType]};

        if (client.nextUpdate > accTime) break;

        const uint32_t batchTicks = dispatchDelegate.DispatchTicks(clientType, client.batchedTicks);
        client.lastUpdate += client.batchedTicks * client.period;
        client.batchedTicks = 0;

        RescheduleClient(clientType, batchTicks);
    }

    UpdateNextUpdate();
}

template <typename T>
void Clock<T>::UpdateNextUpdate() {
    if (queue[-1] <= CLOCK_CLIENT_MAX) {
        nextUpdate = clients[queue[-1]].nextUpdate;
    } else {
        nextUpdate = accTime + 1_sec;
    }
}

#endif  // _CLOCK_H_