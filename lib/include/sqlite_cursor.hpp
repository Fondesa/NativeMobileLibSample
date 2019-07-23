#pragma once

#include <string>
#include "sqlite3.h"

class SQLiteCursor {
   public:
    SQLiteCursor(sqlite3_stmt *stmt);

    ~SQLiteCursor() noexcept(false);

    bool next();

    int getInt(int colIndex);

    double getDouble(int colIndex);

    std::string getString(int colIndex);

   private:
    sqlite3_stmt *stmt;
    int columnCount;

    void ensureIndexInBounds(int colIndex);
};