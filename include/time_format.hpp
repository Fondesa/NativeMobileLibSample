#pragma once

#include <string>

namespace Time::Format {

typedef std::string ISO_8601;

ISO_8601 format(time_t time);
}