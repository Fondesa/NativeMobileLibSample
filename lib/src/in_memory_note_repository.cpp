#include "in_memory_note_repository.hpp"

void InMemoryNoteRepository::insert(const DraftNote &draftNote) {
    int noteId = currentId++;
    notes.push_back(std::make_shared<Note>(noteId, draftNote.getTitle(), draftNote.getDescription()));
}

void InMemoryNoteRepository::remove(int id) {
    notes.erase(std::remove_if(notes.begin(), notes.end(), [id](std::shared_ptr<Note> note) {
                    return note->getId() == id;
                }),
                notes.end());
}

void InMemoryNoteRepository::update(int id, const DraftNote &draftNote) {
    auto idIterator = std::find_if(notes.begin(), notes.end(), [id](std::shared_ptr<Note> note) {
        return note->getId() == id;
    });
    if (idIterator != notes.end()) {
        int index = std::distance(notes.begin(), idIterator);
        notes[index] = std::make_shared<Note>(id, draftNote.getTitle(), draftNote.getDescription());
    }
}

std::vector<std::shared_ptr<Note>> InMemoryNoteRepository::getAll() {
    return notes;
}