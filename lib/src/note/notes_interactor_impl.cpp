#include <utility>
#include "notes_interactor_impl.hpp"

NotesInteractorImpl::NotesInteractorImpl(std::shared_ptr<NotesRepository> notesRepository,
                                         std::shared_ptr<DraftsRepository> draftsRepository) :
    notesRepository(std::move(notesRepository)),
    draftsRepository(std::move(draftsRepository)) {}

void NotesInteractorImpl::insertNote(Draft note) {
    notesRepository->insert(note);
    // The draft isn't needed anymore if the note is saved.
    deleteNewDraft();
}

void NotesInteractorImpl::updateNote(int id, Draft note) {
    notesRepository->update(id, note);
    // The draft isn't needed anymore if the note is saved.
    draftsRepository->deleteExisting(id);
}

std::vector<Note> NotesInteractorImpl::getAllNotes() {
    return notesRepository->getAll();
}

std::vector<Note> NotesInteractorImpl::getNotesByText(std::string text) {
    return notesRepository->getByText(text);
}

std::optional<Draft> NotesInteractorImpl::getNewDraft() {
    return draftsRepository->getNew();
}

std::optional<Draft> NotesInteractorImpl::getExistingDraft(int id) {
    return draftsRepository->getExisting(id);
}

void NotesInteractorImpl::updateNewDraftTitle(std::string title) {
    draftsRepository->updateNewTitle(title);
}

void NotesInteractorImpl::updateNewDraftDescription(std::string description) {
    draftsRepository->updateNewDescription(description);
}

void NotesInteractorImpl::updateExistingDraftTitle(int id, std::string title) {
    draftsRepository->updateExistingTitle(id, title);
}

void NotesInteractorImpl::updateExistingDraftDescription(int id, std::string description) {
    draftsRepository->updateExistingDescription(id, description);
}

void NotesInteractorImpl::deleteNote(int id) {
    notesRepository->deleteWithId(id);
    // The draft should be deleted too since it can't be updated again.
    deleteExistingDraft(id);
}

void NotesInteractorImpl::deleteNewDraft() {
    draftsRepository->deleteNew();
}

void NotesInteractorImpl::deleteExistingDraft(int id) {
    draftsRepository->deleteExisting(id);
}

void NotesInteractorImpl::persistChanges() {
    draftsRepository->persist();
}
