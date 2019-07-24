#pragma once

#include "database.hpp"
#include "database_statement.hpp"
#include "sqlite3/sqlite3.h"

class SQLiteDatabase : public Database {
   public:
    explicit SQLiteDatabase(std::string dbPath);

    ~SQLiteDatabase();

    void executeTransaction(std::function<void()> transact) const override;

    std::shared_ptr<DatabaseStatement> createStatement(std::string sql) const override;

   private:
    sqlite3 *db{};
};