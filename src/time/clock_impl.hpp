#pragma once

#include "clock.hpp"

namespace Time {

class ClockImpl : public Clock {
   public:
    time_t currentTimeSeconds() override;
};
}