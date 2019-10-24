#include "date_iso.hpp"

namespace Date {

std::string getCurrentISODate() {
    time_t now;
    time(&now);
    char timeBuffer[sizeof "0000-00-00T00:00:00Z"];
    strftime(timeBuffer, sizeof timeBuffer, "%FT%TZ", gmtime(&now));
    // this will work too, if your compiler doesn't support %F or %T:
    //strftime(timeBuffer, sizeof timeBuffer, "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));
    return timeBuffer;
}
}