#pragma once

#include <string>
#include "core/include_macros.hpp"
#include AMALGAMATION(note.hpp)
#include AMALGAMATION(draft.hpp)
#include AMALGAMATION(std_optional_compat.hpp)

class DraftsRepository {
   public:
    virtual void updateNewTitle(std::string title) = 0;

    virtual void updateNewDescription(std::string description) = 0;

    virtual void updateExistingTitle(int id, std::string title) = 0;

    virtual void updateExistingDescription(int id, std::string description) = 0;

    virtual void persist() = 0;

    virtual void deleteAll() = 0;

    virtual void deleteNew() = 0;

    virtual void deleteExisting(int id) = 0;

    virtual stdx::optional<Draft> getNew() = 0;

    virtual stdx::optional<Draft> getExisting(int id) = 0;
};
