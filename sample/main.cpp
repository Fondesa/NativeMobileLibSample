#include <iostream>
#include "database_client.hpp"
#include "note_database_initializer.hpp"
#include "database/sqlite_cursor.hpp"
#include "database/sqlite_database.hpp"
#include "database_note_repository.hpp"

void printNotes(const std::vector<Note> &notes);

int main() {
    NoteDb::initialize(":memory:");
//    NoteDb::initialize("notes.db");

    auto db = Db::Client::get();
    auto repository = std::make_shared<DatabaseNoteRepository>(db);

    auto first = DraftNote("First title", "First description");
    repository->insert(first);
    auto second = DraftNote("Second title", "Second description");
    repository->insert(second);

    auto notes = repository->getAll();
    printNotes(notes);

    repository->remove(notes[0].getId());

    auto notesAfterRemove = repository->getAll();
    printNotes(notesAfterRemove);

    repository->update(notesAfterRemove[0].getId(), DraftNote("Updated title", "Updated description"));

    auto notesAfterUpdate = repository->getAll();
    printNotes(notesAfterUpdate);

    return 0;
}

void printNotes(const std::vector<Note> &notes) {
    std::cout << "There are the following notes:"
              << std::endl;
    for (const auto &note : notes) {
        std::cout << "-\tId: "
                  << note.getId()
                  << std::endl
                  << "\tTitle: "
                  << note.getTitle()
                  << std::endl
                  << "\tDescription: "
                  << note.getDescription()
                  << std::endl;
    }

    std::cout << std::endl;
}