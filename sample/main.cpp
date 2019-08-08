#include <iostream>
#include "note_database_initializer.hpp"
#include "notes_interactor_factory.hpp"

void printNotes(const std::vector<Note> &notes);

int main() {
//    NoteDb::initialize(":memory:");
    NoteDb::initialize("notes.db");

    auto interactor = NotesInteractorFactory::create();

    auto first = Draft("First title", "First description");
    interactor->insertNote(first);
    auto second = Draft("Second title", "Second description");
    interactor->insertNote(second);

    auto notes = interactor->getAllNotes();
    printNotes(notes);

    interactor->deleteNote(notes[0].getId());

    auto notesAfterRemove = interactor->getAllNotes();
    printNotes(notesAfterRemove);

    interactor->updateNote(notesAfterRemove[0].getId(), Draft("Updated title", "Updated description"));

    auto notesAfterUpdate = interactor->getAllNotes();
    printNotes(notesAfterUpdate);

    interactor->updateNewDraftTitle("new-draft-title");
    interactor->updateNewDraftDescription("new-draft-description");

    interactor->updateExistingDraftTitle(notesAfterUpdate[0].getId(), "existing-draft-title");
    interactor->updateExistingDraftDescription(notesAfterUpdate[0].getId(), "existing-draft-description");

    interactor->persistChanges();

    interactor->updateExistingDraftTitle(notesAfterUpdate[0].getId(), "existing-draft-title2");

    interactor->persistChanges();

    return 0;
}

void printNotes(const std::vector<Note> &notes) {
    std::cout << "There are the following notes:"
              << std::endl;
    for (const auto &note : notes) {
        std::cout << "-\tId: "
                  << note.getId()
                  << ", "
                  << "Title: "
                  << note.getTitle()
                  << ", "
                  << "Description: "
                  << note.getDescription()
                  << std::endl;
    }

    std::cout << std::endl;
}