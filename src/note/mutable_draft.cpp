#include "core/compat_bad_optional_access_exception.hpp"
#include "mutable_draft.hpp"
#include "incomplete_draft_exception.hpp"

std::string MutableDraft::requireTitle() const {
    if (!title) {
        throw CompatBadOptionalAccessException();
    }
    return *title;
}

std::string MutableDraft::requireDescription() const {
    if (!description) {
        throw CompatBadOptionalAccessException();
    }
    return *description;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"

void MutableDraft::updateTitle(std::string title) {
    this->title = title;
}

void MutableDraft::updateDescription(std::string description) {
    this->description = description;
}

bool MutableDraft::hasTitle() const {
    return title.has_value();
}

bool MutableDraft::hasDescription() const {
    return description.has_value();
}

bool MutableDraft::isIncomplete() const {
    return !title || !description;
}

Draft MutableDraft::toDraft() {
    if (isIncomplete()) {
        throw IncompleteDraftException(*this);
    }
    return Draft(requireTitle(), requireDescription());
}

#pragma clang diagnostic pop
