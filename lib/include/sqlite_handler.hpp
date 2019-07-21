#pragma once

#include <string>
#include "sqlite3.h"

class SQLiteHandler {
   public:
    SQLiteHandler(std::string dbPath);

    ~SQLiteHandler();

   private:
    sqlite3* db;
};