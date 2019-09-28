#include "sqlite_statement_test.hpp"
#include "database/smart_c_statement.hpp"
#include "database/sqlite_exception.hpp"

void SQLiteStatementTest::SetUp() {
    sqlite3_open(":memory:", &db);
}

void SQLiteStatementTest::TearDown() {
    sqlite3_close(db);
}

TEST_F(SQLiteStatementTest, givenRowResultWhenExecuteVoidIsInvokedThenExceptionIsThrown) {
    auto statement = Db::Sql::Statement(db, "PRAGMA user_version");

    ASSERT_THROW(statement.execute<void>(), Db::Sql::Exception);
}

TEST_F(SQLiteStatementTest, givenResetViolationWhenExecuteVoidIsInvokedThenExceptionIsThrown) {
    auto statement = ResetViolationStatement(db, "PRAGMA user_version = 1");

    ASSERT_THROW(statement.execute<void>(), Db::Sql::Exception);
}

TEST_F(SQLiteStatementTest, givenClearBindingsViolationWhenExecuteVoidIsInvokedThenExceptionIsThrown) {
    auto statement = ClearBindingsViolationStatement(db, "PRAGMA user_version = 1");

    ASSERT_THROW(statement.execute<void>(), Db::Sql::Exception);
}

TEST_F(SQLiteStatementTest, givenDoneResultWhenExecuteVoidIsInvokedThenExceptionIsNotThrown) {
    auto statement = Db::Sql::Statement(db, "PRAGMA user_version = 2");

    statement.execute<void>();

    auto stmt = Db::Sql::SmartCStatement(db, "PRAGMA user_version");
    // Execute the statement.
    int rc = sqlite3_step(stmt);
    // It should contain one row with the db version.
    ASSERT_EQ(SQLITE_ROW, rc);
    // Get the version.
    int version = sqlite3_column_int(stmt, 0);
    // It should be 2 because we set it before.
    ASSERT_EQ(2, version);
}

TEST_F(SQLiteStatementTest, givenZeroRowResultWhenExecuteIntIsInvokedThenNullOptionalIsReturned) {
    auto statement = Db::Sql::Statement(db, "PRAGMA user_version = 2");

    auto result = statement.execute<std::optional<int>>();

    ASSERT_FALSE(result.has_value());
}

TEST_F(SQLiteStatementTest, givenErrorInSqliteStepWhenExecuteIntIsInvokedThenExceptionIsThrown) {
    int rc = sqlite3_step(Db::Sql::SmartCStatement(db, "CREATE TABLE dummy_table (id INTEGER PRIMARY KEY)"));
    // The statement should be executed correctly.
    ASSERT_EQ(SQLITE_DONE, rc);
    // Insert a record to throw a constraint violation when it will be inserted again.
    std::string sql = "INSERT INTO dummy_table (id) VALUES (1)";
    rc = sqlite3_step(Db::Sql::SmartCStatement(db, sql));
    // The statement should be executed correctly.
    ASSERT_EQ(SQLITE_DONE, rc);

    auto statement = Db::Sql::Statement(db, sql);

    // We try to insert a record with the same id to simulate a constraint violation.
    // The error code will be SQLITE_CONSTRAINT.
    ASSERT_THROW(statement.execute<std::optional<int>>(), Db::Sql::Exception);
}

TEST_F(SQLiteStatementTest, givenDifferentTypeOfColumnWhenExecuteIntIsInvokedThenExceptionIsThrown) {
    // The returned journal mode will be a string.
    auto statement = Db::Sql::Statement(db, "PRAGMA journal_mode");

    ASSERT_THROW(statement.execute<std::optional<int>>(), Db::Sql::Exception);
}

TEST_F(SQLiteStatementTest, givenMoreThanOneRowWhenExecuteIntIsInvokedThenExceptionIsThrown) {
    int rc = sqlite3_step(Db::Sql::SmartCStatement(db, "CREATE TABLE dummy_table (id INTEGER PRIMARY KEY)"));
    // The statement should be executed correctly.
    ASSERT_EQ(SQLITE_DONE, rc);
    rc = sqlite3_step(Db::Sql::SmartCStatement(db, "INSERT INTO dummy_table (id) VALUES (1)"));
    // The statement should be executed correctly.
    ASSERT_EQ(SQLITE_DONE, rc);
    rc = sqlite3_step(Db::Sql::SmartCStatement(db, "INSERT INTO dummy_table (id) VALUES (2)"));
    // The statement should be executed correctly.
    ASSERT_EQ(SQLITE_DONE, rc);

    auto statement = Db::Sql::Statement(db, "SELECT id FROM dummy_table");
    // The result should contain two rows.
    ASSERT_THROW(statement.execute<std::optional<int>>(), Db::Sql::Exception);
}

TEST_F(SQLiteStatementTest, givenResetViolationWhenExecuteIntIsInvokedThenExceptionIsThrown) {
    auto statement = ResetViolationStatement(db, "PRAGMA user_version");

    ASSERT_THROW(statement.execute<std::optional<int>>(), Db::Sql::Exception);
}

