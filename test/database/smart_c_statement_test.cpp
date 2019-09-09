#include "smart_c_statement_test.hpp"
#include "database/smart_c_statement.hpp"
#include "database/sqlite_exception.hpp"

void SmartCStatementTest::SetUp() {
    sqlite3_open(":memory:", &db);
    sqlite3_stmt *stmt;
    // Create the table.
    int rc = sqlite3_prepare_v2(db, "CREATE TABLE dummy_table (col_int INTEGER PRIMARY KEY)", -1, &stmt, nullptr);
    // We assume the result is ok since we haven't to test SQLite APIs.
    ASSERT_EQ(SQLITE_OK, rc);
    // Execute the statement.
    rc = sqlite3_step(stmt);
    // We assume the result is ok since we haven't to test the SQLite APIs.
    ASSERT_EQ(SQLITE_DONE, rc);
    sqlite3_finalize(stmt);
}

void SmartCStatementTest::TearDown() {
    sqlite3_close(db);
}

TEST_F(SmartCStatementTest, givenValidQueryWhenSmartCStatementIsExecutedThenOriginalStatementIsExecutedCorrectly) {
    std::string query = "INSERT INTO dummy_table (col_int) VALUES (1)";

    auto createTableStmt = Db::Sql::SmartCStatement(db, query);
    // Executes the SmartCStatement using the pointer operator.
    int rc = sqlite3_step(createTableStmt);

    ASSERT_EQ(SQLITE_DONE, rc);
    // To be sure the statement is executed correctly, we verify to have at least one row in dummy_table.
    // To do that, we execute a SELECT query expecting to return SQLITE_ROW and not SQLITE_DONE.
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, "SELECT col_int FROM dummy_table", -1, &stmt, nullptr);
    // We assume the result is ok since we haven't to test the SQLite APIs.
    ASSERT_EQ(SQLITE_OK, rc);
    rc = sqlite3_step(stmt);
    EXPECT_EQ(SQLITE_ROW, rc);
    sqlite3_finalize(stmt);
}

TEST_F(SmartCStatementTest, givenInvalidQueryWhenSmartCStatementIsCreatedThenExceptionIsThrown) {
    std::string query = "INVALID_INSERT INTO dummy_table (col_int) VALUES (1)";

    EXPECT_THROW(Db::Sql::SmartCStatement(db, query), Db::Sql::Exception);
}

TEST_F(SmartCStatementTest, givenEmptyQueryWhenSmartCStatementIsCreatedThenExceptionIsThrown) {
    EXPECT_THROW(Db::Sql::SmartCStatement(db, ""), Db::Sql::Exception);
}

TEST_F(SmartCStatementTest, givenTwoOwnersWhenBothDestructorsAreInvokedThenStatementIsFinalized) {
    // Insert one record in dummy_table to simulate the busy behavior of SQLite statements.
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, "INSERT INTO dummy_table (col_int) VALUES (1)", -1, &stmt, nullptr);
    // We assume the result is ok since we haven't to test the SQLite APIs.
    ASSERT_EQ(SQLITE_OK, rc);
    rc = sqlite3_step(stmt);
    ASSERT_EQ(SQLITE_DONE, rc);
    sqlite3_finalize(stmt);

    // The reference count will be increased to 1.
    auto creator = new Creator(db, "SELECT col_int FROM dummy_table");
    // The reference count will be increased to 2.
    auto owner = creator->createOwner();
    // Obtain the original statement to verify its busy state.
    sqlite3_stmt *originalStmt = owner->get();

    // Execute the statement when the reference count is 2.
    rc = sqlite3_step(originalStmt);
    // The statement should be executed correctly.
    ASSERT_EQ(SQLITE_ROW, rc);
    // The statement should be busy since it didn't end its execution and it's not finalized.
    ASSERT_TRUE(sqlite3_stmt_busy(originalStmt));

    // The reference count will be decreased to 1.
    delete creator;
    // The statement should be busy since it didn't end its execution and it's not finalized.
    ASSERT_TRUE(sqlite3_stmt_busy(originalStmt));

    // The reference count will be decreased to 0.
    delete owner;
    // The statement shouldn't be busy anymore since it should have been finalized.
    ASSERT_FALSE(sqlite3_stmt_busy(originalStmt));
}

TEST_F(SmartCStatementTest, givenMultipleOwnersWhenUseCountIsInvokedThenTheOwnerNumberIsReturned) {
    // The count will be increased to 1.
    auto creator = new Creator(db, "SELECT col_int FROM dummy_table");
    // The count will be increased to 2.
    auto stmt = creator->get();
    ASSERT_EQ(2, stmt.useCount());

    // The count will be increased to 3.
    auto owner = creator->createOwner();
    ASSERT_EQ(3, stmt.useCount());

    // One owner of the statement is destroyed so the count is decreased by one.
    delete creator;
    ASSERT_EQ(2, stmt.useCount());

    // The count will be increased to 3 again since we are invoking the copy constructor.
    auto firstCopy = new Db::Sql::SmartCStatement(stmt);
    ASSERT_EQ(3, stmt.useCount());

    // The other owner of the statement is destroyed so the count is decreased by one.
    delete owner;
    ASSERT_EQ(2, stmt.useCount());

    // The count will be decreased to 1 again since we are invoking the destructor of the first copy.
    delete firstCopy;
    ASSERT_EQ(1, stmt.useCount());

    // The count will be increased to 3 again since we are invoking the copy constructor.
    auto secondCopy = stmt;
    ASSERT_EQ(2, stmt.useCount());
}