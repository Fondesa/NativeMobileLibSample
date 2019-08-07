#pragma once

#include <optional>
#include <vector>
#include "note.hpp"
#include "draft.hpp"

class NotesInteractor {
   public:
    virtual void insertNote(Draft note) = 0;

    virtual void removeNote(int id) = 0;

    virtual void updateNote(int id, Draft note) = 0;

    virtual void updateNewDraftTitle(std::string title) = 0;

    virtual void updateNewDraftDescription(std::string description) = 0;

    virtual void updateExistingDraftTitle(int id, std::string title) = 0;

    virtual void updateExistingDraftDescription(int id, std::string description) = 0;

    virtual std::vector<Note> getAllNotes() = 0;

    virtual std::optional<Draft> getNewDraftNote() = 0;

    virtual std::optional<Draft> getExistingDraftNote(int id) = 0;
};
