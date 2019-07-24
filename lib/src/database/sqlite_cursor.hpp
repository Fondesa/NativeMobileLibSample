#pragma once

#include <string>
#include "sqlite3/sqlite3.h"
#include "database_cursor.hpp"

class SQLiteCursor : public DatabaseCursor {
   public:
    explicit SQLiteCursor(sqlite3_stmt *stmt);

    ~SQLiteCursor();

    bool next() override;

   protected:
    void ensureIndexInBounds(int colIndex) override;

    int getInt(int colIndex) override;

    double getDouble(int colIndex) override;

    std::string getString(int colIndex) override;

    bool getBool(int colIndex) override;

   private:
    sqlite3_stmt *stmt;
    int columnCount;
};