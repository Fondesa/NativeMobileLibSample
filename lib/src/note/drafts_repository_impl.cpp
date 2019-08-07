#include "drafts_repository_impl.hpp"
#include "invalid_draft_session_exception.hpp"

DraftsRepositoryImpl::DraftsRepositoryImpl(std::shared_ptr<Db::Database> db) :
    db(std::move(db)),
    pendingNew(Draft("", "")) {}

void DraftsRepositoryImpl::updateNewTitle(std::string title) {
    // TODO read from DB
    if (!pendingNew) {
        pendingNew = Draft(title, "");
    }
    pendingNew->setTitle(title);
}

void DraftsRepositoryImpl::updateNewDescription(std::string description) {
    // TODO read from DB
    if (!pendingNew) {
        pendingNew = Draft("", description);
    }
    pendingNew->setDescription(description);
}

void DraftsRepositoryImpl::updateExistingTitle(int id, std::string title) {
    // TODO read from DB

    auto existingEntry = pendingExisting.find(id);
    if (existingEntry != pendingExisting.end()) {
        auto draft = existingEntry->second;
        draft.setTitle(title);
    }
}

void DraftsRepositoryImpl::updateExistingDescription(int id, std::string description) {
    // TODO read from DB

    auto existingEntry = pendingExisting.find(id);
    if (existingEntry != pendingExisting.end()) {
        auto draft = existingEntry->second;
        draft.setDescription(description);
    }
}

void DraftsRepositoryImpl::deleteAll() {
    db->executeTransaction([this]() {
        deleteNew();
        db->createStatement("DELETE FROM pending_drafts_update")->execute<void>();
    });
}

void DraftsRepositoryImpl::deleteNew() {
    db->createStatement("DELETE FROM pending_draft_creation")->execute<void>();
}

void DraftsRepositoryImpl::deleteExisting(int id) {
    auto stmt = db->createStatement("DELETE FROM pending_drafts_update WHERE rowid = ?");
    stmt->bind(1, id);
    stmt->execute<void>();
}

std::optional<Draft> DraftsRepositoryImpl::getNew() {
    if (pendingNew) {
        return pendingNew;
    }
    auto stmt = db->createStatement("SELECT title, description "
                                    "FROM pending_draft_creation "
                                    "LIMIT 1");
    auto draft = std::optional<Draft>();
    auto cursor = stmt->execute<std::shared_ptr<Db::Cursor>>();
    while (cursor->next()) {
        auto title = cursor->get<std::string>(0);
        auto description = cursor->get<std::string>(1);
        draft = Draft(title, description);
    }
    return draft;
}

std::optional<Draft> DraftsRepositoryImpl::getExisting(int id) {
    auto existingEntry = pendingExisting.find(id);
    if (existingEntry != pendingExisting.end()) {
        return existingEntry->second;
    }

    auto stmt = db->createStatement("SELECT title, description "
                                    "FROM pending_drafts_update "
                                    "WHERE rowid = ?"
                                    "LIMIT 1");
    stmt->bind(1, id);

    auto draft = std::optional<Draft>();
    auto cursor = stmt->execute<std::shared_ptr<Db::Cursor>>();
    while (cursor->next()) {
        auto title = cursor->get<std::string>(0);
        auto description = cursor->get<std::string>(1);
        draft = Draft(title, description);
    }
    return draft;
}

void DraftsRepositoryImpl::persist() {
    auto tempPendingNew = pendingNew;
    auto tempPendingExisting = pendingExisting;

    // Reset the in-memory storage since they will be persisted.
    pendingNew.reset();
    pendingExisting.clear();

    auto dbTransaction = [&]() {
        if (tempPendingNew) {
            // Persist in DB the new draft note.
            persistNew(tempPendingNew.value());
        }
        if (!tempPendingExisting.empty()) {
            // Persist in DB the draft notes which should be updated.
            persistExisting(tempPendingExisting);
        }
    };
    db->executeTransaction(dbTransaction);
}

void DraftsRepositoryImpl::persistNew(const Draft &draft) {
    auto title = draft.getTitle();
    auto description = draft.getDescription();
    if (title.empty() && description.empty()) {
        // The draft shouldn't be persisted.
        // To reach this state, the user updated the title and/or the description and then he emptied them.
        return;
    }
    auto stmt = db->createStatement("INSERT INTO pending_draft_creation (id, title, description) "
                                    "VALUES (0, ?, ?) "
                                    "ON CONFLICT(id) "
                                    "DO UPDATE SET title = ?, description = ?");

    stmt->bind(1, title);
    stmt->bind(2, description);
    stmt->bind(3, title);
    stmt->bind(4, description);
    stmt->execute<void>();
}

void DraftsRepositoryImpl::persistExisting(const std::map<int, Draft> &drafts) {
    auto stmt = db->createStatement("INSERT INTO pending_drafts_update (rowid, title, description) "
                                    "VALUES (?, ?, ?) "
                                    "ON CONFLICT(rowid) "
                                    "DO UPDATE SET title = ?, description = ?");

    for (auto const&[id, draft] : drafts) {
        stmt->bind(1, id);

        auto title = draft.getTitle();
        auto description = draft.getDescription();
        stmt->bind(2, title);
        stmt->bind(3, description);
        stmt->bind(4, title);
        stmt->bind(5, description);

        stmt->execute<void>();
    }
}
