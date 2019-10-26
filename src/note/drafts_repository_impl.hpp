#pragma once

#include <map>
#include "drafts_repository.hpp"
#include "core/include_macros.hpp"
#include "mutable_draft.hpp"
#include AMALGAMATION(database.hpp)

class DraftsRepositoryImpl : public DraftsRepository {
   public:
    explicit DraftsRepositoryImpl(std::shared_ptr<Db::Database> db);

    stdx::optional<Draft> getNew() override;

    stdx::optional<Draft> getExisting(int id) override;

    void updateNewTitle(std::string title) override;

    void updateNewDescription(std::string description) override;

    void updateExistingTitle(int id, std::string title) override;

    void updateExistingDescription(int id, std::string description) override;

    void deleteNew() override;

    void deleteExisting(int id) override;

    void deleteAll() override;

    void persist() override;

   private:
    std::shared_ptr<Db::Database> db;
    stdx::optional<MutableDraft> pendingNew;
    std::map<int, MutableDraft> pendingExisting;

    void persistNew(const MutableDraft &draft);

    void persistExisting(const std::map<int, MutableDraft> &drafts);

    stdx::optional<Draft> getNewFromDb();

    stdx::optional<Draft> getExistingFromDb(int id);

    stdx::optional<std::string> getNewTitleFromDb();

    stdx::optional<std::string> getNewDescriptionFromDb();

    stdx::optional<std::string> getExistingTitleFromDb(int id);

    stdx::optional<std::string> getExistingDescriptionFromDb(int id);
};