#pragma once

#include <optional>
#include <map>
#include "draft_notes_repository.hpp"
#include "database.hpp"

class DraftNotesRepositoryImpl : public DraftNotesRepository {
   public:
    explicit DraftNotesRepositoryImpl(std::shared_ptr<Db::Database> db);

    void updateNewDraftTitle(std::string title) override;

    void updateNewDraftDescription(std::string description) override;

    void updateExistingDraftTitle(int id, std::string title) override;

    void updateExistingDraftDescription(int id, std::string description) override;

    void clearDrafts() override;

    void removeDraftCreation() override;

    void removeDraftUpdate(int id) override;

    void persistDraftNotes() override;

    std::optional<DraftNote> getDraftCreationNote() override;

    std::optional<DraftNote> getDraftUpdateNote(int id) override;

   private:
    std::shared_ptr<Db::Database> db;
    std::optional<DraftNote> pendingNewDraft;
    std::map<int, DraftNote> pendingExistingDrafts;

    void persistNewDraftNote(const DraftNote &note);

    void persistExistingDraftNotes(const std::map<int, DraftNote> &notes);
};