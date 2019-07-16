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
