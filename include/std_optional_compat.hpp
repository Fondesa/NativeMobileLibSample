#pragma once

#if __has_include(<optional>)
#include <optional>
namespace stdx = std;
#elif __has_include(<experimental/optional>)
#include <experimental/optional>
namespace stdx = std::experimental;
#else
#error Must have an optional type, either from <optional> or from <experimental/optional>.
#endif