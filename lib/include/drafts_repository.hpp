#pragma once

#include <string>
#include "note.hpp"
#include "draft.hpp"

class DraftsRepository {
   public:
    virtual void updateNewDraftTitle(std::string title) = 0;

    virtual void updateNewDraftDescription(std::string description) = 0;

    virtual void updateExistingDraftTitle(int id, std::string title) = 0;

    virtual void updateExistingDraftDescription(int id, std::string description) = 0;

    virtual void persist() = 0;

    virtual void clear() = 0;

    virtual void deleteNewDraft() = 0;

    virtual void deleteExistingDraft(int id) = 0;

    virtual std::optional<Draft> getDraftCreationNote() = 0;

    virtual std::optional<Draft> getDraftUpdateNote(int id) = 0;
};
