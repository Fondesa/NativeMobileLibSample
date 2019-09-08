#pragma once

#include "sqlite3/sqlite3.h"
#include "database/sqlite_cursor.hpp"
#include <gtest/gtest.h>

namespace Db::Sql {

class SQLiteCursorTest : public ::testing::Test {
   protected:
    sqlite3 *db{};

    void SetUp() override;

    void TearDown() override;

    void insertRecord(const std::string &colText, double colDouble, int colInt, bool colBool);

    std::shared_ptr<Db::Sql::Cursor> selectAll();
};

class ResetViolationCursor : public Db::Sql::Cursor {
   public:
    ResetViolationCursor(sqlite3 *db, const SmartCStatement &stmt): Db::Sql::Cursor(db, stmt) {};

   protected:
    int reset() override {
        return SQLITE_MISUSE;
    }
};

class ClearBindingsViolationCursor : public Db::Sql::Cursor {
   public:
    ClearBindingsViolationCursor(sqlite3 *db, const SmartCStatement &stmt): Db::Sql::Cursor(db, stmt) {};

   protected:
    int clearBindings() override {
        return SQLITE_MISUSE;
    }
};
}