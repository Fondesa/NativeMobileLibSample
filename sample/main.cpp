#include <iostream>
#include "database_client.hpp"
#include "note_database_initializer.hpp"
#include "database/sqlite_cursor.hpp"
#include "database/sqlite_database.hpp"
#include "note/notes_repository_impl.hpp"
#include "note/drafts_repository_impl.hpp"

void printNotes(const std::vector<Note> &notes);

int main() {
//    NoteDb::initialize(":memory:");
    NoteDb::initialize("notes.db");

    auto db = Db::Client::get();
    auto repository = std::make_shared<NotesRepositoryImpl>(db);

    auto first = Draft("First title", "First description");
    repository->insert(first);
    auto second = Draft("Second title", "Second description");
    repository->insert(second);

    auto notes = repository->getAll();
    printNotes(notes);

    repository->deleteWithId(notes[0].getId());

    auto notesAfterRemove = repository->getAll();
    printNotes(notesAfterRemove);

    repository->update(notesAfterRemove[0].getId(), Draft("Updated title", "Updated description"));

    auto notesAfterUpdate = repository->getAll();
    printNotes(notesAfterUpdate);

    auto draftRepository = std::make_shared<DraftsRepositoryImpl>(db);
    draftRepository->updateNewTitle("draft-create-title");
    draftRepository->updateNewDescription("draft-create-description");

    draftRepository->updateNewTitle("draft-update-title");
    draftRepository->updateNewDescription("draft-update-description");
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