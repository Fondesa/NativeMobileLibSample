#include "in_memory_note_repository.hpp"

void InMemoryNoteRepository::insert(Draft draftNote) {
    auto movedDraftNote = std::move(draftNote);
    int noteId = currentId++;
    notes.emplace_back(noteId, movedDraftNote.getTitle(), movedDraftNote.getDescription());
}

void InMemoryNoteRepository::remove(int id) {
    notes.erase(std::remove_if(notes.begin(), notes.end(), [id](Note note) {
                    return note.getId() == id;
                }),
                notes.end());
}

void InMemoryNoteRepository::update(int id, Draft draftNote) {
    auto movedDraftNote = std::move(draftNote);
    auto idIterator = std::find_if(notes.begin(), notes.end(), [id](Note note) {
        return note.getId() == id;
    });
    if (idIterator != notes.end()) {
        int index = std::distance(notes.begin(), idIterator);
        notes[index] = Note(id, movedDraftNote.getTitle(), movedDraftNote.getDescription());
    }
}

std::vector<Note> InMemoryNoteRepository::getAll() {
    return notes;
}