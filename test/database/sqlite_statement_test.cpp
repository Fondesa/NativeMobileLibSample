#include "sqlite_statement_test.hpp"
#include "database/smart_c_statement.hpp"
#include "database/sqlite_cursor.hpp"
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

TEST_F(SQLiteStatementTest, givenZeroRowResultWhenExecuteOptionalIntIsInvokedThenNullOptionalIsReturned) {
    auto statement = Db::Sql::Statement(db, "PRAGMA user_version = 2");

    auto result = statement.execute<stdx::optional<int>>();

    ASSERT_FALSE(result);
}

TEST_F(SQLiteStatementTest, givenErrorInSqliteStepWhenExecuteOptionalIntIsInvokedThenExceptionIsThrown) {
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
    ASSERT_THROW(statement.execute<stdx::optional<int>>(), Db::Sql::Exception);
}

TEST_F(SQLiteStatementTest, givenDifferentTypeOfColumnWhenExecuteOptionalIntIsInvokedThenExceptionIsThrown) {
    // The returned journal mode will be a string.
    auto statement = Db::Sql::Statement(db, "PRAGMA journal_mode");

    ASSERT_THROW(statement.execute<stdx::optional<int>>(), Db::Sql::Exception);
}

TEST_F(SQLiteStatementTest, givenMoreThanOneRowWhenExecuteOptionalIntIsInvokedThenExceptionIsThrown) {
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
    ASSERT_THROW(statement.execute<stdx::optional<int>>(), Db::Sql::Exception);
}

TEST_F(SQLiteStatementTest, givenResetViolationWhenExecuteOptionalIntIsInvokedThenExceptionIsThrown) {
    auto statement = ResetViolationStatement(db, "PRAGMA user_version");

    ASSERT_THROW(statement.execute<stdx::optional<int>>(), Db::Sql::Exception);
}

TEST_F(SQLiteStatementTest, givenClearBindingsViolationWhenExecuteOptionalIntIsInvokedThenExceptionIsThrown) {
    auto statement = ClearBindingsViolationStatement(db, "PRAGMA user_version");

    ASSERT_THROW(statement.execute<stdx::optional<int>>(), Db::Sql::Exception);
}

TEST_F(SQLiteStatementTest, givenOneIntRowResultWhenExecuteOptionalIntIsInvokedThenIntIsReturned) {
    int rc = sqlite3_step(Db::Sql::SmartCStatement(db, "PRAGMA user_version = 2"));
    // The statement should be executed correctly.
    ASSERT_EQ(SQLITE_DONE, rc);
    auto statement = Db::Sql::Statement(db, "PRAGMA user_version");

    auto result = statement.execute<stdx::optional<int>>();

    ASSERT_EQ(2, result);
}

TEST_F(SQLiteStatementTest, givenZeroRowResultWhenExecuteIntIsInvokedThenExceptionIsThrown) {
    auto statement = Db::Sql::Statement(db, "PRAGMA user_version = 2");

    ASSERT_THROW(statement.execute<int>(), Db::Sql::Exception);
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
    ASSERT_THROW(statement.execute<int>(), Db::Sql::Exception);
}

TEST_F(SQLiteStatementTest, givenDifferentTypeOfColumnWhenExecuteIntIsInvokedThenExceptionIsThrown) {
    // The returned journal mode will be a string.
    auto statement = Db::Sql::Statement(db, "PRAGMA journal_mode");

    ASSERT_THROW(statement.execute<int>(), Db::Sql::Exception);
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
    ASSERT_THROW(statement.execute<int>(), Db::Sql::Exception);
}

TEST_F(SQLiteStatementTest, givenResetViolationWhenExecuteIntIsInvokedThenExceptionIsThrown) {
    auto statement = ResetViolationStatement(db, "PRAGMA user_version");

    ASSERT_THROW(statement.execute<int>(), Db::Sql::Exception);
}

TEST_F(SQLiteStatementTest, givenClearBindingsViolationWhenExecuteIntIsInvokedThenExceptionIsThrown) {
    auto statement = ClearBindingsViolationStatement(db, "PRAGMA user_version");

    ASSERT_THROW(statement.execute<int>(), Db::Sql::Exception);
}

TEST_F(SQLiteStatementTest, givenOneIntRowResultWhenExecuteIntIsInvokedThenIntIsReturned) {
    int rc = sqlite3_step(Db::Sql::SmartCStatement(db, "PRAGMA user_version = 2"));
    // The statement should be executed correctly.
    ASSERT_EQ(SQLITE_DONE, rc);
    auto statement = Db::Sql::Statement(db, "PRAGMA user_version");

    auto result = statement.execute<int>();

    ASSERT_EQ(2, result);
}

TEST_F(SQLiteStatementTest, givenZeroRowResultWhenExecuteOptionalStringIsInvokedThenNullOptionalIsReturned) {
    auto statement = Db::Sql::Statement(db, "PRAGMA user_version = 2");

    auto result = statement.execute<stdx::optional<std::string>>();

    ASSERT_FALSE(result);
}

