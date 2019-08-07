#include "notes_interactor_impl.hpp"

void NotesInteractorImpl::insertNote(Draft note) {

}

void NotesInteractorImpl::removeNote(int id) {

}

void NotesInteractorImpl::updateNote(int id, Draft note) {

}

void NotesInteractorImpl::updateNewDraftTitle(std::string title) {

}

void NotesInteractorImpl::updateNewDraftDescription(std::string description) {

}

void NotesInteractorImpl::updateExistingDraftTitle(int id, std::string title) {

}

void NotesInteractorImpl::updateExistingDraftDescription(int id, std::string description) {

}

std::vector<Note> NotesInteractorImpl::getAllNotes() {
    return std::vector<Note>();
}

std::optional<Draft> NotesInteractorImpl::getNewDraftNote() {
    return std::optional<Draft>();
}

std::optional<Draft> NotesInteractorImpl::getExistingDraftNote(int id) {
    return std::optional<Draft>();
}

