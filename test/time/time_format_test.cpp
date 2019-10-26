#include <gtest/gtest.h>
#include "core/include_macros.hpp"
#include AMALGAMATION(time_format.hpp)

TEST(TimeFormatTest, givenTimeInSecondsWhenFormatIsInvokedThenISO_8601TimeIsReturned) {
    Time::Format::ISO_8601 formattedTime = Time::Format::format(1572085165);

    ASSERT_EQ("2019-10-26T10:19:25Z", formattedTime);
}