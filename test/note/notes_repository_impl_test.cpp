#include "database_client.hpp"
#include "note_database_initializer.hpp"
#include "notes_repository_impl_test.hpp"

void NotesRepositoryImplTest::SetUp() {
    NoteDb::initialize(":memory:");
    db = Db::Client::get();
    repository = std::make_shared<NotesRepositoryImpl>(db);
}

void NotesRepositoryImplTest::TearDown() {
    repository = nullptr;
    db = nullptr;
    Db::Client::release();
}

int NotesRepositoryImplTest::getNotesCount() {
    return db->createStatement("SELECT COUNT(*) FROM notes")->execute<int>();
}

int NotesRepositoryImplTest::getLastRowId() {
    // It returns the rowid of the last inserted record.
    return db->createStatement("SELECT last_insert_rowid()")->execute<int>();
}

TEST_F(NotesRepositoryImplTest, givenDraftWhenInsertIsInvokedThenNoteIsInserted) {
    auto draft = Draft("dummy-title", "dummy-description");

    repository->insert(draft);

    auto stmt = db->createStatement("SELECT title, description FROM notes");
    auto cursor = stmt->execute<std::shared_ptr<Db::Cursor>>();
    // The DB should contain one record.
    ASSERT_TRUE(cursor->next());
    auto title = cursor->get<std::string>(0);
    auto description = cursor->get<std::string>(1);
    EXPECT_EQ(draft.getTitle(), title);
    EXPECT_EQ(draft.getDescription(), description);
    // Only one record should have been inserted.
    ASSERT_FALSE(cursor->next());
}

TEST_F(NotesRepositoryImplTest, givenExistentIdWhenDeleteWithIdIsInvokedThenNoteIsDeleted) {
    repository->insert(Draft("dummy-title", "dummy-description"));
    // Get the newly inserted row id.
    int rowId = getLastRowId();

    repository->deleteWithId(rowId);

    ASSERT_EQ(0, getNotesCount());
}

TEST_F(NotesRepositoryImplTest, givenInexistentIdWhenDeleteWithIdIsInvokedThenNoteIsNotDeleted) {
    repository->insert(Draft("dummy-title", "dummy-description"));
    // Get the newly inserted row id.
    int rowId = getLastRowId();

    // The id + 1 doesn't exist yet.
    repository->deleteWithId(rowId + 1);

    int recordCount = db->createStatement("SELECT COUNT(*) FROM notes")->execute<int>();
    ASSERT_EQ(1, getNotesCount());
}

TEST_F(NotesRepositoryImplTest, givenExistentIdWhenUpdateIsInvokedThenNoteIsUpdated) {
    auto expectedTitle = "updated-title";
    auto expectedDescription = "updated-description";
    repository->insert(Draft("dummy-title", "dummy-description"));
    // Get the newly inserted row id.
    int rowId = getLastRowId();

    repository->update(rowId, Draft(expectedTitle, expectedDescription));

    auto stmt = db->createStatement("SELECT rowid, title, description FROM notes");
    auto cursor = stmt->execute<std::shared_ptr<Db::Cursor>>();
    // The DB should contain one record.
    ASSERT_TRUE(cursor->next());
    auto id = cursor->get<int>(0);
    auto title = cursor->get<std::string>(1);
    auto description = cursor->get<std::string>(2);
    EXPECT_EQ(rowId, id);
    EXPECT_EQ(expectedTitle, title);
    EXPECT_EQ(expectedDescription, description);
    // The DB should still contain only one record after the update.
    ASSERT_FALSE(cursor->next());
}

TEST_F(NotesRepositoryImplTest, givenZeroInsertedNotesWhenDeleteAllIsInvokedThenNoNotesAreDeleted) {
    repository->deleteAll();

    ASSERT_EQ(0, getNotesCount());
}

TEST_F(NotesRepositoryImplTest, givenMultipleInsertedNotesWhenDeleteAllIsInvokedThenAllNotesAreDeleted) {
    repository->insert(Draft("dummy-title", "dummy-description"));
    repository->insert(Draft("dummy-title-2", "dummy-description-2"));

    repository->deleteAll();

    ASSERT_EQ(0, getNotesCount());
}

TEST_F(NotesRepositoryImplTest, givenZeroNotesWhenGetAllIsInvokedThenEmptyListIsReturned) {
    auto notes = repository->getAll();

    ASSERT_TRUE(notes.empty());
}

TEST_F(NotesRepositoryImplTest, givenMultipleNotesWhenGetAllIsInvokedThenListContainingAllNotesIsReturned) {
    auto firstDraft = Draft("dummy-title", "dummy-description");
    auto secondDraft = Draft("dummy-title-2", "dummy-description-2");
    repository->insert(firstDraft);
    int firstId = getLastRowId();
    repository->insert(secondDraft);
    int secondId = getLastRowId();

    auto notes = repository->getAll();

    ASSERT_EQ(2, notes.size());
    EXPECT_EQ(Note(firstId, firstDraft.getTitle(), firstDraft.getDescription()), notes[0]);
    EXPECT_EQ(Note(secondId, secondDraft.getTitle(), secondDraft.getDescription()), notes[1]);
}

TEST_F(NotesRepositoryImplTest, givenZeroNotesWhenGetByTextIsInvokedThenEmptyListIsReturned) {
    auto notes = repository->getByText("dummy-text");

    ASSERT_TRUE(notes.empty());
}

TEST_F(NotesRepositoryImplTest, givenAllNotesMatchingFilterWhenGetByTextIsInvokedThenAllNotesAreReturned) {
    auto firstDraft = Draft("first", "second");
    auto secondDraft = Draft("third", "fourth");
    repository->insert(firstDraft);
    int firstId = getLastRowId();
    repository->insert(secondDraft);
    int secondId = getLastRowId();

    auto notes = repository->getByText("o");

    ASSERT_EQ(2, notes.size());
    EXPECT_EQ(Note(firstId, firstDraft.getTitle(), firstDraft.getDescription()), notes[0]);
    EXPECT_EQ(Note(secondId, secondDraft.getTitle(), secondDraft.getDescription()), notes[1]);
}

TEST_F(NotesRepositoryImplTest, givenSomeNotesMatchingFilterWhenGetByTextIsInvokedThenOnlyMatchingNotesAreReturned) {
    auto firstDraft = Draft("first", "second");
    auto secondDraft = Draft("third", "fourth");
    repository->insert(firstDraft);
    repository->insert(secondDraft);
    int secondId = getLastRowId();

    auto notes = repository->getByText("urt");

    ASSERT_EQ(1, notes.size());
    EXPECT_EQ(Note(secondId, secondDraft.getTitle(), secondDraft.getDescription()), notes[0]);
}

TEST_F(NotesRepositoryImplTest, givenZeroNotesMatchingFilterWhenGetByTextIsInvokedThenEmptyListIsReturned) {
    auto firstDraft = Draft("first", "second");
    auto secondDraft = Draft("third", "fourth");
    repository->insert(firstDraft);
    repository->insert(secondDraft);

    auto notes = repository->getByText("fifth");

    ASSERT_TRUE(notes.empty());
}