#pragma once

#include "notes_interactor.hpp"

class NotesInteractorImpl : NotesInteractor {
   public:
    void insertNote(Draft note) override;

    void removeNote(int id) override;

    void updateNote(int id, Draft note) override;

    void updateNewDraftTitle(std::string title) override;

    void updateNewDraftDescription(std::string description) override;

    void updateExistingDraftTitle(int id, std::string title) override;

    void updateExistingDraftDescription(int id, std::string description) override;

    std::vector<Note> getAllNotes() override;

    std::optional<Draft> getNewDraft() override;

    std::optional<Draft> getExistingDraft(int id) override;
};
