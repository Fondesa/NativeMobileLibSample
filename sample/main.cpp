#include <iostream>
#include "database_factory.hpp"
#include "database/sqlite_cursor.hpp"
#include "database/sqlite_database.hpp"
#include "database_note_repository.hpp"

void printNotes(std::vector<Note> notes);

int main() {
//    std::shared_ptr<Database> db = std::make_shared<SQLiteDatabase>("notes.db");
    std::shared_ptr<Database> db = DatabaseFactory::createDatabase(":memory:");
    auto createTableStmt = db->createStatement("CREATE TABLE IF NOT EXISTS notes ("
                                               "title TEXT NOT NULL, "
                                               "description TEXT NOT NULL"
                                               ")");
    createTableStmt->execute<void>();

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