TEST_F(SQLiteStatementTest, givenErrorInSqliteStepWhenExecuteOptionalStringIsInvokedThenExceptionIsThrown) {
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
    ASSERT_THROW(statement.execute<stdx::optional<std::string>>(), Db::Sql::Exception);
}

TEST_F(SQLiteStatementTest, givenDifferentTypeOfColumnWhenExecuteOptionalStringIsInvokedThenExceptionIsThrown) {
    // The returned DB version mode will be an integer.
    auto statement = Db::Sql::Statement(db, "PRAGMA user_version");

    ASSERT_THROW(statement.execute<stdx::optional<std::string>>(), Db::Sql::Exception);
}

TEST_F(SQLiteStatementTest, givenMoreThanOneRowWhenExecuteOptionalStringIsInvokedThenExceptionIsThrown) {
    int rc = sqlite3_step(Db::Sql::SmartCStatement(db, "CREATE TABLE dummy_table (title TEXT PRIMARY KEY)"));
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
    ASSERT_THROW(statement.execute<stdx::optional<std::string>>(), Db::Sql::Exception);
}

TEST_F(SQLiteStatementTest, givenResetViolationWhenExecuteOptionalStringIsInvokedThenExceptionIsThrown) {
    auto statement = ResetViolationStatement(db, "PRAGMA journal_mode");

    ASSERT_THROW(statement.execute<stdx::optional<std::string>>(), Db::Sql::Exception);
}

TEST_F(SQLiteStatementTest, givenClearBindingsViolationWhenExecuteOptionalStringIsInvokedThenExceptionIsThrown) {
    auto statement = ClearBindingsViolationStatement(db, "PRAGMA journal_mode");

    ASSERT_THROW(statement.execute<stdx::optional<std::string>>(), Db::Sql::Exception);
}

TEST_F(SQLiteStatementTest, givenOneStringRowResultWhenExecuteOptionalStringIsInvokedThenIntIsReturned) {
    int rc = sqlite3_step(Db::Sql::SmartCStatement(db, "CREATE TABLE dummy_table (title TEXT PRIMARY KEY)"));
    // The statement should be executed correctly.
    ASSERT_EQ(SQLITE_DONE, rc);
    rc = sqlite3_step(Db::Sql::SmartCStatement(db, "INSERT INTO dummy_table (title) VALUES (\"first\")"));
    // The statement should be executed correctly.
    ASSERT_EQ(SQLITE_DONE, rc);
    auto statement = Db::Sql::Statement(db, "SELECT title FROM dummy_table");

    auto result = statement.execute<stdx::optional<std::string>>();

    ASSERT_EQ("first", result);
}

TEST_F(SQLiteStatementTest, givenValidStatementWhenExecuteCursorIsInvokedThenCursorIsReturned) {
    int rc = sqlite3_step(Db::Sql::SmartCStatement(db, "CREATE TABLE dummy_table (id INTEGER PRIMARY KEY)"));
    // The statement should be executed correctly.
    ASSERT_EQ(SQLITE_DONE, rc);
    rc = sqlite3_step(Db::Sql::SmartCStatement(db, "INSERT INTO dummy_table (id) VALUES (1)"));
    // The statement should be executed correctly.
    ASSERT_EQ(SQLITE_DONE, rc);
    auto statement = Db::Sql::Statement(db, "SELECT id FROM dummy_table");

    auto cursor = statement.execute<std::shared_ptr<Db::Cursor>>();

    EXPECT_TRUE(std::dynamic_pointer_cast<Db::Sql::Cursor>(cursor));
    ASSERT_TRUE(cursor->next());
    ASSERT_EQ(1, cursor->get<int>(0));
}

TEST_F(SQLiteStatementTest, givenErrorInSqlBindingWhenBindIntIsInvokedThenExceptionIsThrown) {
    int rc = sqlite3_step(Db::Sql::SmartCStatement(db, "CREATE TABLE dummy_table (id INTEGER PRIMARY KEY)"));
    // The statement should be executed correctly.
    ASSERT_EQ(SQLITE_DONE, rc);
    auto statement = Db::Sql::Statement(db, "INSERT INTO dummy_table (id) VALUES (?)");

    // The binding is 1-indexed so the index 0 is considered out of range.
    ASSERT_THROW(statement.bind<int>(0, 1), Db::Sql::Exception);
}

TEST_F(SQLiteStatementTest, givenValidSqlBindingWhenBindIntIsInvokedThenValueIsBoundCorrectly) {
    int rc = sqlite3_step(Db::Sql::SmartCStatement(db, "CREATE TABLE dummy_table (id INTEGER PRIMARY KEY)"));
    // The statement should be executed correctly.
    ASSERT_EQ(SQLITE_DONE, rc);
    auto statement = Db::Sql::Statement(db, "INSERT INTO dummy_table (id) VALUES (?)");

    statement.bind<int>(1, 1);
    statement.execute<void>();
    auto readStmt = Db::Sql::Statement(db, "SELECT id FROM dummy_table");
    auto cursor = readStmt.execute<std::shared_ptr<Db::Cursor>>();

    ASSERT_TRUE(cursor->next());
    ASSERT_EQ(1, cursor->get<int>(0));
}

