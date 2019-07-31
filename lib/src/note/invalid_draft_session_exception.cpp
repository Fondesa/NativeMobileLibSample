#include "invalid_draft_session_exception.hpp"

InvalidDraftSessionException::InvalidDraftSessionException(std::string msg) {
    this->msg = std::move(msg);
}

const char *InvalidDraftSessionException::what() const noexcept {
    return msg.c_str();
}