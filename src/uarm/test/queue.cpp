#include "../uarm/queue.h"

#include <gtest/gtest.h>

#include <cstddef>

TEST(Queue, QueueStartsEmpty) {
    Queue<int> queue(3);

    EXPECT_EQ(queue.GetSize(), static_cast<size_t>(0));
}

TEST(Queue, QueueIsFifo) {
    Queue<int> queue(3);

    queue.Push(1);
    EXPECT_EQ(queue.GetSize(), static_cast<size_t>(1));

    queue.Push(2);
    EXPECT_EQ(queue.GetSize(), static_cast<size_t>(2));

    EXPECT_EQ(queue.Pop(), 1);
    EXPECT_EQ(queue.GetSize(), static_cast<size_t>(1));

    EXPECT_EQ(queue.Pop(), 2);
    EXPECT_EQ(queue.GetSize(), static_cast<size_t>(0));
}

TEST(Queue, QueueWraps) {
    Queue<int> queue(3);

    queue.Push(1);
    EXPECT_EQ(queue.GetSize(), static_cast<size_t>(1));

    queue.Push(2);
    EXPECT_EQ(queue.GetSize(), static_cast<size_t>(2));

    queue.Push(3);
    EXPECT_EQ(queue.GetSize(), static_cast<size_t>(3));

    queue.Push(4);
    EXPECT_EQ(queue.GetSize(), static_cast<size_t>(3));

    EXPECT_EQ(queue.Pop(), 2);
    EXPECT_EQ(queue.GetSize(), static_cast<size_t>(2));

    EXPECT_EQ(queue.Pop(), 3);
    EXPECT_EQ(queue.GetSize(), static_cast<size_t>(1));

    EXPECT_EQ(queue.Pop(), 4);
    EXPECT_EQ(queue.GetSize(), static_cast<size_t>(0));
}
