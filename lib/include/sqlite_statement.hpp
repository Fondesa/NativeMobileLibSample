#pragma once

#include <string>
#include "sqlite3.h"

class SQLiteStatement {
   public:
    SQLiteStatement(sqlite3 *db, std::string sql);

    ~SQLiteStatement();

#pragma clang diagnostic push
#pragma ide diagnostic ignored "NotImplementedFunctions"
    template<typename T>
    void bind(int colIndex, T value);
#pragma clang diagnostic pop

    template<typename T>
    T execute();

   private:
    sqlite3_stmt *stmt{};

    int step();
    int clearBindings();
    int reset();
};