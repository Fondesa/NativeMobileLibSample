#pragma once

#include <gmock/gmock.h>
#include "core/include_macros.hpp"
#include AMALGAMATION(clock.hpp)

namespace Time {

class ClockMock : public Clock {
   public:
    MOCK_METHOD(time_t, currentTimeSeconds, (), (override));
};
}