#pragma once

#include <gtest/gtest.h>
#include "database/sqlite_statement.hpp"
#include "sqlite3/sqlite3.h"

class SQLiteStatementTest : public ::testing::Test {
   protected:
    sqlite3 *db{};

    void SetUp() override;

    void TearDown() override;
};