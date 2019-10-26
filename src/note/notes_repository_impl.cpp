#include "core/include_macros.hpp"
#include "notes_repository_impl.hpp"
#include AMALGAMATION(database_cursor.hpp)

NotesRepositoryImpl::NotesRepositoryImpl(std::shared_ptr<Db::Database> db, std::shared_ptr<Time::Clock> clock)
    : db(std::move(db)), clock(std::move(clock)) {}

void NotesRepositoryImpl::insert(Draft draftNote) {
    auto movedDraftNote = std::move(draftNote);
    auto stmt = db->createStatement("INSERT INTO notes (title, description, last_update_date) "
                                    "VALUES (?, ?, ?)");
    stmt->bind(1, movedDraftNote.getTitle());
    stmt->bind(2, movedDraftNote.getDescription());
    stmt->bind(3, Time::Format::format(clock->currentTimeSeconds()));
    stmt->execute<void>();
}

void NotesRepositoryImpl::deleteWithId(int id) {
    auto stmt = db->createStatement("DELETE FROM notes "
                                    "WHERE rowid = ?");
    stmt->bind(1, id);
    stmt->execute<void>();
}

void NotesRepositoryImpl::update(int id, Draft draftNote) {
    auto stmt = db->createStatement("UPDATE notes "
                                    "SET title = ?, description = ?, last_update_date = ? "
                                    "WHERE rowid = ?");
    stmt->bind(1, draftNote.getTitle());
    stmt->bind(2, draftNote.getDescription());
    stmt->bind(3, Time::Format::format(clock->currentTimeSeconds()));
    stmt->bind(4, id);
    stmt->execute<void>();
}

void NotesRepositoryImpl::deleteAll() {
    auto stmt = db->createStatement("DELETE FROM notes");
    stmt->execute<void>();
}

std::vector<Note> NotesRepositoryImpl::getAll() {
    std::vector<Note> notes;
    auto stmt = db->createStatement("SELECT rowid, title, description, last_update_date FROM notes");
    auto cursor = stmt->execute<std::shared_ptr<Db::Cursor>>();
    while (cursor->next()) {
        auto id = cursor->get<int>(0);
        auto title = cursor->get<std::string>(1);
        auto description = cursor->get<std::string>(2);
        auto lastUpdateDate = cursor->get<std::string>(3);
        notes.emplace_back(id, title, description,lastUpdateDate);
    }
    return notes;
} // LCOV_EXCL_BR_LINE

std::vector<Note> NotesRepositoryImpl::getByText(std::string text) {
    std::vector<Note> notes;
    auto stmt = db->createStatement(
        "SELECT rowid, title, description, last_update_date "
        "FROM notes "
        "WHERE title LIKE ?"
        "OR description LIKE ?"
    );
    // In this way all the notes which contain the given text in the title or the description will be retrieved.
    auto likeText = "%" + text + "%";
    stmt->bind(1, likeText);
    stmt->bind(2, likeText);
    auto cursor = stmt->execute<std::shared_ptr<Db::Cursor>>();
    while (cursor->next()) {
        auto id = cursor->get<int>(0);
        auto title = cursor->get<std::string>(1);
        auto description = cursor->get<std::string>(2);
        auto lastUpdateDate = cursor->get<std::string>(3);
        notes.emplace_back(id, title, description, lastUpdateDate);
    }
    return notes;
} // LCOV_EXCL_BR_LINE
