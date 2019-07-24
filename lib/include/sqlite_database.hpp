#pragma once

#include <string>
#include "sqlite3.h"
#include "sqlite_statement.hpp"

class SQLiteDatabase {
   public:
    explicit SQLiteDatabase(std::string dbPath);

    ~SQLiteDatabase();

    void executeTransaction(std::function<void()> transact) const;

    SQLiteStatement createStatement(std::string sql) const;

   private:
    sqlite3 *db{};
};