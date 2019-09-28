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

class ResetViolationStatement : public Db::Sql::Statement {
   public:
    ResetViolationStatement(sqlite3 *db, std::string sql) : Db::Sql::Statement(db, sql){};

   protected:
    int reset() override {
        return SQLITE_MISUSE;
    }
};

class ClearBindingsViolationStatement : public Db::Sql::Statement {
   public:
    ClearBindingsViolationStatement(sqlite3 *db, std::string sql) : Db::Sql::Statement(db, sql){};

   protected:
    int clearBindings() override {
        return SQLITE_MISUSE;
    }
};