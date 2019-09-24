#include "sqlite_statement_test.hpp"
#include "database/smart_c_statement.hpp"

void SQLiteStatementTest::SetUp() {
    sqlite3_open(":memory:", &db);
}

void SQLiteStatementTest::TearDown() {
    sqlite3_close(db);
}
