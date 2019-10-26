#include "time/clock_impl.hpp"
#include <gtest/gtest.h>

TEST(ClockImplTest, whenCurrentTimeSecondsIsInvokedThenValidTimeIsReturned) {
    auto clock = Time::ClockImpl();
    time_t now;
    time(&now);

    ASSERT_LE(now, clock.currentTimeSeconds());
}