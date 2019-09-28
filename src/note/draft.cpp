#include <utility>

#include "draft.hpp"

Draft::Draft(std::string title, std::string description) {
    this->title = std::move(title);
    this->description = std::move(description);
}

std::string Draft::getTitle() const {
    return title;
}

std::string Draft::getDescription() const {
    return description;
}

void Draft::setTitle(std::string title) {
    this->title = std::move(title);
}

void Draft::setDescription(std::string description) {
    this->description = std::move(description);
}
