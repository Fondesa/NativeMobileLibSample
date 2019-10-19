#include <gtest/gtest.h>
#include <database/sqlite_exception.hpp>
#include "database/sqlite_database.hpp"
#include "database/sqlite_statement.hpp"
#include "core/test_exceptions_macros.hpp"

TEST(SQLiteDatabaseTest, givenInvalidDbPathWhenDatabaseIsCreatedThenItCanNotBeOpened) {
    // Without using the flag SQLITE_OPEN_CREATE, the database won't be created if it doesn't exist so the api
    // sqlite3_open_v2 won't return SQLITE_OK.
    EXPECT_LIB_THROW(Db::Sql::Database("invalid_path.db", SQLITE_OPEN_READWRITE), Db::Sql::Exception);
}

TEST(SQLiteDatabaseTest, givenValidDbPathWhenDestructorIsInvokedThenDatabaseIsClosed) {
    int version = 45;
    auto db = new Db::Sql::Database(":memory:", SQLITE_OPEN_READWRITE);
    // Increase the user version.
    db->createStatement("PRAGMA user_version = " + std::to_string(version))->execute<void>();
    auto obtainedVersion = db->createStatement("PRAGMA user_version")->execute<stdx::optional<int>>();
    // Just to be sure the obtained version is the one set before.
    ASSERT_EQ(version, obtainedVersion);
    // The db should be closed now.
    // When an in-memory SQLite db is closed, it is reset.
    delete db;

    auto newDb = Db::Sql::Database(":memory:", SQLITE_OPEN_READWRITE);
    obtainedVersion = newDb.createStatement("PRAGMA user_version")->execute<stdx::optional<int>>();
    // The default version is 0.
    EXPECT_EQ(0, obtainedVersion);
}

TEST(SQLiteDatabaseTest, givenInitializedDbWhenCreateStatementIsInvokedThenSqlStatementIsReturned) {
    auto db = Db::Sql::Database(":memory:", SQLITE_OPEN_READWRITE);
    auto stmt = db.createStatement("PRAGMA user_version");

    ASSERT_TRUE(stmt != nullptr);
    EXPECT_TRUE(std::dynamic_pointer_cast<Db::Sql::Statement>(stmt));
}

// It's necessary to put the following tests in the same namespace of Db::Sql::Database to allow friend classes.
namespace Db::Sql {

TEST(SQLiteDatabaseTest, givenInitializedDbWhenExecuteTransactionIsInvokedThenDbBeginsAndEndsATransaction) {
    auto db = Db::Sql::Database(":memory:", SQLITE_OPEN_READWRITE);
    auto rawDb = db.db;
    // The autocommit mode is enabled by default.
    EXPECT_TRUE(sqlite3_get_autocommit(rawDb));

    db.executeTransaction([rawDb] {
        // The autocommit mode is disabled when the SQLite db is inside a transaction.
        EXPECT_FALSE(sqlite3_get_autocommit(rawDb));
    });

    // The autocommit mode is enabled again when the transaction is closed.
    EXPECT_TRUE(sqlite3_get_autocommit(rawDb));
}

TEST(SQLiteDatabaseTest, givenInitializedDbWhenBeginTransactionFailsThenExceptionIsThrown) {
    auto db = Db::Sql::Database(":memory:", SQLITE_OPEN_READWRITE);
    auto rawDb = db.db;
    // Calling BEGIN_TRANSACTION after the SQLite db has been closed won't return SQLITE_OK.
    sqlite3_close(rawDb);

    EXPECT_LIB_THROW(db.executeTransaction({}), Db::Sql::Exception);
}

TEST(SQLiteDatabaseTest, givenInitializedDbWhenEndTransactionFailsThenExceptionIsThrown) {
    auto db = Db::Sql::Database(":memory:", SQLITE_OPEN_READWRITE);
    auto rawDb = db.db;

    EXPECT_LIB_THROW(db.executeTransaction([rawDb]{
        // Calling END_TRANSACTION after the SQLite db has been closed won't return SQLITE_OK.
        sqlite3_close(rawDb);
    }), Db::Sql::Exception);
}
}