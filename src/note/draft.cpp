#include <utility>
#include "core/include_macros.hpp"
#include AMALGAMATION(draft.hpp)

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

bool operator==(const Draft &first, const Draft &second) {
    return first.title == second.title && first.description == second.description;
}
