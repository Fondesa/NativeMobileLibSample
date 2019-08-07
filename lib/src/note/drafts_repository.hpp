#pragma once

#include <string>
#include <optional>
#include "note.hpp"
#include "draft.hpp"

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

    virtual std::optional<Draft> getNew() = 0;

    virtual std::optional<Draft> getExisting(int id) = 0;
};
