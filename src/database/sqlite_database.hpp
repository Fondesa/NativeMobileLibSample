#pragma once

#include "database.hpp"
#include "database_statement.hpp"
#include "sqlite3/sqlite3.h"

namespace Db::Sql {

class Database : public Db::Database {
   public:
    explicit Database(std::string dbPath);

    ~Database();

    void executeTransaction(std::function<void()> transact) const override;

    [[nodiscard]] std::shared_ptr<Db::Statement> createStatement(std::string sql) const override;

   private:
    sqlite3 *db{};
};
}