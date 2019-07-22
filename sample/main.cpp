#include <iostream>
#include "sqlite_database.hpp"
#include "sqlite_note_repository.hpp"

void printNotes(std::vector<Note> notes);

int main() {
    auto db = std::make_unique<SQLiteDatabase>("notes.db");
    auto createTableStmt = db->createStatement("CREATE TABLE IF NOT EXISTS notes ("
                                               "TITLE TEXT NOT NULL, "
                                               "DESCRIPTION TEXT NOT NULL"
                                               ")");
    createTableStmt.execute<void>();

    auto insertStmt = db->createStatement("INSERT INTO notes (TITLE,DESCRIPTION) "
                        "VALUES ('dummy-title', 'dummy-description')");
    insertStmt.execute<void>();

    auto repository = std::make_unique<SQLiteNoteRepository>(*db);

    auto first = DraftNote("First title", "First description");
    repository->insert(first);
    auto second = DraftNote("Second title", "Second description");
    repository->insert(second);

    printNotes(repository->getAll());

    repository->remove(0);

    printNotes(repository->getAll());

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