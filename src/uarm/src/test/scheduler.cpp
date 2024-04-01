#include "../uarm/scheduler.h"

#include <gtest/gtest.h>

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

namespace {
    uint64_t operator""_mhz(uint64_t valueMhz) { return valueMhz * 1000000; }

    class DispatchDelegate {
       public:
        struct Invocation {
            uint32_t clientType;
            uint32_t batchedTicks;
        };

       public:
        void Reset() { invocations.clear(); }

        size_t GetInvocationCount() const { return invocations.size(); }

        const Invocation& GetInvocation(size_t index) const { return invocations[index]; }

        void ExpectInvocation(size_t index, uint32_t clientType, uint32_t batchedTicks) const {
            ASSERT_LT(index, GetInvocationCount());
            EXPECT_EQ(invocations[index].clientType, clientType);
            EXPECT_EQ(invocations[index].batchedTicks, batchedTicks);
        }

        uint32_t DispatchTicks(uint32_t clientType, uint32_t batchedTicks) {
            invocations.push_back({clientType, batchedTicks});

            return BatchedTicksForInvocation(invocations.size() - 1);
        }

        virtual uint32_t BatchedTicksForInvocation(uint32_t invocationIndex) {
            return invocations[invocationIndex].batchedTicks;
        }

       protected:
        std::vector<const Invocation> invocations;
    };

    TEST(Scheduler, ClientsAreScheduledInAppropiateOrder) {
        DispatchDelegate dispatchDelegate;
        Scheduler<DispatchDelegate> scheduler(dispatchDelegate);

        scheduler.ScheduleClient(SCHEDULER_CLIENT_TIMER, 50_usec, 1);
        scheduler.ScheduleClient(SCHEDULER_CLIENT_RTC, 110_usec, 1);

        scheduler.Advance(49, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        scheduler.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, SCHEDULER_CLIENT_TIMER, 1);
        dispatchDelegate.Reset();

        scheduler.Advance(49, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        scheduler.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, SCHEDULER_CLIENT_TIMER, 1);
        dispatchDelegate.Reset();

        scheduler.Advance(9, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        scheduler.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, SCHEDULER_CLIENT_RTC, 1);
        dispatchDelegate.Reset();

        scheduler.Advance(39, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        scheduler.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, SCHEDULER_CLIENT_TIMER, 1);
        dispatchDelegate.Reset();
    }

    TEST(Scheduler, BatchedTicksAreScheduledBatched) {
        DispatchDelegate dispatchDelegate;
        Scheduler<DispatchDelegate> scheduler(dispatchDelegate);

        scheduler.ScheduleClient(SCHEDULER_CLIENT_TIMER, 50_usec, 3);
        scheduler.ScheduleClient(SCHEDULER_CLIENT_RTC, 110_usec, 1);

        scheduler.Advance(109, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        scheduler.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, SCHEDULER_CLIENT_RTC, 1);
        dispatchDelegate.Reset();

        scheduler.Advance(39, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        scheduler.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, SCHEDULER_CLIENT_TIMER, 3);
        dispatchDelegate.Reset();
    }

    TEST(Scheduler, UnscheduleRemovesClientFromTopOfQueue) {
        DispatchDelegate dispatchDelegate;
        Scheduler<DispatchDelegate> scheduler(dispatchDelegate);

        scheduler.ScheduleClient(SCHEDULER_CLIENT_TIMER, 50_usec, 1);
        scheduler.ScheduleClient(SCHEDULER_CLIENT_RTC, 110_usec, 1);
        scheduler.ScheduleClient(SCHEDULER_CLIENT_LCD, 130_usec, 1);

        scheduler.UnscheduleClient(SCHEDULER_CLIENT_TIMER);

        scheduler.Advance(109, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        scheduler.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, SCHEDULER_CLIENT_RTC, 1);
        dispatchDelegate.Reset();

        scheduler.Advance(19, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        scheduler.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, SCHEDULER_CLIENT_LCD, 1);
        dispatchDelegate.Reset();

        scheduler.Advance(89, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        scheduler.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, SCHEDULER_CLIENT_RTC, 1);
        dispatchDelegate.Reset();
    }

    TEST(Scheduler, UnscheduleRemovesClientFromMiddleOfQueue) {
        DispatchDelegate dispatchDelegate;
        Scheduler<DispatchDelegate> scheduler(dispatchDelegate);

        scheduler.ScheduleClient(SCHEDULER_CLIENT_TIMER, 50_usec, 1);
        scheduler.ScheduleClient(SCHEDULER_CLIENT_RTC, 110_usec, 1);
        scheduler.ScheduleClient(SCHEDULER_CLIENT_LCD, 130_usec, 1);

        scheduler.UnscheduleClient(SCHEDULER_CLIENT_RTC);

        scheduler.Advance(49, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        scheduler.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, SCHEDULER_CLIENT_TIMER, 1);
        dispatchDelegate.Reset();

        scheduler.Advance(49, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        scheduler.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, SCHEDULER_CLIENT_TIMER, 1);
        dispatchDelegate.Reset();

        scheduler.Advance(29, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        scheduler.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, SCHEDULER_CLIENT_LCD, 1);
        dispatchDelegate.Reset();
    }

