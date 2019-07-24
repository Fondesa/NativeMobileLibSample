#pragma once

#include <string>
#include "sqlite3.h"

class SQLiteCursor {
   public:
    explicit SQLiteCursor(sqlite3_stmt *stmt);

    ~SQLiteCursor() noexcept(false);

    bool next();

    template <typename T>
    T get(int colIndex);

   private:
    sqlite3_stmt *stmt;
    int columnCount;

    void ensureIndexInBounds(int colIndex);
};