#include "drafts_repository_impl.hpp"
#include "invalid_draft_session_exception.hpp"

DraftsRepositoryImpl::DraftsRepositoryImpl(std::shared_ptr<Db::Database> db) :
    db(std::move(db)) {}

void DraftsRepositoryImpl::updateNewTitle(std::string title) {
    pendingNew->setTitle(title);
}

void DraftsRepositoryImpl::updateNewDescription(std::string description) {
    pendingNew->setDescription(description);
}

void DraftsRepositoryImpl::updateExistingTitle(int id, std::string title) {
    auto existingEntry = pendingExisting.find(id);
    if (existingEntry != pendingExisting.end()) {
        auto note = existingEntry->second;
        note.setTitle(title);
    }
}

void DraftsRepositoryImpl::updateExistingDescription(int id, std::string description) {
    auto existingEntry = pendingExisting.find(id);
    if (existingEntry != pendingExisting.end()) {
        auto note = existingEntry->second;
        note.setDescription(description);
    }
}

void DraftsRepositoryImpl::clear() {
    db->executeTransaction([this]() {
        deleteNew();
        db->createStatement("DELETE FROM pending_draft_notes_update")->execute<void>();
    });
}

void DraftsRepositoryImpl::deleteNew() {
    db->createStatement("DELETE FROM pending_draft_note_creation")->execute<void>();
}

void DraftsRepositoryImpl::deleteExisting(int id) {
    auto stmt = db->createStatement("DELETE FROM pending_draft_notes_update WHERE rowid = ?");
    stmt->bind(1, id);
    stmt->execute<void>();
}

std::optional<Draft> DraftsRepositoryImpl::getNew() {
    if (pendingNew) {
        return pendingNew;
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

std::optional<Draft> DraftsRepositoryImpl::getExisting(int id) {
    auto existingEntry = pendingExisting.find(id);
    if (existingEntry != pendingExisting.end()) {
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
        if (pendingNew) {
            // Persist in DB the new draft note.
            persistNew(pendingNew.value());
            // The new draft note is not needed anymore in memory.
            pendingNew.reset();
        }
        if (!pendingExisting.empty()) {
            // Persist in DB the draft notes which should be updated.
            persistExisting(pendingExisting);
            // The old draft notes are not needed anymore in memory.
            // TODO: make it thread safe
            pendingExisting.clear();
        }
    };
    db->executeTransaction(dbTransaction);
}

void DraftsRepositoryImpl::persistNew(const Draft &note) {
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

void DraftsRepositoryImpl::persistExisting(const std::map<int, Draft> &notes) {
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
