#include "core/include_macros.hpp"
#include AMALGAMATION(note.hpp)

Note::Note(int id, std::string title, std::string description, std::time_t lastUpdateDate) {
    this->id = id;
    this->title = std::move(title);
    this->description = std::move(description);
    this->lastUpdateDate = std::move(lastUpdateDate);
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

std::time_t Note::getLastUpdateTime() const {
    return lastUpdateDate;
}

bool operator==(const Note &first, const Note &second) {
    return first.id == second.id &&
        first.title == second.title &&
        first.description == second.description &&
        difftime(first.lastUpdateDate, second.lastUpdateDate) == 0;
}
