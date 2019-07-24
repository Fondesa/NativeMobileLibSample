#pragma once

#include <string>
#include "sqlite3/sqlite3.h"
#include "database_statement.hpp"

class SQLiteStatement : public DatabaseStatement {
   public:
    SQLiteStatement(sqlite3 *db, std::string sql);

    ~SQLiteStatement();

   protected:
    void executeVoid() override;

    std::shared_ptr<DatabaseCursor> executeCursor() override;

    void bindInt(int colIndex, int value) override;

    void bindDouble(int colIndex, double value) override;

    void bindString(int colIndex, std::string value) override;

    void bindBool(int colIndex, bool value) override;

   private:
    sqlite3_stmt *stmt{};
};