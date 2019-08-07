#include <utility>
#include "mutable_draft.hpp"
#include "incomplete_draft_exception.hpp"

std::string MutableDraft::requireTitle() const {
    // It will throw a std::bad_optional_access if the value isn't set yet.
    return title.value();
}

std::string MutableDraft::requireDescription() const {
    // It will throw a std::bad_optional_access if the value isn't set yet.
    return description.value();
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
