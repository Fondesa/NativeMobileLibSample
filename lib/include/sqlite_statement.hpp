#pragma once

#include <string>
#include "sqlite3.h"

class SQLiteStatement {
   public:
    SQLiteStatement(sqlite3 *db, std::string sql);

    ~SQLiteStatement();

    template<typename T>
    T execute();

   private:
    sqlite3_stmt *stmt{};

    int step();
    int clearBindings();
    int reset();
};