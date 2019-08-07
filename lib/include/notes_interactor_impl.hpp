#pragma once

#include "notes_interactor.hpp"

class NotesInteractorImpl : NotesInteractor {
   public:
    void insertNote(DraftNote note) override;

    void removeNote(int id) override;

    void updateNote(int id, DraftNote note) override;

    void updateNewDraftTitle(std::string title) override;

    void updateNewDraftDescription(std::string description) override;

    void updateExistingDraftTitle(int id, std::string title) override;

    void updateExistingDraftDescription(int id, std::string description) override;

    std::vector<Note> getAllNotes() override;

    std::optional<DraftNote> getNewDraftNote() override;

    std::optional<DraftNote> getExistingDraftNote(int id) override;
};
