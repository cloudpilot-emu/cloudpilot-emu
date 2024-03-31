#include "../uarm/clock.h"

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

    TEST(Clock, ClientsAreScheduledInAppropiateOrder) {
        DispatchDelegate dispatchDelegate;
        Clock<DispatchDelegate> clock(dispatchDelegate);

        clock.ScheduleClient(CLOCK_CLIENT_TIMER, 50_usec, 1);
        clock.ScheduleClient(CLOCK_CLIENT_RTC, 110_usec, 1);

        clock.Advance(49, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        clock.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, CLOCK_CLIENT_TIMER, 1);
        dispatchDelegate.Reset();

        clock.Advance(49, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        clock.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, CLOCK_CLIENT_TIMER, 1);
        dispatchDelegate.Reset();

        clock.Advance(9, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        clock.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, CLOCK_CLIENT_RTC, 1);
        dispatchDelegate.Reset();

        clock.Advance(39, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        clock.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, CLOCK_CLIENT_TIMER, 1);
        dispatchDelegate.Reset();
    }

    TEST(Clock, BatchedTicksAreScheduledBatched) {
        DispatchDelegate dispatchDelegate;
        Clock<DispatchDelegate> clock(dispatchDelegate);

        clock.ScheduleClient(CLOCK_CLIENT_TIMER, 50_usec, 3);
        clock.ScheduleClient(CLOCK_CLIENT_RTC, 110_usec, 1);

        clock.Advance(109, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        clock.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, CLOCK_CLIENT_RTC, 1);
        dispatchDelegate.Reset();

        clock.Advance(39, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        clock.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, CLOCK_CLIENT_TIMER, 3);
        dispatchDelegate.Reset();
    }

    TEST(Clock, UnscheduleRemovesClientFromTopOfQueue) {
        DispatchDelegate dispatchDelegate;
        Clock<DispatchDelegate> clock(dispatchDelegate);

        clock.ScheduleClient(CLOCK_CLIENT_TIMER, 50_usec, 1);
        clock.ScheduleClient(CLOCK_CLIENT_RTC, 110_usec, 1);
        clock.ScheduleClient(CLOCK_CLIENT_LCD, 130_usec, 1);

        clock.UnscheduleClient(CLOCK_CLIENT_TIMER);

        clock.Advance(109, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        clock.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, CLOCK_CLIENT_RTC, 1);
        dispatchDelegate.Reset();

        clock.Advance(19, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        clock.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, CLOCK_CLIENT_LCD, 1);
        dispatchDelegate.Reset();

        clock.Advance(89, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        clock.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, CLOCK_CLIENT_RTC, 1);
        dispatchDelegate.Reset();
    }

    TEST(Clock, UnscheduleRemovesClientFromMiddleOfQueue) {
        DispatchDelegate dispatchDelegate;
        Clock<DispatchDelegate> clock(dispatchDelegate);

        clock.ScheduleClient(CLOCK_CLIENT_TIMER, 50_usec, 1);
        clock.ScheduleClient(CLOCK_CLIENT_RTC, 110_usec, 1);
        clock.ScheduleClient(CLOCK_CLIENT_LCD, 130_usec, 1);

        clock.UnscheduleClient(CLOCK_CLIENT_RTC);

        clock.Advance(49, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        clock.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, CLOCK_CLIENT_TIMER, 1);
        dispatchDelegate.Reset();

        clock.Advance(49, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        clock.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, CLOCK_CLIENT_TIMER, 1);
        dispatchDelegate.Reset();

        clock.Advance(29, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        clock.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, CLOCK_CLIENT_LCD, 1);
        dispatchDelegate.Reset();
    }

    TEST(Clock, UnscheduleRemovesClientFromEndOfQueue) {
        DispatchDelegate dispatchDelegate;
        Clock<DispatchDelegate> clock(dispatchDelegate);

        clock.ScheduleClient(CLOCK_CLIENT_TIMER, 50_usec, 1);
        clock.ScheduleClient(CLOCK_CLIENT_RTC, 110_usec, 1);
        clock.ScheduleClient(CLOCK_CLIENT_LCD, 130_usec, 1);

        clock.UnscheduleClient(CLOCK_CLIENT_LCD);

        clock.Advance(49, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        clock.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, CLOCK_CLIENT_TIMER, 1);
        dispatchDelegate.Reset();

        clock.Advance(49, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        clock.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, CLOCK_CLIENT_TIMER, 1);
        dispatchDelegate.Reset();

        clock.Advance(9, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        clock.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, CLOCK_CLIENT_RTC, 1);
        dispatchDelegate.Reset();

        clock.Advance(39, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        clock.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, CLOCK_CLIENT_TIMER, 1);
        dispatchDelegate.Reset();
    }

    TEST(Clock, ReschedulingBatchesOverdueTicks) {
        DispatchDelegate dispatchDelegate;
        Clock<DispatchDelegate> clock(dispatchDelegate);

        clock.ScheduleClient(CLOCK_CLIENT_TIMER, 10_usec, 10);
        clock.ScheduleClient(CLOCK_CLIENT_RTC, 55_usec, 1);

        clock.Advance(42, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        clock.RescheduleClient(CLOCK_CLIENT_TIMER, 1);

        clock.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, CLOCK_CLIENT_TIMER, 4);
        dispatchDelegate.Reset();

        clock.RescheduleClient(CLOCK_CLIENT_TIMER, 1);

        clock.Advance(6, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        clock.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, CLOCK_CLIENT_TIMER, 1);
        dispatchDelegate.Reset();

        clock.Advance(4, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        clock.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, CLOCK_CLIENT_RTC, 1);
        dispatchDelegate.Reset();
    }

    TEST(Clock, ReschedulingPrefersRequestedBatchSizeIfThereAreLessOverdueTicks) {
        DispatchDelegate dispatchDelegate;
        Clock<DispatchDelegate> clock(dispatchDelegate);

        clock.ScheduleClient(CLOCK_CLIENT_TIMER, 10_usec, 10);
        clock.ScheduleClient(CLOCK_CLIENT_RTC, 55_usec, 1);

        clock.Advance(42, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        clock.RescheduleClient(CLOCK_CLIENT_TIMER, 5);

        clock.Advance(7, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        clock.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, CLOCK_CLIENT_TIMER, 5);
        dispatchDelegate.Reset();
    }

    TEST(Clock, TicksAreBatchedAccordingToDispatcher) {
        class CustomDispatchDelegate : public DispatchDelegate {
           public:
            uint32_t BatchedTicksForInvocation(uint32_t invocationIndex) override {
                return invocations[invocationIndex].clientType == CLOCK_CLIENT_TIMER ? 2 : 1;
            }
        };

        CustomDispatchDelegate dispatchDelegate;
        Clock<CustomDispatchDelegate> clock(dispatchDelegate);

        clock.ScheduleClient(CLOCK_CLIENT_TIMER, 10_usec, 3);
        clock.ScheduleClient(CLOCK_CLIENT_RTC, 55_usec, 1);

        clock.Advance(29, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        clock.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, CLOCK_CLIENT_TIMER, 3);
        dispatchDelegate.Reset();

        clock.Advance(19, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        clock.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, CLOCK_CLIENT_TIMER, 2);
        dispatchDelegate.Reset();

        clock.Advance(4, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        clock.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, CLOCK_CLIENT_RTC, 1);
        dispatchDelegate.Reset();

        clock.Advance(14, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        clock.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, CLOCK_CLIENT_TIMER, 2);
        dispatchDelegate.Reset();
    }

    TEST(Clock, ZeroBatchedTicksDisablesClient) {
        class CustomDispatchDelegate : public DispatchDelegate {
           public:
            uint32_t BatchedTicksForInvocation(uint32_t invocationIndex) override {
                return invocations[invocationIndex].clientType == CLOCK_CLIENT_TIMER ? 0 : 1;
            }
        };

        CustomDispatchDelegate dispatchDelegate;
        Clock<CustomDispatchDelegate> clock(dispatchDelegate);

        clock.ScheduleClient(CLOCK_CLIENT_TIMER, 10_usec, 3);
        clock.ScheduleClient(CLOCK_CLIENT_RTC, 55_usec, 1);

        clock.Advance(29, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        clock.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, CLOCK_CLIENT_TIMER, 3);
        dispatchDelegate.Reset();

        clock.Advance(24, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        clock.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, CLOCK_CLIENT_RTC, 1);
        dispatchDelegate.Reset();

        clock.Advance(54, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        clock.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, CLOCK_CLIENT_RTC, 1);
        dispatchDelegate.Reset();
    }

    TEST(Clock, ReschedulingDropsOverdueTicksOnBatchSizeZero) {
        DispatchDelegate dispatchDelegate;
        Clock<DispatchDelegate> clock(dispatchDelegate);

        clock.ScheduleClient(CLOCK_CLIENT_TIMER, 10_usec, 10);
        clock.ScheduleClient(CLOCK_CLIENT_RTC, 55_usec, 1);

        clock.Advance(42, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        clock.RescheduleClient(CLOCK_CLIENT_TIMER, 0);

        clock.Advance(12, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(0));

        clock.Advance(1, 1_mhz);
        EXPECT_EQ(dispatchDelegate.GetInvocationCount(), static_cast<size_t>(1));
        dispatchDelegate.ExpectInvocation(0, CLOCK_CLIENT_RTC, 1);
        dispatchDelegate.Reset();
    }

}  // namespace