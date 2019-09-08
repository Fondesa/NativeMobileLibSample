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
}