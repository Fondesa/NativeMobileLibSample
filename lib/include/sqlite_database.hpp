#pragma once

#include <string>
#include "sqlite3.h"
#include "sqlite_statement.hpp"

class SQLiteDatabase {
   public:
    explicit SQLiteDatabase(std::string dbPath);

    ~SQLiteDatabase();

    void executeTransaction(std::function<void()> transact);

    SQLiteStatement createStatement(std::string sql);

   private:
    sqlite3 *db{};
};