TEST_F(SQLiteStatementTest, givenErrorInSqlBindingWhenBindDoubleIsInvokedThenExceptionIsThrown) {
    int rc = sqlite3_step(Db::Sql::SmartCStatement(db, "CREATE TABLE dummy_table (number REAL PRIMARY KEY)"));
    // The statement should be executed correctly.
    ASSERT_EQ(SQLITE_DONE, rc);
    auto statement = Db::Sql::Statement(db, "INSERT INTO dummy_table (number) VALUES (?)");

    // The binding is 1-indexed so the index 0 is considered out of range.
    ASSERT_THROW(statement.bind<double>(0, 4.5), Db::Sql::Exception);
}

TEST_F(SQLiteStatementTest, givenValidSqlBindingWhenBindDoubleIsInvokedThenValueIsBoundCorrectly) {
    int rc = sqlite3_step(Db::Sql::SmartCStatement(db, "CREATE TABLE dummy_table (number REAL PRIMARY KEY)"));
    // The statement should be executed correctly.
    ASSERT_EQ(SQLITE_DONE, rc);
    auto statement = Db::Sql::Statement(db, "INSERT INTO dummy_table (number) VALUES (?)");

    statement.bind<double>(1, 4.5);
    statement.execute<void>();
    auto readStmt = Db::Sql::Statement(db, "SELECT number FROM dummy_table");
    auto cursor = readStmt.execute<std::shared_ptr<Db::Cursor>>();

    ASSERT_TRUE(cursor->next());
    ASSERT_EQ(4.5, cursor->get<double>(0));
}

TEST_F(SQLiteStatementTest, givenErrorInSqlBindingWhenBindStringIsInvokedThenExceptionIsThrown) {
    int rc = sqlite3_step(Db::Sql::SmartCStatement(db, "CREATE TABLE dummy_table (title TEXT PRIMARY KEY)"));
    // The statement should be executed correctly.
    ASSERT_EQ(SQLITE_DONE, rc);
    auto statement = Db::Sql::Statement(db, "INSERT INTO dummy_table (title) VALUES (?)");

    // The binding is 1-indexed so the index 0 is considered out of range.
    ASSERT_THROW(statement.bind<std::string>(0, "dummy-text-value"), Db::Sql::Exception);
}

TEST_F(SQLiteStatementTest, givenValidSqlBindingWhenBindStringIsInvokedThenValueIsBoundCorrectly) {
    int rc = sqlite3_step(Db::Sql::SmartCStatement(db, "CREATE TABLE dummy_table (title TEXT PRIMARY KEY)"));
    // The statement should be executed correctly.
    ASSERT_EQ(SQLITE_DONE, rc);
    auto statement = Db::Sql::Statement(db, "INSERT INTO dummy_table (title) VALUES (?)");

    statement.bind<std::string>(1, "dummy-text-value");
    statement.execute<void>();
    auto readStmt = Db::Sql::Statement(db, "SELECT title FROM dummy_table");
    auto cursor = readStmt.execute<std::shared_ptr<Db::Cursor>>();

    ASSERT_TRUE(cursor->next());
    ASSERT_EQ("dummy-text-value", cursor->get<std::string>(0));
}

TEST_F(SQLiteStatementTest, givenErrorInSqlBindingWhenBindBoolIsInvokedThenExceptionIsThrown) {
    int rc = sqlite3_step(Db::Sql::SmartCStatement(db, "CREATE TABLE dummy_table (is_true INTEGER PRIMARY KEY)"));
    // The statement should be executed correctly.
    ASSERT_EQ(SQLITE_DONE, rc);
    auto statement = Db::Sql::Statement(db, "INSERT INTO dummy_table (is_true) VALUES (?)");

    // The binding is 1-indexed so the index 0 is considered out of range.
    ASSERT_THROW(statement.bind<bool>(0, true), Db::Sql::Exception);
}

TEST_F(SQLiteStatementTest, givenValidSqlBindingWhenBindBoolIsInvokedThenValueIsBoundCorrectly) {
    int rc = sqlite3_step(Db::Sql::SmartCStatement(db, "CREATE TABLE dummy_table (is_true INTEGER PRIMARY KEY)"));
    // The statement should be executed correctly.
    ASSERT_EQ(SQLITE_DONE, rc);
    auto statement = Db::Sql::Statement(db, "INSERT INTO dummy_table (is_true) VALUES (?)");

    statement.bind<bool>(1, true);
    statement.execute<void>();
    auto readStmt = Db::Sql::Statement(db, "SELECT is_true FROM dummy_table");
    auto cursor = readStmt.execute<std::shared_ptr<Db::Cursor>>();

    ASSERT_TRUE(cursor->next());
    ASSERT_EQ(true, cursor->get<bool>(0));
}