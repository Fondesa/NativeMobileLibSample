#pragma once

#include "database.hpp"
#include "database_statement.hpp"
#include "sqlite3/sqlite3.h"

namespace Db::Sql {

class Database : public Db::Database {
   public:
    Database(std::string dbPath, int flags);

    ~Database();

    void executeTransaction(std::function<void()> transact) const override;

    [[nodiscard]] std::shared_ptr<Db::Statement> createStatement(std::string sql) const override;

   private:
    sqlite3 *db{};

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedStructInspection"
    // This is a workaround to access the private member sqlite3 *db inside the following tests.
    // GTest creates classes named {test suite}_{test name}_Test.
    // Even if this is considered a bad behavior, the fastest way to test a failure in beginning or ending a SQLite
    // transaction is to operate directly on the db and for example to close it.
    friend class SQLiteDatabaseTest_givenInitializedDbWhenExecuteTransactionIsInvokedThenDbBeginsAndEndsATransaction_Test;
    friend class SQLiteDatabaseTest_givenInitializedDbWhenBeginTransactionFailsThenExceptionIsThrown_Test;
    friend class SQLiteDatabaseTest_givenInitializedDbWhenEndTransactionFailsThenExceptionIsThrown_Test;
#pragma clang diagnostic pop
};
}