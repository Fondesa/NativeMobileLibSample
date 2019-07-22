#pragma once

#include <string>
#include "sqlite3.h"

class SQLiteDatabase {
   public:
    explicit SQLiteDatabase(std::string dbPath);

    ~SQLiteDatabase();

    void executeTransaction(std::function<void()> transact);

   private:
    sqlite3 *db{};
};