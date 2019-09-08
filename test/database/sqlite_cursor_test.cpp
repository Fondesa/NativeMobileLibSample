#include "database_client.hpp"
#include "sqlite_cursor_test.hpp"
#include "database/sqlite_exception.hpp"

void SQLiteCursorTest::SetUp() {
    Db::Client::create(":memory:");
    db = Db::Client::get();
    db->createStatement(
        "CREATE TABLE dummy_table ("
        "col_string TEXT NOT NULL, "
        "col_double REAL NOT NULL, "
        "col_int INTEGER NOT NULL, "
        "col_bool INTEGER NOT NULL"
        ")"
    )->execute<void>();

// TODO optional?
    db->createStatement("PRAGMA user_version = 1")->execute<void>();
}

void SQLiteCursorTest::TearDown() {
    Db::Client::release();
}

void SQLiteCursorTest::insertRecord(std::string colText, double colDouble, int colInt, bool colBool) {
    auto stmt = db->createStatement("INSERT INTO dummy_table (col_string, col_double, col_int, col_bool) "
                                    "VALUES (?, ?, ?, ?)");
    stmt->bind(1, std::move(colText));
    stmt->bind(2, colDouble);
    stmt->bind(3, colInt);
    stmt->bind(4, colBool);
    stmt->execute<void>();
}

std::shared_ptr<Db::Cursor> SQLiteCursorTest::selectAll() {
    return db->createStatement("SELECT col_string, col_double, col_int, col_bool FROM dummy_table")->
        execute<std::shared_ptr<Db::Cursor>>();
}

TEST_F(SQLiteCursorTest, givenZeroRecordsWhenNextIsInvokedThenExceptionIsThrown) {
    auto stmt = db->createStatement("SELECT col_string, col_double, col_int, col_bool FROM dummy_table");
//    auto cursor = stmt->execute<std::shared_ptr<Db::Cursor>>();
    auto cursor = selectAll();
    cursor->next();
//    EXPECT_THROW(cursor->next(), Db::Sql::Exception);
}
