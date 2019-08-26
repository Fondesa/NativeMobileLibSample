#pragma once

#include <exception>

/**
 * This class is needed because std::bad_optional_access() is unavailable below iOS 12.0.
 */
class CompatibleBadOptionalAccessException : public std::exception {
   public:
    explicit CompatibleBadOptionalAccessException() = default;

    const char *what() const noexcept override;
};