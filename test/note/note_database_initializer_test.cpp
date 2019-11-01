#include "core/include_macros.hpp"
#include "note_database_initializer_test.hpp"
#include "core/test_exceptions_macros.hpp"
#include "database/database_exception.hpp"
#include AMALGAMATION(database_client.hpp)
#include AMALGAMATION(note_database_initializer.hpp)

#if !__cpp_inline_variables
const std::string NoteDatabaseInitializerTest::testDbPath = "note_database_initializer_test.db";
#endif

void NoteDatabaseInitializerTest::changeVersion(int version) {
    // The DB is opened only to execute the PRAGMA statement and closed rightly after.
    Db::Client::create(testDbPath);
    Db::Client::get()->createStatement("PRAGMA user_version = " + std::to_string(version))->execute<void>();
    Db::Client::release();
}

void NoteDatabaseInitializerTest::TearDown() {
    Db::Client::release();
    // Remove the database file to reset the environment after each test.
    std::remove(testDbPath.c_str());
}

TEST_F(NoteDatabaseInitializerTest, givenSameVersionWhenInitializeIsInvokedThenDatabaseIsOpenedWithoutChanges) {
    changeVersion(NoteDb::version);

    NoteDb::initialize(testDbPath);

    auto db = Db::Client::get();
    ASSERT_TRUE(db != nullptr);
    auto version = db->createStatement("PRAGMA user_version")->execute<int>();
    EXPECT_EQ(NoteDb::version, version);
    // Get all the tables of the DB.
    auto tableCursor = db->createStatement("SELECT name FROM sqlite_master WHERE type=\"table\"")->
        execute<std::shared_ptr<Db::Cursor>>();
    // The table list should be empty since, with the first manual PRAGMA user_version statement, the DB was created
    // without creating its schema.
    EXPECT_FALSE(tableCursor->next());
}

TEST_F(NoteDatabaseInitializerTest, givenMinorVersionWhenInitializeIsInvokedThenExceptionIsThrown) {
    changeVersion(NoteDb::version + 1);

    EXPECT_LIB_THROW(NoteDb::initialize(testDbPath), Db::Exception);
}

TEST_F(NoteDatabaseInitializerTest, givenFirstCreationWhenInitializeIsInvokedThenSchemaIsCreated) {
    NoteDb::initialize(testDbPath);

    auto db = Db::Client::get();
    ASSERT_TRUE(db != nullptr);
    auto version = db->createStatement("PRAGMA user_version")->execute<int>();
    // The version should be set to NoteDb::version.
    EXPECT_EQ(NoteDb::version, version);
    // Get all the tables of the DB.
    auto tableCursor = db->createStatement("SELECT name FROM sqlite_master WHERE type=\"table\"")->
        execute<std::shared_ptr<Db::Cursor>>();
    // The schema should contain only the following tables.
    EXPECT_TRUE(tableCursor->next());
    EXPECT_EQ("notes", tableCursor->get<std::string>(0));
    EXPECT_TRUE(tableCursor->next());
    EXPECT_EQ("pending_drafts_update", tableCursor->get<std::string>(0));
    EXPECT_TRUE(tableCursor->next());
    EXPECT_EQ("pending_draft_creation", tableCursor->get<std::string>(0));
    EXPECT_FALSE(tableCursor->next());
}

TEST_F(NoteDatabaseInitializerTest, givenMajorVersionWhenInitializeIsInvokedThenVersionIsUpdated) {
    changeVersion(NoteDb::version - 1);

    NoteDb::initialize(testDbPath);

    auto db = Db::Client::get();
    ASSERT_TRUE(db != nullptr);
    auto version = db->createStatement("PRAGMA user_version")->execute<int>();
    // The version should be set to NoteDb::version.
    EXPECT_EQ(NoteDb::version, version);
}