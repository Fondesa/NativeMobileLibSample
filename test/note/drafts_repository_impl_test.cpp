#include "database_client.hpp"
#include "note_database_initializer.hpp"
#include "drafts_repository_impl_test.hpp"
#include "note/incomplete_draft_exception.hpp"

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

int DraftsRepositoryImplTest::getPendingNewDraftCount() {
    return db->createStatement("SELECT COUNT(*) FROM pending_draft_creation")->execute<int>();
}

int DraftsRepositoryImplTest::getPendingExistingDraftCount() {
    return db->createStatement("SELECT COUNT(*) FROM pending_drafts_update")->execute<int>();
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

    ASSERT_EQ(0, getPendingNewDraftCount());
}

TEST_F(DraftsRepositoryImplTest, givenNoNewDraftInDbWhenUpdateNewDescriptionIsInvokedThenDraftIsNotStoredInDb) {
    repository->updateNewDescription("dummy-description");

    ASSERT_EQ(0, getPendingNewDraftCount());
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

TEST_F(DraftsRepositoryImplTest, givenDraftInMemoryWhenDeleteNewIsInvokedThenDraftIsNotAccessibleAnymore) {
    repository->updateNewTitle("dummy-title");
    repository->updateNewDescription("dummy-description");

    repository->deleteNew();

    auto draft = repository->getNew();
    ASSERT_FALSE(draft.has_value());
}

TEST_F(DraftsRepositoryImplTest, givenDraftInDbWhenDeleteNewIsInvokedThenDraftIsNotAccessibleAnymore) {
    auto stmt = db->createStatement("INSERT INTO pending_draft_creation (id, title, description) VALUES (0, ?, ?)");
    stmt->bind<std::string>(1, "dummy-title");
    stmt->bind<std::string>(2, "dummy-description");
    stmt->execute<void>();

    repository->deleteNew();

    auto draft = repository->getNew();
    ASSERT_FALSE(draft.has_value());
}

TEST_F(DraftsRepositoryImplTest, givenNoPendingExistingWhenGetExistingIsInvokedThenNullOptionalIsReturned) {
    auto draft = repository->getExisting(1);

    ASSERT_FALSE(draft.has_value());
}

TEST_F(DraftsRepositoryImplTest, givenPendingExistingInDbWhenGetExistingIsInvokedThenDraftIsReturned) {
    int draftId = 45;
    auto expectedTitle = "dummy-title";
    auto expectedDescription = "dummy-description";
    auto stmt = db->createStatement("INSERT INTO pending_drafts_update (rowid, title, description) VALUES (?, ?, ?)");
    stmt->bind<int>(1, draftId);
    stmt->bind<std::string>(2, expectedTitle);
    stmt->bind<std::string>(3, expectedDescription);
    stmt->execute<void>();

    auto draft = repository->getExisting(draftId);

    ASSERT_TRUE(draft.has_value());
    EXPECT_EQ(expectedTitle, draft->getTitle());
    EXPECT_EQ(expectedDescription, draft->getDescription());
}

TEST_F(DraftsRepositoryImplTest, givenNoExistingDraftInDbWhenUpdateExistingTitleIsInvokedThenDraftIsNotStoredInDb) {
    repository->updateExistingTitle(1, "dummy-title");

    ASSERT_EQ(0, getPendingExistingDraftCount());
}

TEST_F(DraftsRepositoryImplTest,
       givenNoExistingDraftInDbWhenUpdateExistingDescriptionIsInvokedThenDraftIsNotStoredInDb) {
    repository->updateExistingDescription(1, "dummy-description");

    ASSERT_EQ(0, getPendingExistingDraftCount());
}

TEST_F(DraftsRepositoryImplTest, givenExistingDraftWithOnlyTitleWhenGetExistingIsInvokedThenExceptionIsThrown) {
    int draftId = 45;
    repository->updateExistingTitle(draftId, "dummy-title");

    ASSERT_THROW(repository->getExisting(draftId), IncompleteDraftException);
}

TEST_F(DraftsRepositoryImplTest, givenExistingDraftWithOnlyDescriptionWhenGetExistingIsInvokedThenExceptionIsThrown) {
    int draftId = 45;
    repository->updateExistingDescription(draftId, "dummy-description");

    ASSERT_THROW(repository->getExisting(draftId), IncompleteDraftException);
}

TEST_F(DraftsRepositoryImplTest,
       givenExistingDraftWithOnlyTitleAndDraftFromDbWhenGetExistingIsInvokedThenCompleteDraftIsReturned) {
    int draftId = 45;
    auto expectedTitle = "dummy-title";
    auto expectedDescription = "dummy-description";
    // Simulate an existing record in the DB.
    auto stmt = db->createStatement("INSERT INTO pending_drafts_update (rowid, title, description) VALUES (?, ?, ?)");
    stmt->bind<int>(1, draftId);
    stmt->bind<std::string>(2, "old-title");
    stmt->bind<std::string>(3, expectedDescription);
    stmt->execute<void>();
    // Update the title after to override the old one.
    repository->updateExistingTitle(draftId, expectedTitle);

    auto draft = repository->getExisting(draftId);

    ASSERT_TRUE(draft.has_value());
    EXPECT_EQ(expectedTitle, draft->getTitle());
    EXPECT_EQ(expectedDescription, draft->getDescription());
}

TEST_F(DraftsRepositoryImplTest,
       givenExistingDraftWithOnlyDescriptionAndDraftFromDbWhenGetExistingIsInvokedThenCompleteDraftIsReturned) {
    int draftId = 45;
    auto expectedTitle = "dummy-title";
    auto expectedDescription = "dummy-description";
    // Simulate an existing record in the DB.
    auto stmt = db->createStatement("INSERT INTO pending_drafts_update (rowid, title, description) VALUES (?, ?, ?)");
    stmt->bind<int>(1, draftId);
    stmt->bind<std::string>(2, expectedTitle);
    stmt->bind<std::string>(3, "old-description");
    stmt->execute<void>();
    // Update the description after to override the old one.
    repository->updateExistingDescription(draftId, expectedDescription);

    auto draft = repository->getExisting(draftId);

    ASSERT_TRUE(draft.has_value());
    EXPECT_EQ(expectedTitle, draft->getTitle());
    EXPECT_EQ(expectedDescription, draft->getDescription());
}

TEST_F(DraftsRepositoryImplTest,
       givenNoExistingDraftInDbWhenUpdateExistingDescriptionIsInvokedAfterUpdateExistingTitleThenCompleteDraftIsReturned) {
    int draftId = 45;
    auto expectedTitle = "dummy-title";
    auto expectedDescription = "dummy-description";
    repository->updateExistingTitle(draftId, expectedTitle);
    repository->updateExistingDescription(draftId, expectedDescription);

    auto draft = repository->getExisting(draftId);

    ASSERT_TRUE(draft.has_value());
    EXPECT_EQ(expectedTitle, draft->getTitle());
    EXPECT_EQ(expectedDescription, draft->getDescription());
}

TEST_F(DraftsRepositoryImplTest,
       givenNoExistingDraftInDbWhenUpdateExistingTitleIsInvokedAfterUpdateExistingDescriptionThenCompleteDraftIsReturned) {
    int draftId = 45;
    auto expectedTitle = "dummy-title";
    auto expectedDescription = "dummy-description";
    repository->updateExistingDescription(draftId, expectedDescription);
    repository->updateExistingTitle(draftId, expectedTitle);

    auto draft = repository->getExisting(draftId);

    ASSERT_TRUE(draft.has_value());
    EXPECT_EQ(expectedTitle, draft->getTitle());
    EXPECT_EQ(expectedDescription, draft->getDescription());
}

TEST_F(DraftsRepositoryImplTest, givenDraftInMemoryWhenDeleteExistingIsInvokedThenDraftIsNotAccessibleAnymore) {
    int draftId = 45;
    repository->updateExistingTitle(draftId, "dummy-title");
    repository->updateExistingDescription(draftId, "dummy-description");

    repository->deleteExisting(draftId);

    auto draft = repository->getExisting(draftId);
    ASSERT_FALSE(draft.has_value());
}

TEST_F(DraftsRepositoryImplTest, givenDraftInDbWhenDeleteExistingIsInvokedThenDraftIsNotAccessibleAnymore) {
    int draftId = 45;
    auto stmt = db->createStatement("INSERT INTO pending_drafts_update (rowid, title, description) VALUES (?, ?, ?)");
    stmt->bind<int>(1, draftId);
    stmt->bind<std::string>(2, "dummy-title");
    stmt->bind<std::string>(3, "dummy-description");
    stmt->execute<void>();

    repository->deleteExisting(draftId);

    auto draft = repository->getExisting(draftId);
    ASSERT_FALSE(draft.has_value());
}

TEST_F(DraftsRepositoryImplTest, givenDraftsInMemoryWhenDeleteAllIsInvokedThenAllDraftsAreNotAccessibleAnymore) {
    repository->updateNewTitle("dummy-title");
    repository->updateNewDescription("dummy-description");
    int firstExistingId = 45;
    int secondExistingId = 87;
    repository->updateExistingTitle(firstExistingId, "dummy-title");
    repository->updateExistingDescription(firstExistingId, "dummy-description");
    repository->updateExistingTitle(secondExistingId, "dummy-title-2");
    repository->updateExistingDescription(secondExistingId, "dummy-description-2");

    repository->deleteAll();

    auto draft = repository->getNew();
    EXPECT_FALSE(draft.has_value());
    draft = repository->getExisting(firstExistingId);
    EXPECT_FALSE(draft.has_value());
    draft = repository->getExisting(secondExistingId);
    EXPECT_FALSE(draft.has_value());
}

TEST_F(DraftsRepositoryImplTest, givenDraftsInDbWhenDeleteAllIsInvokedThenAllDraftsAreNotAccessibleAnymore) {
    auto newStmt = db->createStatement("INSERT INTO pending_draft_creation (id, title, description) VALUES (0, ?, ?)");
    newStmt->bind<std::string>(1, "dummy-title");
    newStmt->bind<std::string>(2, "dummy-description");
    newStmt->execute<void>();
    int firstExistingId = 45;
    int secondExistingId = 87;
    auto existingStmt =
        db->createStatement("INSERT INTO pending_drafts_update (rowid, title, description) VALUES (?, ?, ?)");
    existingStmt->bind<int>(1, 45);
    existingStmt->bind<std::string>(2, "dummy-title");
    existingStmt->bind<std::string>(3, "dummy-description");
    existingStmt->execute<void>();
    existingStmt->bind<int>(1, 87);
    existingStmt->bind<std::string>(2, "dummy-title-2");
    existingStmt->bind<std::string>(3, "dummy-description-2");
    existingStmt->execute<void>();

    repository->deleteAll();

    auto draft = repository->getNew();
    EXPECT_FALSE(draft.has_value());
    draft = repository->getExisting(firstExistingId);
    EXPECT_FALSE(draft.has_value());
    draft = repository->getExisting(secondExistingId);
    EXPECT_FALSE(draft.has_value());
}
