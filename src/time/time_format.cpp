#include "core/include_macros.hpp"
#include AMALGAMATION(time_format.hpp)

namespace Time::Format {

ISO_8601 format(std::time_t time) {
    // Create a buffer of the same result's size.
    char timeBuffer[sizeof "0000-00-00T00:00:00Z"];
    // Convert the time in seconds to the ISO-8601 format.
    strftime(timeBuffer, sizeof timeBuffer, "%FT%TZ", gmtime(&time));
    return timeBuffer;
}

std::time_t parse(ISO_8601 formattedDate) {
    auto movedDate = std::move(formattedDate);
    tm tm{};
    // Convert the ISO-8601 to a time struct.
    strptime(movedDate.c_str(), "%FT%TZ", &tm);
    // Get the time_t value for the struct.
    return timegm(&tm);
}
}