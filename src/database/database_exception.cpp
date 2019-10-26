#include "core/include_macros.hpp"
#include AMALGAMATION(database_exception.hpp)

namespace Db {

Exception::Exception(std::string msg) {
    this->msg = std::move(msg);
}

const char *Exception::what() const noexcept {
    return msg.c_str();
}
}