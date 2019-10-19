#include "mutable_draft.hpp"
#include "core/compat_bad_optional_access_exception.hpp"
#include "incomplete_draft_exception.hpp"
#include "core/exception_macros.hpp"

std::string MutableDraft::requireTitle() const {
    if (!title) {
        THROW(CompatBadOptionalAccessException());
    }
    return *title;
}

std::string MutableDraft::requireDescription() const {
    if (!description) {
        THROW(CompatBadOptionalAccessException());
    }
    return *description;
}

void MutableDraft::updateTitle(std::string title) {
    this->title = title;
}

void MutableDraft::updateDescription(std::string description) {
    this->description = description;
}

bool MutableDraft::hasTitle() const {
    return title ? true : false;
}

bool MutableDraft::hasDescription() const {
    return description ? true : false;
}

bool MutableDraft::isIncomplete() const {
    return !title || !description;
}

Draft MutableDraft::toDraft() {
    if (isIncomplete()) {
        THROW(IncompleteDraftException(*this));
    }
    return Draft(*title, *description);
}

bool operator==(const MutableDraft &first, const MutableDraft &second) {
    return first.title == second.title && first.description == second.description;
}
