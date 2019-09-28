#include "incomplete_draft_exception.hpp"

IncompleteDraftException::IncompleteDraftException(const MutableDraft &draft) {
    auto hasTitle = draft.hasTitle();
    auto hasDescription = draft.hasDescription();
    if (hasTitle && hasDescription) {
        this->msg = "The new draft is complete so this exception mustn't be thrown, use another exception.";
        return;
    }
    this->msg = "The following fields of the new draft aren't set: ";
    if (!hasTitle) {
        msg += "title";
        if (!hasDescription) {
            msg += ", ";
        }
    }
    if (!hasDescription) {
        msg += "description";
    }
}

IncompleteDraftException::IncompleteDraftException(int id, const MutableDraft &draft) {
    auto hasTitle = draft.hasTitle();
    auto hasDescription = draft.hasDescription();
    if (hasTitle && hasDescription) {
        this->msg = "The draft with id " +
            std::to_string(id) +
            " is complete so this exception mustn't be thrown, use another exception.";
        return;
    }
    this->msg = "The following fields of the existing draft with id " +
        std::to_string(id) +
        " aren't set: ";
    if (!hasTitle) {
        msg += "title";
        if (!hasDescription) {
            msg += ", ";
        }
    }
    if (!hasDescription) {
        msg += "description";
    }
}

const char *IncompleteDraftException::what() const noexcept {
    return msg.c_str();
}
