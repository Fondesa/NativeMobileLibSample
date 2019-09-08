#include "database/sqlite_exception.hpp"
#include <gtest/gtest.h>

TEST(SQLiteExceptionTest, givenMsgWhenWhatIsInvokedThenMsgIsReturned) {
    std::string msg = "A dummy message.";
    auto exc = Db::Sql::Exception(msg);

    EXPECT_EQ(msg, exc.what());
}

TEST(SQLiteExceptionTest, givenSqliteErrorWhenWhatIsInvokedThenErrorStringIsReturned) {
    sqlite3 *db = nullptr;
    auto exc = Db::Sql::Exception(db);

    // To force an easy error, when the database pointer is null, the method sqlite_errmsg() returns the message
    // related to the error code SQLITE_NOMEN.
    EXPECT_STREQ(sqlite3_errstr(SQLITE_NOMEM), exc.what());
}
