#include "illegal_state_exception.hpp"

IllegalStateException::IllegalStateException(std::string msg) {
    auto movedMsg = std::move(msg);
    this->msg = movedMsg.c_str();
}

const char *IllegalStateException::what() const noexcept {
    return msg;
}

