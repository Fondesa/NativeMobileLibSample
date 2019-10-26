#pragma once

#include <gmock/gmock.h>
#include "clock.hpp"

namespace Time {

class ClockMock : public Clock {
   public:
    MOCK_METHOD(time_t, currentTimeSeconds, (), (override));
};
}