#include "drafts_repository_impl.hpp"
#include "invalid_draft_session_exception.hpp"

DraftsRepositoryImpl::DraftsRepositoryImpl(std::shared_ptr<Db::Database> db) :
    db(std::move(db)) {}

void DraftsRepositoryImpl::updateNewDraftTitle(std::string title) {
    pendingNewDraft->setTitle(title);
}

void DraftsRepositoryImpl::updateNewDraftDescription(std::string description) {
    pendingNewDraft->setDescription(description);
}

void DraftsRepositoryImpl::updateExistingDraftTitle(int id, std::string title) {
    auto existingEntry = pendingExistingDrafts.find(id);
    if (existingEntry != pendingExistingDrafts.end()) {
        auto note = existingEntry->second;
        note.setTitle(title);
    }
}

void DraftsRepositoryImpl::updateExistingDraftDescription(int id, std::string description) {
    auto existingEntry = pendingExistingDrafts.find(id);
    if (existingEntry != pendingExistingDrafts.end()) {
        auto note = existingEntry->second;
        note.setDescription(description);
    }
}

void DraftsRepositoryImpl::clear() {
    db->executeTransaction([this]() {
        deleteNewDraft();
        db->createStatement("DELETE FROM pending_draft_notes_update")->execute<void>();
    });
}

void DraftsRepositoryImpl::deleteNewDraft() {
    db->createStatement("DELETE FROM pending_draft_note_creation")->execute<void>();
}

void DraftsRepositoryImpl::deleteExistingDraft(int id) {
    auto stmt = db->createStatement("DELETE FROM pending_draft_notes_update WHERE rowid = ?");
    stmt->bind(1, id);
    stmt->execute<void>();
}

std::optional<Draft> DraftsRepositoryImpl::getDraftCreationNote() {
    if (pendingNewDraft) {
        return pendingNewDraft;
    }
    auto stmt = db->createStatement("SELECT title, description "
                                    "FROM pending_draft_note_creation "
                                    "LIMIT 1");
    auto draftNote = std::optional<Draft>();
    auto cursor = stmt->execute<std::shared_ptr<Db::Cursor>>();
    while (cursor->next()) {
        auto title = cursor->get<std::string>(0);
        auto description = cursor->get<std::string>(1);
        draftNote = Draft(title, description);
    }
    return draftNote;
}

std::optional<Draft> DraftsRepositoryImpl::getDraftUpdateNote(int id) {
    auto existingEntry = pendingExistingDrafts.find(id);
    if (existingEntry != pendingExistingDrafts.end()) {
        return existingEntry->second;
    }

    auto stmt = db->createStatement("SELECT title, description "
                                    "FROM pending_draft_notes_update "
                                    "WHERE rowid = ?"
                                    "LIMIT 1");
    stmt->bind(1, id);

    auto draftNote = std::optional<Draft>();
    auto cursor = stmt->execute<std::shared_ptr<Db::Cursor>>();
    while (cursor->next()) {
        auto title = cursor->get<std::string>(0);
        auto description = cursor->get<std::string>(1);
        draftNote = Draft(title, description);
    }
    return draftNote;
}

void DraftsRepositoryImpl::persist() {
    auto dbTransaction = [&]() {
        if (pendingNewDraft) {
            // Persist in DB the new draft note.
            persistNewDraftNote(pendingNewDraft.value());
            // The new draft note is not needed anymore in memory.
            pendingNewDraft.reset();
        }
        if (!pendingExistingDrafts.empty()) {
            // Persist in DB the draft notes which should be updated.
            persistExistingDraftNotes(pendingExistingDrafts);
            // The old draft notes are not needed anymore in memory.
            // TODO: make it thread safe
            pendingExistingDrafts.clear();
        }
    };
    db->executeTransaction(dbTransaction);
}

void DraftsRepositoryImpl::persistNewDraftNote(const Draft &note) {
    auto title = note.getTitle();
    auto description = note.getDescription();
    if (title.empty() && description.empty()) {
        // The draft shouldn't be persisted.
        return;
    }

    auto stmt = db->createStatement("INSERT INTO pending_draft_note_creation (id, title, description) "
                                    "VALUES (0, ?, ?) "
                                    "ON CONFLICT(id) "
                                    "DO UPDATE SET title = ?, description = ?");

    stmt->bind(1, title);
    stmt->bind(2, description);
    stmt->bind(3, title);
    stmt->bind(4, description);
    stmt->execute<void>();
}

void DraftsRepositoryImpl::persistExistingDraftNotes(const std::map<int, Draft> &notes) {
    auto stmt = db->createStatement("INSERT INTO pending_draft_notes_update (rowid, title, description) "
                                    "VALUES (?, ?, ?) "
                                    "ON CONFLICT(rowid) "
                                    "DO UPDATE SET title = ?, description = ?");

    for (auto const&[id, note] : notes) {
        stmt->bind(1, id);

        auto title = note.getTitle();
        auto description = note.getDescription();
        stmt->bind(2, title);
        stmt->bind(3, description);
        stmt->bind(4, title);
        stmt->bind(5, description);

        stmt->execute<void>();
    }
}
