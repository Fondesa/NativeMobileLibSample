#include "in_mem_to_db_draft_notes_repository.hpp"
#include "invalid_draft_session_exception.hpp"

InMemToDbDraftNotesRepository::InMemToDbDraftNotesRepository(std::shared_ptr<Db::Database> db) :
    db(std::move(db)),
    _title(""),
    _description(""),
    draftTitle(_title),
    draftDescription(_description),
    isInSession(false) {}

void InMemToDbDraftNotesRepository::beginCreationSession() {
    if (isInSession) {
        // Close the previous session.
        endSession();
    }
    draftTitle = "";
    draftDescription = "";
    noteInUpdate.reset();
    isInSession = true;
}

void InMemToDbDraftNotesRepository::beginUpdateSession(Note note) {
    if (isInSession) {
        // Close the previous session.
        endSession();
    }
    draftTitle = note.getTitle();
    draftDescription = note.getDescription();
    noteInUpdate = note;
    isInSession = true;
}

void InMemToDbDraftNotesRepository::endSession() {
    if (!isInSession) {
        // If there isn't an active session, nothing should be done.
        return;
    }
    if (noteInUpdate) {
        // The note in draft already existed.
        updateNoteInDb();
    } else {
        // The note in draft is a new one.
        createNoteInDb();
    }
}

void InMemToDbDraftNotesRepository::updateTitle(std::string title) {
    draftTitle = title;
}

void InMemToDbDraftNotesRepository::updateDescription(std::string description) {
    draftDescription = description;
}

void InMemToDbDraftNotesRepository::clearDrafts() {
    db->executeTransaction([this]() {
        removeDraftCreation();
        db->createStatement("DELETE FROM pending_draft_notes_update")->execute<void>();
    });
}

void InMemToDbDraftNotesRepository::removeDraftCreation() {
    db->createStatement("DELETE FROM pending_draft_note_creation")->execute<void>();
}

void InMemToDbDraftNotesRepository::removeDraftUpdate(int id) {
    auto stmt = db->createStatement("DELETE FROM pending_draft_notes_update WHERE rowid = ?");
    stmt->bind(1, id);
    stmt->execute<void>();
}

void InMemToDbDraftNotesRepository::updateNoteInDb() {
    bool hasSameTitle = draftTitle == noteInUpdate->getTitle();
    bool hasSameDescription = draftDescription == noteInUpdate->getDescription();
    if (hasSameTitle && hasSameDescription) {
        // The draft shouldn't be persisted.
        return;
    }

    auto stmt = db->createStatement("INSERT INTO pending_draft_notes_update (rowid, title, description) "
                                    "VALUES (?, ?, ?) "
                                    "ON CONFLICT(rowid) "
                                    "DO UPDATE SET title = ?, description = ?");

    stmt->bind(1, noteInUpdate->getId());
    stmt->bind(2, draftTitle);
    stmt->bind(3, draftDescription);
    stmt->bind(4, draftTitle);
    stmt->bind(5, draftDescription);
    stmt->execute<void>();
}

void InMemToDbDraftNotesRepository::createNoteInDb() {
    if (draftTitle.empty() && draftDescription.empty()) {
        // The draft shouldn't be persisted.
        return;
    }

    auto stmt = db->createStatement("INSERT INTO pending_draft_note_creation (id, title, description) "
                                    "VALUES (0, ?, ?) "
                                    "ON CONFLICT(id) "
                                    "DO UPDATE SET title = ?, description = ?");

    stmt->bind(1, draftTitle);
    stmt->bind(2, draftDescription);
    stmt->bind(3, draftTitle);
    stmt->bind(4, draftDescription);
    stmt->execute<void>();
}

std::optional<DraftNote> InMemToDbDraftNotesRepository::getDraftCreationNote() {
    auto stmt = db->createStatement("SELECT title, description "
                                    "FROM pending_draft_note_creation "
                                    "LIMIT 1");
    auto draftNote = std::optional<DraftNote>();
    auto cursor = stmt->execute<std::shared_ptr<Db::Cursor>>();
    while (cursor->next()) {
        auto title = cursor->get<std::string>(0);
        auto description = cursor->get<std::string>(1);
        draftNote = DraftNote(title, description);
    }
    return draftNote;
}

std::optional<DraftNote> InMemToDbDraftNotesRepository::getDraftUpdateNote(int id) {
    auto stmt = db->createStatement("SELECT title, description "
                                    "FROM pending_draft_notes_update "
                                    "WHERE rowid = ?"
                                    "LIMIT 1");
    stmt->bind(1, id);

    auto draftNote = std::optional<DraftNote>();
    auto cursor = stmt->execute<std::shared_ptr<Db::Cursor>>();
    while (cursor->next()) {
        auto title = cursor->get<std::string>(0);
        auto description = cursor->get<std::string>(1);
        draftNote = DraftNote(title, description);
    }
    return draftNote;
}
