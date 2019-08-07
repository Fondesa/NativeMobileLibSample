#pragma once

#include <optional>
#include <map>
#include "drafts_repository.hpp"
#include "database.hpp"

class DraftsRepositoryImpl : public DraftsRepository {
   public:
    explicit DraftsRepositoryImpl(std::shared_ptr<Db::Database> db);

    std::optional<Draft> getNew() override;

    std::optional<Draft> getExisting(int id) override;

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
    std::optional<Draft> pendingNew;
    std::map<int, Draft> pendingExisting;

    void persistNew(const Draft &note);

    void persistExisting(const std::map<int, Draft> &notes);
};