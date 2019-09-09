#pragma once

#include <exception>

/**
 * This class is needed because std::bad_optional_access() is unavailable below iOS 12.0.
 */
class CompatBadOptionalAccessException : public std::exception {
   public:
    explicit CompatBadOptionalAccessException() = default;

    [[nodiscard]] const char *what() const noexcept override;
};