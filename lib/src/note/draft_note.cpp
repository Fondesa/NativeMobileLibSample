#include <utility>

#include "draft_note.hpp"

DraftNote::DraftNote(std::string title, std::string description) {
    this->title = std::move(title);
    this->description = std::move(description);
}

std::string DraftNote::getTitle() const {
    return title;
}

std::string DraftNote::getDescription() const {
    return description;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"

void DraftNote::setTitle(std::string title) {
    this->title = std::move(title);
}

void DraftNote::setDescription(std::string description) {
    this->description = std::move(description);
}

#pragma clang diagnostic pop