    TEST(Scheduler, UnscheduleRemovesClientFromEndOfQueue) {
        DispatchDelegate dispatchDelegate;
        Scheduler<DispatchDelegate> scheduler(dispatchDelegate);

        scheduler.ScheduleClient(SCHEDULER_CLIENT_TIMER, 50_usec, 1);
        scheduler.ScheduleClient(SCHEDULER_CLIENT_RTC, 110_usec, 1);
        scheduler.ScheduleClient(SCHEDULER_CLIENT_LCD, 130_usec, 1);

        scheduler.UnscheduleClient(SCHEDULER_CLIENT_LCD);

        scheduler.Advance(49, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        scheduler.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, SCHEDULER_CLIENT_TIMER, 1);
        dispatchDelegate.Reset();

        scheduler.Advance(49, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        scheduler.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, SCHEDULER_CLIENT_TIMER, 1);
        dispatchDelegate.Reset();

        scheduler.Advance(9, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        scheduler.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, SCHEDULER_CLIENT_RTC, 1);
        dispatchDelegate.Reset();

        scheduler.Advance(39, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        scheduler.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, SCHEDULER_CLIENT_TIMER, 1);
        dispatchDelegate.Reset();
    }

    TEST(Scheduler, ReschedulingBatchesOverdueTicks) {
        DispatchDelegate dispatchDelegate;
        Scheduler<DispatchDelegate> scheduler(dispatchDelegate);

        scheduler.ScheduleClient(SCHEDULER_CLIENT_TIMER, 10_usec, 10);
        scheduler.ScheduleClient(SCHEDULER_CLIENT_RTC, 55_usec, 1);

        scheduler.Advance(42, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        scheduler.RescheduleClient(SCHEDULER_CLIENT_TIMER, 1);

        scheduler.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, SCHEDULER_CLIENT_TIMER, 4);
        dispatchDelegate.Reset();

        scheduler.RescheduleClient(SCHEDULER_CLIENT_TIMER, 1);

        scheduler.Advance(6, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        scheduler.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, SCHEDULER_CLIENT_TIMER, 1);
        dispatchDelegate.Reset();

        scheduler.Advance(4, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        scheduler.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, SCHEDULER_CLIENT_RTC, 1);
        dispatchDelegate.Reset();
    }

    TEST(Scheduler, ReschedulingPrefersRequestedBatchSizeIfThereAreLessOverdueTicks) {
        DispatchDelegate dispatchDelegate;
        Scheduler<DispatchDelegate> scheduler(dispatchDelegate);

        scheduler.ScheduleClient(SCHEDULER_CLIENT_TIMER, 10_usec, 10);
        scheduler.ScheduleClient(SCHEDULER_CLIENT_RTC, 55_usec, 1);

        scheduler.Advance(42, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        scheduler.RescheduleClient(SCHEDULER_CLIENT_TIMER, 5);

        scheduler.Advance(7, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        scheduler.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, SCHEDULER_CLIENT_TIMER, 5);
        dispatchDelegate.Reset();
    }

    TEST(Scheduler, TicksAreBatchedAccordingToDispatcher) {
        class CustomDispatchDelegate : public DispatchDelegate {
           public:
            uint32_t BatchedTicksForInvocation(uint32_t invocationIndex) override {
                return invocations[invocationIndex].clientType == SCHEDULER_CLIENT_TIMER ? 2 : 1;
            }
        };

        CustomDispatchDelegate dispatchDelegate;
        Scheduler<CustomDispatchDelegate> scheduler(dispatchDelegate);

        scheduler.ScheduleClient(SCHEDULER_CLIENT_TIMER, 10_usec, 3);
        scheduler.ScheduleClient(SCHEDULER_CLIENT_RTC, 55_usec, 1);

        scheduler.Advance(29, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        scheduler.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, SCHEDULER_CLIENT_TIMER, 3);
        dispatchDelegate.Reset();

        scheduler.Advance(19, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        scheduler.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, SCHEDULER_CLIENT_TIMER, 2);
        dispatchDelegate.Reset();

        scheduler.Advance(4, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        scheduler.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, SCHEDULER_CLIENT_RTC, 1);
        dispatchDelegate.Reset();

        scheduler.Advance(14, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        scheduler.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, SCHEDULER_CLIENT_TIMER, 2);
        dispatchDelegate.Reset();
    }

    TEST(Scheduler, ZeroBatchedTicksDisablesClient) {
        class CustomDispatchDelegate : public DispatchDelegate {
           public:
            uint32_t BatchedTicksForInvocation(uint32_t invocationIndex) override {
                return invocations[invocationIndex].clientType == SCHEDULER_CLIENT_TIMER ? 0 : 1;
            }
        };

        CustomDispatchDelegate dispatchDelegate;
        Scheduler<CustomDispatchDelegate> scheduler(dispatchDelegate);

        scheduler.ScheduleClient(SCHEDULER_CLIENT_TIMER, 10_usec, 3);
        scheduler.ScheduleClient(SCHEDULER_CLIENT_RTC, 55_usec, 1);

        scheduler.Advance(29, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        scheduler.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, SCHEDULER_CLIENT_TIMER, 3);
        dispatchDelegate.Reset();

        scheduler.Advance(24, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        scheduler.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, SCHEDULER_CLIENT_RTC, 1);
        dispatchDelegate.Reset();

        scheduler.Advance(54, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        scheduler.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, SCHEDULER_CLIENT_RTC, 1);
        dispatchDelegate.Reset();
    }

    TEST(Scheduler, ReschedulingDropsOverdueTicksOnBatchSizeZero) {
        DispatchDelegate dispatchDelegate;
        Scheduler<DispatchDelegate> scheduler(dispatchDelegate);

        scheduler.ScheduleClient(SCHEDULER_CLIENT_TIMER, 10_usec, 10);
        scheduler.ScheduleClient(SCHEDULER_CLIENT_RTC, 55_usec, 1);

        scheduler.Advance(42, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        scheduler.RescheduleClient(SCHEDULER_CLIENT_TIMER, 0);

        scheduler.Advance(12, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        scheduler.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, SCHEDULER_CLIENT_RTC, 1);
        dispatchDelegate.Reset();
    }

}  // namespace