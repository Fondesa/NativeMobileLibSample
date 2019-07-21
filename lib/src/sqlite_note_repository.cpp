#include "sqlite_note_repository.hpp"

SQLiteNoteRepository::SQLiteNoteRepository(const SQLiteHandler& handler) : handler(handler) {}

void SQLiteNoteRepository::insert(DraftNote draftNote) {
    auto movedDraftNote = std::move(draftNote);
    int noteId = currentId++;
    notes.emplace_back(noteId, movedDraftNote.getTitle(), movedDraftNote.getDescription());
}

void SQLiteNoteRepository::remove(int id) {
    notes.erase(std::remove_if(notes.begin(), notes.end(), [id](Note note) {
                    return note.getId() == id;
                }),
                notes.end());
}

void SQLiteNoteRepository::update(int id, DraftNote draftNote) {
    auto movedDraftNote = std::move(draftNote);
    auto idIterator = std::find_if(notes.begin(), notes.end(), [id](Note note) {
        return note.getId() == id;
    });
    if (idIterator != notes.end()) {
        int index = std::distance(notes.begin(), idIterator);
        notes[index] = Note(id, movedDraftNote.getTitle(), movedDraftNote.getDescription());
    }
}

std::vector<Note> SQLiteNoteRepository::getAll() {
    return notes;
}