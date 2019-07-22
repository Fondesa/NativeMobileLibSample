#pragma once

#include <string>
#include "sqlite3.h"

class SQLiteHandler {
   public:
    explicit SQLiteHandler(std::string dbPath);

    ~SQLiteHandler();

    void executeTransaction(std::function<void()> transact);

   private:
    sqlite3 *db{};
};