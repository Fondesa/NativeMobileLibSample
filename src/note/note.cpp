#include "note.hpp"

Note::Note(int id, std::string title, std::string description) {
    this->id = id;
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

bool operator==(const Note &first, const Note &second) {
    return first.id == second.id &&
        first.title == second.title &&
        first.description == second.description;
}
