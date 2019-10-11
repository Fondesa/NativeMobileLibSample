#include "database_client.hpp"
#include "note_database_initializer.hpp"
#include "drafts_repository_impl_test.hpp"

void DraftsRepositoryImplTest::SetUp() {
    NoteDb::initialize(":memory:");
    db = Db::Client::get();
    repository = std::make_shared<DraftsRepositoryImpl>(db);
}

void DraftsRepositoryImplTest::TearDown() {
    repository = nullptr;
    db = nullptr;
    Db::Client::release();
}

int DraftsRepositoryImplTest::getNotesCount() {
    return db->createStatement("SELECT COUNT(*) FROM notes")->execute<int>();
}

int DraftsRepositoryImplTest::getLastRowId() {
    // It returns the rowid of the last inserted record.
    return db->createStatement("SELECT last_insert_rowid()")->execute<int>();
}

TEST_F(DraftsRepositoryImplTest, givenNoPendingNewWhenGetNewIsInvokedThenNullOptionalIsReturned) {
    auto draft = repository->getNew();

    ASSERT_FALSE(draft.has_value());
}

TEST_F(DraftsRepositoryImplTest, givenPendingNewInDbWhenGetNewIsInvokedThenDraftIsReturned) {
    auto expectedTitle = "dummy-title";
    auto expectedDescription = "dummy-description";
    auto stmt = db->createStatement("INSERT INTO pending_draft_creation (id, title, description) VALUES (0, ?, ?)");
    stmt->bind<std::string>(1, expectedTitle);
    stmt->bind<std::string>(2, expectedDescription);
    stmt->execute<void>();

    auto draft = repository->getNew();

    ASSERT_TRUE(draft.has_value());
    EXPECT_EQ(expectedTitle, draft->getTitle());
    EXPECT_EQ(expectedDescription, draft->getDescription());
}

TEST_F(DraftsRepositoryImplTest, givenNoNewDraftInDbWhenUpdateNewTitleIsInvokedThenDraftIsNotStoredInDb) {
    repository->updateNewTitle("dummy-title");

    auto recordCount = db->createStatement("SELECT COUNT(*) FROM pending_draft_creation")->execute<int>();
    ASSERT_EQ(0, recordCount);
}

TEST_F(DraftsRepositoryImplTest, givenNoNewDraftInDbWhenUpdateNewDescriptionIsInvokedThenDraftIsNotStoredInDb) {
    repository->updateNewDescription("dummy-description");

    auto recordCount = db->createStatement("SELECT COUNT(*) FROM pending_draft_creation")->execute<int>();
    ASSERT_EQ(0, recordCount);
}

TEST_F(DraftsRepositoryImplTest, givenNewDraftWithOnlyTitleWhenGetNewIsInvokedThenDraftIsReturnedWithEmptyDescription) {
    auto expectedTitle = "dummy-title";
    repository->updateNewTitle(expectedTitle);

    auto draft = repository->getNew();

    ASSERT_TRUE(draft.has_value());
    EXPECT_EQ(expectedTitle, draft->getTitle());
    EXPECT_EQ("", draft->getDescription());
}

TEST_F(DraftsRepositoryImplTest, givenNewDraftWithOnlyDescriptionWhenGetNewIsInvokedThenDraftIsReturnedWithEmptyTitle) {
    auto expectedDescription = "dummy-description";
    repository->updateNewDescription(expectedDescription);

    auto draft = repository->getNew();

    ASSERT_TRUE(draft.has_value());
    EXPECT_EQ("", draft->getTitle());
    EXPECT_EQ(expectedDescription, draft->getDescription());
}

TEST_F(DraftsRepositoryImplTest,
       givenNewDraftWithOnlyTitleAndDraftFromDbWhenGetNewIsInvokedThenCompleteDraftIsReturned) {
    auto expectedTitle = "dummy-title";
    auto expectedDescription = "dummy-description";
    // Simulate an existing record in the DB.
    auto stmt = db->createStatement("INSERT INTO pending_draft_creation (id, title, description) VALUES (0, ?, ?)");
    stmt->bind<std::string>(1, "old-title");
    stmt->bind<std::string>(2, expectedDescription);
    stmt->execute<void>();
    // Update the title after to override the old one.
    repository->updateNewTitle(expectedTitle);

    auto draft = repository->getNew();

    ASSERT_TRUE(draft.has_value());
    EXPECT_EQ(expectedTitle, draft->getTitle());
    EXPECT_EQ(expectedDescription, draft->getDescription());
}

TEST_F(DraftsRepositoryImplTest,
       givenNewDraftWithOnlyDescriptionAndDraftFromDbWhenGetNewIsInvokedThenCompleteDraftIsReturned) {
    auto expectedTitle = "dummy-title";
    auto expectedDescription = "dummy-description";
    // Simulate an existing record in the DB.
    auto stmt = db->createStatement("INSERT INTO pending_draft_creation (id, title, description) VALUES (0, ?, ?)");
    stmt->bind<std::string>(1, expectedTitle);
    stmt->bind<std::string>(2, "old-description");
    stmt->execute<void>();
    // Update the description after to override the old one.
    repository->updateNewDescription(expectedDescription);

    auto draft = repository->getNew();

    ASSERT_TRUE(draft.has_value());
    EXPECT_EQ(expectedTitle, draft->getTitle());
    EXPECT_EQ(expectedDescription, draft->getDescription());
}

TEST_F(DraftsRepositoryImplTest,
       givenNoNewDraftInDbWhenUpdateNewDescriptionIsInvokedAfterUpdateNewTitleThenCompleteDraftIsReturned) {
    auto expectedTitle = "dummy-title";
    auto expectedDescription = "dummy-description";
    repository->updateNewTitle(expectedTitle);
    repository->updateNewDescription(expectedDescription);

    auto draft = repository->getNew();

    ASSERT_TRUE(draft.has_value());
    EXPECT_EQ(expectedTitle, draft->getTitle());
    EXPECT_EQ(expectedDescription, draft->getDescription());
}

TEST_F(DraftsRepositoryImplTest,
       givenNoNewDraftInDbWhenUpdateNewTitleIsInvokedAfterUpdateNewDescriptionThenCompleteDraftIsReturned) {
    auto expectedTitle = "dummy-title";
    auto expectedDescription = "dummy-description";
    repository->updateNewDescription(expectedDescription);
    repository->updateNewTitle(expectedTitle);

    auto draft = repository->getNew();

    ASSERT_TRUE(draft.has_value());
    EXPECT_EQ(expectedTitle, draft->getTitle());
    EXPECT_EQ(expectedDescription, draft->getDescription());
}