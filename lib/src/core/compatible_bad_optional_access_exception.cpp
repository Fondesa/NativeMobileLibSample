#include "compatible_bad_optional_access_exception.hpp"

const char *CompatibleBadOptionalAccessException::what() const noexcept {
    return "Bad optional access.";
}
