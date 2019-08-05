#pragma once

#include <string>
#include "note.hpp"
#include "draft_note.hpp"

class DraftNotesRepository {
   public:
    virtual void updateNewDraftTitle(std::string title) = 0;

    virtual void updateNewDraftDescription(std::string description) = 0;

    virtual void updateExistingDraftTitle(int id, std::string title) = 0;

    virtual void updateExistingDraftDescription(int id, std::string description) = 0;

    virtual void persistDraftNotes() = 0;

    virtual void clearDrafts() = 0;

    virtual void removeDraftCreation() = 0;

    virtual void removeDraftUpdate(int id) = 0;

    virtual std::optional<DraftNote> getDraftCreationNote() = 0;

    virtual std::optional<DraftNote> getDraftUpdateNote(int id) = 0;
};