TEST_F(SQLiteStatementTest, givenClearBindingsViolationWhenExecuteIntIsInvokedThenExceptionIsThrown) {
    auto statement = ClearBindingsViolationStatement(db, "PRAGMA user_version");

    ASSERT_THROW(statement.execute<std::optional<int>>(), Db::Sql::Exception);
}

TEST_F(SQLiteStatementTest, givenOneIntRowResultWhenExecuteIntIsInvokedThenIntIsReturned) {
    int rc = sqlite3_step(Db::Sql::SmartCStatement(db, "PRAGMA user_version = 2"));
    // The statement should be executed correctly.
    ASSERT_EQ(SQLITE_DONE, rc);
    auto statement = Db::Sql::Statement(db, "PRAGMA user_version");

    auto result = statement.execute<std::optional<int>>();

    ASSERT_EQ(2, result);
}

TEST_F(SQLiteStatementTest, givenZeroRowResultWhenExecuteStringIsInvokedThenNullOptionalIsReturned) {
    auto statement = Db::Sql::Statement(db, "PRAGMA user_version = 2");

    auto result = statement.execute<std::optional<std::string>>();

    ASSERT_FALSE(result.has_value());
}

TEST_F(SQLiteStatementTest, givenErrorInSqliteStepWhenExecuteStringIsInvokedThenExceptionIsThrown) {
    int rc = sqlite3_step(Db::Sql::SmartCStatement(db, "CREATE TABLE dummy_table (id INTEGER PRIMARY KEY)"));
    // The statement should be executed correctly.
    ASSERT_EQ(SQLITE_DONE, rc);
    // Insert a record to throw a constraint violation when it will be inserted again.
    std::string sql = "INSERT INTO dummy_table (id) VALUES (1)";
    rc = sqlite3_step(Db::Sql::SmartCStatement(db, sql));
    // The statement should be executed correctly.
    ASSERT_EQ(SQLITE_DONE, rc);

    auto statement = Db::Sql::Statement(db, sql);

    // We try to insert a record with the same id to simulate a constraint violation.
    // The error code will be SQLITE_CONSTRAINT.
    ASSERT_THROW(statement.execute<std::optional<std::string>>(), Db::Sql::Exception);
}

TEST_F(SQLiteStatementTest, givenDifferentTypeOfColumnWhenExecuteStringIsInvokedThenExceptionIsThrown) {
    // The returned DB version mode will be an integer.
    auto statement = Db::Sql::Statement(db, "PRAGMA user_version");

    ASSERT_THROW(statement.execute<std::optional<std::string>>(), Db::Sql::Exception);
}

TEST_F(SQLiteStatementTest, givenMoreThanOneRowWhenExecuteStringIsInvokedThenExceptionIsThrown) {
    int rc = sqlite3_step(Db::Sql::SmartCStatement(db, "CREATE TABLE dummy_table (title TEXT)"));
    // The statement should be executed correctly.
    ASSERT_EQ(SQLITE_DONE, rc);
    rc = sqlite3_step(Db::Sql::SmartCStatement(db, "INSERT INTO dummy_table (title) VALUES (\"first\")"));
    // The statement should be executed correctly.
    ASSERT_EQ(SQLITE_DONE, rc);
    rc = sqlite3_step(Db::Sql::SmartCStatement(db, "INSERT INTO dummy_table (title) VALUES (\"second\")"));
    // The statement should be executed correctly.
    ASSERT_EQ(SQLITE_DONE, rc);

    auto statement = Db::Sql::Statement(db, "SELECT title FROM dummy_table");
    // The result should contain two rows.
    ASSERT_THROW(statement.execute<std::optional<std::string>>(), Db::Sql::Exception);
}

TEST_F(SQLiteStatementTest, givenResetViolationWhenExecuteStringIsInvokedThenExceptionIsThrown) {
    auto statement = ResetViolationStatement(db, "PRAGMA journal_mode");

    ASSERT_THROW(statement.execute<std::optional<std::string>>(), Db::Sql::Exception);
}

TEST_F(SQLiteStatementTest, givenClearBindingsViolationWhenExecuteStringIsInvokedThenExceptionIsThrown) {
    auto statement = ClearBindingsViolationStatement(db, "PRAGMA journal_mode");

    ASSERT_THROW(statement.execute<std::optional<std::string>>(), Db::Sql::Exception);
}

TEST_F(SQLiteStatementTest, givenOneStringRowResultWhenExecuteStringIsInvokedThenIntIsReturned) {
    int rc = sqlite3_step(Db::Sql::SmartCStatement(db, "CREATE TABLE dummy_table (title TEXT)"));
    // The statement should be executed correctly.
    ASSERT_EQ(SQLITE_DONE, rc);
    rc = sqlite3_step(Db::Sql::SmartCStatement(db, "INSERT INTO dummy_table (title) VALUES (\"first\")"));
    // The statement should be executed correctly.
    ASSERT_EQ(SQLITE_DONE, rc);
    auto statement = Db::Sql::Statement(db, "SELECT title FROM dummy_table");

    auto result = statement.execute<std::optional<std::string>>();

    ASSERT_EQ("first", result);
}