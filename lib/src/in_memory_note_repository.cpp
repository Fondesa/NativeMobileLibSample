#include "in_memory_note_repository.hpp"

void InMemoryNoteRepository::insert(const DraftNote &draftNote) {
    int noteId = currentId++;
    notes.push_back(Note(noteId, draftNote.getTitle(), draftNote.getDescription()));
}

void InMemoryNoteRepository::remove(int id) {
    notes.erase(std::remove_if(notes.begin(), notes.end(), [id](Note note) {
                    return note.getId() == id;
                }),
                notes.end());
}

void InMemoryNoteRepository::update(int id, const DraftNote &draftNote) {
    auto idIterator = std::find_if(notes.begin(), notes.end(), [id](Note note) {
        return note.getId() == id;
    });
    if (idIterator != notes.end()) {
        int index = std::distance(notes.begin(), idIterator);
        notes[index] = Note(id, draftNote.getTitle(), draftNote.getDescription());
    }
}

std::vector<Note> InMemoryNoteRepository::getAll() {
    return notes;
}