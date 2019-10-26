#pragma once

#include <memory>
#include "core/include_macros.hpp"
#include "notes_repository.hpp"
#include "drafts_repository.hpp"
#include AMALGAMATION(notes_interactor.hpp)

class NotesInteractorImpl : public NotesInteractor {
   public:
    NotesInteractorImpl(std::shared_ptr<NotesRepository> notesRepository,
                        std::shared_ptr<DraftsRepository> draftsRepository);

    void insertNote(Draft note) override;

    void updateNote(int id, Draft note) override;

    std::vector<Note> getAllNotes() override;

    std::vector<Note> getNotesByText(std::string text) override;

    stdx::optional<Draft> getNewDraft() override;

    stdx::optional<Draft> getExistingDraft(int id) override;

    void updateNewDraftTitle(std::string title) override;

    void updateNewDraftDescription(std::string description) override;

    void updateExistingDraftTitle(int id, std::string title) override;

    void updateExistingDraftDescription(int id, std::string description) override;

    void deleteNote(int id) override;

    void deleteNewDraft() override;

    void deleteExistingDraft(int id) override;

    void persistChanges() override;

   private:
    std::shared_ptr<NotesRepository> notesRepository;
    std::shared_ptr<DraftsRepository> draftsRepository;
};
