#pragma once

#include <optional>
#include <map>
#include "drafts_repository.hpp"
#include "database.hpp"

class DraftsRepositoryImpl : public DraftsRepository {
   public:
    explicit DraftsRepositoryImpl(std::shared_ptr<Db::Database> db);

    void updateNewDraftTitle(std::string title) override;

    void updateNewDraftDescription(std::string description) override;

    void updateExistingDraftTitle(int id, std::string title) override;

    void updateExistingDraftDescription(int id, std::string description) override;

    void clear() override;

    void deleteNewDraft() override;

    void deleteExistingDraft(int id) override;

    void persist() override;

    std::optional<Draft> getDraftCreationNote() override;

    std::optional<Draft> getDraftUpdateNote(int id) override;

   private:
    std::shared_ptr<Db::Database> db;
    std::optional<Draft> pendingNewDraft;
    std::map<int, Draft> pendingExistingDrafts;

    void persistNewDraftNote(const Draft &note);

    void persistExistingDraftNotes(const std::map<int, Draft> &notes);
};