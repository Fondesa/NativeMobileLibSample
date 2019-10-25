#include "time_format.hpp"

namespace Time::Format {

ISO_8601 format(time_t time) {
    char timeBuffer[sizeof "0000-00-00T00:00:00Z"];
    strftime(timeBuffer, sizeof timeBuffer, "%FT%TZ", gmtime(&time));
    // TODO remove
    // this will work too, if your compiler doesn't support %F or %T:
    //strftime(timeBuffer, sizeof timeBuffer, "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));
    return timeBuffer;
}
}