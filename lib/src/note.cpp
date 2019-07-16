#include "note.hpp"

Note::Note(int id, std::string title, std::string description) {
    this->id = std::move(id);
    this->title = std::move(title);
    this->description = std::move(description);
}

int Note::getId() const {
    return id;
}

std::string Note::getTitle() const {
    return title;
}

std::string Note::getDescription() const {
    return description;
}
