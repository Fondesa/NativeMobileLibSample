#pragma once

#include "sqlite3/sqlite3.h"
#include "database/smart_c_statement.hpp"
#include <gtest/gtest.h>

class SmartCStatementTest : public ::testing::Test {
   public:
    class Owner {
       public:
        explicit Owner(const Db::Sql::SmartCStatement &stmt) : stmt(stmt) {}

        Db::Sql::SmartCStatement get() {
            return stmt;
        }

       private:
        Db::Sql::SmartCStatement stmt;
    };

    class Creator {
       public:
        Creator(sqlite3 *db, const std::string &query) : stmt(db, query) {}

        SmartCStatementTest::Owner *createOwner() {
            return new Owner(stmt);
        }

        Db::Sql::SmartCStatement get() {
            return stmt;
        }

       private:
        Db::Sql::SmartCStatement stmt;
    };

   protected:
    sqlite3 *db{};

    void SetUp() override;

    void TearDown() override;
};