#include "sqlite_note_repository.hpp"
#include "sqlite_cursor.hpp"

SQLiteNoteRepository::SQLiteNoteRepository(const SQLiteDatabase &db) : db(db) {}

void SQLiteNoteRepository::insert(DraftNote draftNote) {
    auto movedDraftNote = std::move(draftNote);
    auto stmt = db.createStatement("INSERT INTO notes (title, description) "
                                   "VALUES (?, ?)");
    stmt.bind(1, movedDraftNote.getTitle());
    stmt.bind(2, movedDraftNote.getDescription());
    stmt.execute<void>();

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
    std::vector<Note> notes;
    auto selectStmt = db.createStatement("SELECT rowid, title, description FROM notes");
    auto cursor = selectStmt.execute<SQLiteCursor>();
    while (cursor.next()) {
        auto id = cursor.get<int>(0);
        auto title = cursor.get<std::string>(1);
        auto description = cursor.get<std::string>(2);
        notes.emplace_back(id, title, description);
    }
    return notes;
}