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

TEST_F(SQLiteCursorTest, givenZeroRecordsWhenNextIsInvokedThenNextReturnsFalse) {
    auto cursor = selectAll();

    EXPECT_FALSE(cursor->next());
}

TEST_F(SQLiteCursorTest, givenAtLeastOneRecordWhenNextIsInvokedThenNextReturnsTrue) {
    insertRecord("text", 4.5, 2, true);
    auto cursor = selectAll();

    EXPECT_TRUE(cursor->next());
    EXPECT_FALSE(cursor->next());
}

TEST_F(SQLiteCursorTest, givenFalseNextWhenGetIntIsInvokedThenExceptionIsThrown) {
    auto cursor = selectAll();
    cursor->next();

    EXPECT_THROW(cursor->get<int>(2), Db::Sql::Exception);
}

TEST_F(SQLiteCursorTest, givenTrueNextWhenGetIntIsInvokedOnOutOfIndexColumnThenExceptionIsThrown) {
    insertRecord("text", 4.5, 2, true);
    auto cursor = selectAll();
    cursor->next();

    EXPECT_THROW(cursor->get<int>(67), Db::Sql::Exception);
}

TEST_F(SQLiteCursorTest, givenTrueNextWhenGetIntIsInvokedOnDifferentTypeColumnThenExceptionIsThrown) {
    insertRecord("text", 4.5, 2, true);
    auto cursor = selectAll();
    cursor->next();

    EXPECT_THROW(cursor->get<int>(1), Db::Sql::Exception);
}

TEST_F(SQLiteCursorTest, givenTrueNextWhenGetIntIsInvokedOnCorrectColumnThenValueIsReturned) {
    auto expected = 2;
    insertRecord("text", 4.5, expected, true);
    auto cursor = selectAll();
    cursor->next();

    auto value = cursor->get<int>(2);

    EXPECT_EQ(expected, value);
}

TEST_F(SQLiteCursorTest, givenFalseNextWhenGetDoubleIsInvokedThenExceptionIsThrown) {
    auto cursor = selectAll();
    cursor->next();

    EXPECT_THROW(cursor->get<double>(1), Db::Sql::Exception);
}

TEST_F(SQLiteCursorTest, givenTrueNextWhenGetDoubleIsInvokedOnOutOfIndexColumnThenExceptionIsThrown) {
    insertRecord("text", 4.5, 2, true);
    auto cursor = selectAll();
    cursor->next();

    EXPECT_THROW(cursor->get<double>(67), Db::Sql::Exception);
}

TEST_F(SQLiteCursorTest, givenTrueNextWhenGetDoubleIsInvokedOnDifferentTypeColumnThenExceptionIsThrown) {
    insertRecord("text", 4.5, 2, true);
    auto cursor = selectAll();
    cursor->next();

    EXPECT_THROW(cursor->get<double>(2), Db::Sql::Exception);
}

TEST_F(SQLiteCursorTest, givenTrueNextWhenGetDoubleIsInvokedOnCorrectColumnThenValueIsReturned) {
    auto expected = 4.5;
    insertRecord("text", expected, 2, true);
    auto cursor = selectAll();
    cursor->next();

    auto value = cursor->get<double>(1);

    EXPECT_EQ(expected, value);
}

TEST_F(SQLiteCursorTest, givenFalseNextWhenGetBoolIsInvokedThenExceptionIsThrown) {
    auto cursor = selectAll();
    cursor->next();

    EXPECT_THROW(cursor->get<bool>(3), Db::Sql::Exception);
}

TEST_F(SQLiteCursorTest, givenTrueNextWhenGetBoolIsInvokedOnOutOfIndexColumnThenExceptionIsThrown) {
    insertRecord("text", 4.5, 2, true);
    auto cursor = selectAll();
    cursor->next();

    EXPECT_THROW(cursor->get<bool>(67), Db::Sql::Exception);
}

TEST_F(SQLiteCursorTest, givenTrueNextWhenGetBoolIsInvokedOnDifferentTypeColumnThenExceptionIsThrown) {
    insertRecord("text", 4.5, 2, true);
    auto cursor = selectAll();
    cursor->next();

    EXPECT_THROW(cursor->get<bool>(1), Db::Sql::Exception);
}

TEST_F(SQLiteCursorTest, givenTrueNextWhenGetBoolIsInvokedOnIntColumnWithValueZeroThenFalseIsReturned) {
    insertRecord("text", 4.5, 0, true);
    auto cursor = selectAll();
    cursor->next();

    auto value = cursor->get<bool>(2);

    EXPECT_EQ(false, value);
}

TEST_F(SQLiteCursorTest, givenTrueNextWhenGetBoolIsInvokedOnIntColumnWithValueDifferentThanZeroThenFalseIsReturned) {
    insertRecord("text", 4.5, 4645, true);
    auto cursor = selectAll();
    cursor->next();

    auto value = cursor->get<bool>(2);

    EXPECT_EQ(true, value);
}

TEST_F(SQLiteCursorTest, givenTrueNextWhenGetBoolIsInvokedOnCorrectColumnThenValueIsReturned) {
    auto expected = true;
    insertRecord("text", 4.5, 2, expected);
    auto cursor = selectAll();
    cursor->next();

    auto value = cursor->get<bool>(3);

    EXPECT_EQ(expected, value);
}

TEST_F(SQLiteCursorTest, givenFalseNextWhenGetStringIsInvokedThenExceptionIsThrown) {
    auto cursor = selectAll();
    cursor->next();

    EXPECT_THROW(cursor->get<std::string>(0), Db::Sql::Exception);
}

TEST_F(SQLiteCursorTest, givenTrueNextWhenGetStringIsInvokedOnOutOfIndexColumnThenExceptionIsThrown) {
    insertRecord("text", 4.5, 2, true);
    auto cursor = selectAll();
    cursor->next();

    EXPECT_THROW(cursor->get<std::string>(67), Db::Sql::Exception);
}

TEST_F(SQLiteCursorTest, givenTrueNextWhenGetStringIsInvokedOnDifferentTypeColumnThenExceptionIsThrown) {
    insertRecord("text", 4.5, 2, true);
    auto cursor = selectAll();
    cursor->next();

    EXPECT_THROW(cursor->get<std::string>(2), Db::Sql::Exception);
}

TEST_F(SQLiteCursorTest, givenTrueNextWhenGetStringIsInvokedOnCorrectColumnThenValueIsReturned) {
    auto expected = "text";
    insertRecord(expected, 4.5, 2, true);
    auto cursor = selectAll();
    cursor->next();

    auto value = cursor->get<std::string>(0);

    EXPECT_EQ(expected, value);
}
