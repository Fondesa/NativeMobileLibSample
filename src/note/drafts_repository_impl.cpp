#include "drafts_repository_impl.hpp"
#include "incomplete_draft_exception.hpp"

DraftsRepositoryImpl::DraftsRepositoryImpl(std::shared_ptr<Db::Database> db) :
    db(std::move(db)) {}

void DraftsRepositoryImpl::updateNewTitle(std::string title) {
    if (!pendingNew) {
        pendingNew = MutableDraft();
        std::string description;
        // Read the title from the DB, if any, or put it empty.
        auto descriptionResult = getNewDescriptionFromDb();
        if (descriptionResult) {
            description = *descriptionResult;
        }
        pendingNew->updateDescription(description);
    }
    pendingNew->updateTitle(title);
}

void DraftsRepositoryImpl::updateNewDescription(std::string description) {
    if (!pendingNew) {
        pendingNew = MutableDraft();
        std::string title;
        // Read the title from the DB, if any, or put it empty.
        auto titleResult = getNewTitleFromDb();
        if (titleResult) {
            title = *titleResult;
        }
        pendingNew->updateTitle(title);
    }
    pendingNew->updateDescription(description);
}

void DraftsRepositoryImpl::updateExistingTitle(int id, std::string title) {
    auto existingEntry = pendingExisting.find(id);
    if (existingEntry != pendingExisting.end()) {
        // The draft exists in memory.
        auto &draft = existingEntry->second;
        draft.updateTitle(title);
    } else {
        auto draft = MutableDraft();
        // Read the description from the DB, if any.
        auto descriptionResult = getExistingDescriptionFromDb(id);
        if (descriptionResult) {
            draft.updateDescription(*descriptionResult);
        }
        draft.updateTitle(title);
        pendingExisting[id] = draft;
    }
}

void DraftsRepositoryImpl::updateExistingDescription(int id, std::string description) {
    auto existingEntry = pendingExisting.find(id);
    if (existingEntry != pendingExisting.end()) {
        // The draft exists in memory.
        auto &draft = existingEntry->second;
        draft.updateDescription(description);
    } else {
        auto draft = MutableDraft();
        // Read the title from the DB, if any.
        auto titleResult = getExistingTitleFromDb(id);
        if (titleResult) {
            draft.updateTitle(*titleResult);
        }
        draft.updateDescription(description);
        pendingExisting[id] = draft;
    }
}

void DraftsRepositoryImpl::deleteAll() {
    db->executeTransaction([this]() {
        deleteNew();
        // Reset the existing drafts in memory.
        pendingExisting.clear();
        // Delete all the existing drafts from the DB.
        db->createStatement("DELETE FROM pending_drafts_update")->execute<void>();
    });
}

void DraftsRepositoryImpl::deleteNew() {
    // Remove it from in-memory storage.
    pendingNew.reset();
    // Remove it from database.
    db->createStatement("DELETE FROM pending_draft_creation")->execute<void>();
}

void DraftsRepositoryImpl::deleteExisting(int id) {
    auto existingEntry = pendingExisting.find(id);
    if (existingEntry != pendingExisting.end()) {
        // Remote it from in-memory storage.
        pendingExisting.erase(existingEntry);
    }
    // Remove it from database.
    auto stmt = db->createStatement("DELETE FROM pending_drafts_update WHERE rowid = ?");
    stmt->bind(1, id);
    stmt->execute<void>();
}

std::optional<Draft> DraftsRepositoryImpl::getNew() {
    if (pendingNew) {
        return pendingNew->toDraft();
    }
    return getNewFromDb();
}

std::optional<Draft> DraftsRepositoryImpl::getExisting(int id) {
    auto existingEntry = pendingExisting.find(id);
    if (existingEntry != pendingExisting.end()) {
        return existingEntry->second.toDraft();
    }
    return getExistingFromDb(id);
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
            persistNew(*tempPendingNew);
        }
        if (!tempPendingExisting.empty()) {
            // Persist in DB the draft notes which should be updated.
            persistExisting(tempPendingExisting);
        }
    };
    db->executeTransaction(dbTransaction);
}

void DraftsRepositoryImpl::persistNew(const MutableDraft &draft) {
    auto title = draft.requireTitle();
    auto description = draft.requireDescription();
    if (title.empty() && description.empty()) {
        // To reach this state, the user updated the title and/or the description and then he emptied them.
        // Therefore the draft should be deleted since an empty new draft is equally to a not initialized one.
        deleteNew();
        return;
    }

    auto stmt = db->createStatement(
        "INSERT INTO pending_draft_creation (id, title, description) "
        "VALUES (0, ?, ?) "
        "ON CONFLICT(id) "
        "DO UPDATE SET title = ?, description = ?"
    );

    stmt->bind(1, title);
    stmt->bind(2, description);
    stmt->bind(3, title);
    stmt->bind(4, description);
    stmt->execute<void>();
}

void DraftsRepositoryImpl::persistExisting(const std::map<int, MutableDraft> &drafts) {
    auto stmt = db->createStatement(
        "INSERT INTO pending_drafts_update (rowid, title, description) "
        "VALUES (?, ?, ?) "
        "ON CONFLICT(rowid) "
        "DO UPDATE SET title = ?, description = ?"
    );

    for (auto const&[id, draft] : drafts) {
        if (draft.isIncomplete()) {
            throw IncompleteDraftException(id, draft);
        }

        auto title = draft.requireTitle();
        auto description = draft.requireDescription();

        stmt->bind(1, id);
        stmt->bind(2, title);
        stmt->bind(3, description);
        stmt->bind(4, title);
        stmt->bind(5, description);

        stmt->execute<void>();
    }
}

std::optional<Draft> DraftsRepositoryImpl::getNewFromDb() {
    auto stmt = db->createStatement(
        "SELECT title, description "
        "FROM pending_draft_creation "
        "LIMIT 1"
    );
    auto draft = std::optional<Draft>();
    auto cursor = stmt->execute<std::shared_ptr<Db::Cursor>>();
    while (cursor->next()) {
        auto title = cursor->get<std::string>(0);
        auto description = cursor->get<std::string>(1);
        draft = Draft(title, description);
    }
    return draft;
}

std::optional<Draft> DraftsRepositoryImpl::getExistingFromDb(int id) {
    auto stmt = db->createStatement(
        "SELECT title, description "
        "FROM pending_drafts_update "
        "WHERE rowid = ? "
        "LIMIT 1"
    );
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

std::optional<std::string> DraftsRepositoryImpl::getNewTitleFromDb() {
    return db->createStatement(
        "SELECT title "
        "FROM pending_draft_creation "
        "LIMIT 1"
    )->execute<std::optional<std::string>>();
}

std::optional<std::string> DraftsRepositoryImpl::getNewDescriptionFromDb() {
    return db->createStatement(
        "SELECT description "
        "FROM pending_draft_creation "
        "LIMIT 1"
    )->execute<std::optional<std::string>>();
}

std::optional<std::string> DraftsRepositoryImpl::getExistingTitleFromDb(int id) {
    auto stmt = db->createStatement(
        "SELECT title "
        "FROM pending_drafts_update "
        "WHERE rowid = ? "
        "LIMIT 1"
    );
    stmt->bind(1, id);
    return stmt->execute<std::optional<std::string>>();
}

std::optional<std::string> DraftsRepositoryImpl::getExistingDescriptionFromDb(int id) {
    auto stmt = db->createStatement(
        "SELECT description "
        "FROM pending_drafts_update "
        "WHERE rowid = ? "
        "LIMIT 1"
    );
    stmt->bind(1, id);
    return stmt->execute<std::optional<std::string>>();
}
