#pragma once

#include "core/include_macros.hpp"
#include AMALGAMATION(clock.hpp)

namespace Time {

class ClockImpl : public Clock {
   public:
    time_t currentTimeSeconds() override;
};
}