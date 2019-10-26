#include "clock_impl.hpp"

namespace Time {

time_t ClockImpl::currentTimeSeconds() {
    time_t now;
    return time(&now);
}
}