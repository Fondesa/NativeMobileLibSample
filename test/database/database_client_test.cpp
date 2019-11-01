#include "database/sqlite_database.hpp"
#include "database_client_test.hpp"
#include "core/include_macros.hpp"
#include "core/test_exceptions_macros.hpp"
#include "database/database_exception.hpp"
#include AMALGAMATION(database_client.hpp)

void DatabaseClientTest::TearDown() {
    Db::Client::release();
}

TEST_F(DatabaseClientTest, givenUninitializedDbWhenGetIsInvokedThenExceptionIsThrown) {
    EXPECT_LIB_THROW(Db::Client::get(), Db::Exception);
}

TEST_F(DatabaseClientTest, givenReleasedDbWhenGetIsInvokedThenExceptionIsThrown) {
    Db::Client::create(":memory:");
    Db::Client::release();

    EXPECT_LIB_THROW(Db::Client::get(), Db::Exception);
}

TEST_F(DatabaseClientTest, givenInitializedDbWhenGetIsInvokedThenDbIsReturned) {
    Db::Client::create(":memory:");
    auto db = Db::Client::get();

    ASSERT_TRUE(db != nullptr);
    EXPECT_TRUE(std::dynamic_pointer_cast<Db::Sql::Database>(db));
}

TEST_F(DatabaseClientTest, givenInitializedDbWhenInitializeIsInvokedMultipleTimesThenDbIsNotCreatedAgain) {
    Db::Client::create(":memory:");
    auto db = Db::Client::get();
    Db::Client::create(":memory:");
    auto secondDb = Db::Client::get();

    // They should point to the same location.
    EXPECT_EQ(secondDb, db);
}