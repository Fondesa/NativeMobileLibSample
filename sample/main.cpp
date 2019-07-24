#include <iostream>
#include "sqlite_cursor.hpp"
#include "sqlite_database.hpp"
#include "sqlite_note_repository.hpp"

void printNotes(std::vector<Note> notes);

int main() {
//    auto db = std::make_unique<SQLiteDatabase>("notes.db");
    auto db = std::make_unique<SQLiteDatabase>(":memory:");
    auto createTableStmt = db->createStatement("CREATE TABLE IF NOT EXISTS notes ("
                                               "title TEXT NOT NULL, "
                                               "description TEXT NOT NULL"
                                               ")");
    createTableStmt.execute<void>();

    auto repository = std::make_unique<SQLiteNoteRepository>(*db);

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

void printNotes(std::vector<Note> notes) {
    std::cout << "There are the following notes:\n"
              << std::endl;
    for (auto it = notes.begin(); it != notes.end(); ++it) {
        auto note = *it;
        std::cout << "-\tId: "
                  << note.getId()
                  << std::endl
                  << "\tTitle: "
                  << note.getTitle()
                  << std::endl
                  << "\tDescription: "
                  << note.getDescription()
                  << std::endl;
        if (it != notes.end() - 1) {
            std::cout << std::endl;
        }
    }
}