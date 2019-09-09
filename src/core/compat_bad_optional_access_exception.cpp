#include "compat_bad_optional_access_exception.hpp"

const char *CompatBadOptionalAccessException::what() const noexcept {
    return "Bad optional access.";
}
