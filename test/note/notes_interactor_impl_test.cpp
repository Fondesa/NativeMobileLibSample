#include "mock/notes_repository_mock.hpp"
#include "mock/drafts_repository_mock.hpp"
#include "note/notes_interactor_impl.hpp"
#include "notes_interactor_impl_test.hpp"

using ::testing::AtLeast;
using ::testing::Return;

void NotesInteractorImplTest::SetUp() {
    notesRepository = std::make_shared<NotesRepositoryMock>();
    draftsRepository = std::make_shared<DraftsRepositoryMock>();
    interactor = std::make_shared<NotesInteractorImpl>(notesRepository, draftsRepository);
}

void NotesInteractorImplTest::TearDown() {
    interactor = nullptr;
    notesRepository = nullptr;
    draftsRepository = nullptr;
}

TEST_F(NotesInteractorImplTest, givenDraftWhenInsertNoteIsInvokedThenNoteIsInsertedInRepositoryAndDraftIsDeleted) {
    auto draft = Draft("dummy-title", "dummy-description");
    EXPECT_CALL(*notesRepository, insert(draft)).Times(1);
    EXPECT_CALL(*draftsRepository, deleteNew()).Times(1);

    interactor->insertNote(draft);
}

TEST_F(NotesInteractorImplTest,
       givenNoteIdAndDraftWhenUpdateNoteIsInvokedThenNoteIsUpdatedInRepositoryAndDraftIsDeleted) {
    int id = 3;
    auto draft = Draft("dummy-title", "dummy-description");
    EXPECT_CALL(*notesRepository, update(id, draft)).Times(1);
    EXPECT_CALL(*draftsRepository, deleteExisting(id)).Times(1);

    interactor->updateNote(id, draft);
}

TEST_F(NotesInteractorImplTest, whenGetAllNotesIsInvokedThenRepositoryReturnsAllNotes) {
    std::vector<Note> notes;
    notes.emplace_back(1, "first-title", "first-description");
    notes.emplace_back(2, "second-title", "second-description");
    EXPECT_CALL(*notesRepository, getAll()).Times(1).WillOnce(Return(notes));

    ASSERT_EQ(notes, interactor->getAllNotes());
}

TEST_F(NotesInteractorImplTest, givenTextFilterWhenGetNotesByTextIsInvokedThenRepositoryReturnsFilteredNotes) {
    std::string text = "filter";
    std::vector<Note> notes;
    notes.emplace_back(1, "first-title", "first-description");
    notes.emplace_back(2, "second-title", "second-description");
    EXPECT_CALL(*notesRepository, getByText(text)).Times(1).WillOnce(Return(notes));

    ASSERT_EQ(notes, interactor->getNotesByText(text));
}

TEST_F(NotesInteractorImplTest, whenGetNewDraftIsInvokedThenRepositoryReturnsNewDraft) {
    auto draft = Draft("dummy-title", "dummy-description");
    EXPECT_CALL(*draftsRepository, getNew()).Times(2)
        .WillOnce(Return(stdx::optional<Draft>(draft)))
        .WillOnce(Return(stdx::optional<Draft>()));

    EXPECT_EQ(draft, interactor->getNewDraft());
    EXPECT_FALSE(interactor->getNewDraft());
}

TEST_F(NotesInteractorImplTest, givenNoteIdWhenGetExistingDraftIsInvokedThenRepositoryReturnsExistingDraft) {
    int id = 3;
    auto draft = Draft("dummy-title", "dummy-description");
    EXPECT_CALL(*draftsRepository, getExisting(id)).Times(2)
        .WillOnce(Return(stdx::optional<Draft>(draft)))
        .WillOnce(Return(stdx::optional<Draft>()));

    EXPECT_EQ(draft, interactor->getExistingDraft(id));
    EXPECT_FALSE(interactor->getExistingDraft(id));
}

TEST_F(NotesInteractorImplTest, givenTitleWhenUpdateNewDraftTitleIsInvokedThenNewDraftChangesTitle) {
    std::string title = "dummy-title";
    EXPECT_CALL(*draftsRepository, updateNewTitle(title)).Times(1);

    interactor->updateNewDraftTitle(title);
}

TEST_F(NotesInteractorImplTest, givenDescriptionWhenUpdateNewDraftDescriptionIsInvokedThenNewDraftChangesDescription) {
    std::string description = "dummy-description";
    EXPECT_CALL(*draftsRepository, updateNewDescription(description)).Times(1);

    interactor->updateNewDraftDescription(description);
}

TEST_F(NotesInteractorImplTest,
       givenNoteIdAndTitleWhenUpdateExistingDraftTitleIsInvokedThenExistingDraftWithTheGivenIdChangesTitle) {
    int id = 3;
    std::string title = "dummy-title";
    EXPECT_CALL(*draftsRepository, updateExistingTitle(id, title)).Times(1);

    interactor->updateExistingDraftTitle(id, title);
}

TEST_F(NotesInteractorImplTest,
       givenNoteIdAndDescriptionWhenUpdateExistingDraftDescriptionIsInvokedThenExistingDraftWithTheGivenIdChangesDescription) {
    int id = 3;
    std::string description = "dummy-description";
    EXPECT_CALL(*draftsRepository, updateExistingDescription(id, description)).Times(1);

    interactor->updateExistingDraftDescription(id, description);
}

TEST_F(NotesInteractorImplTest, givenNoteIdWhenDeleteNoteIsInvokedThenRepositoryDeletesNoteAndDraft) {
    int id = 3;
    EXPECT_CALL(*notesRepository, deleteWithId(id)).Times(1);
    EXPECT_CALL(*draftsRepository, deleteExisting(id)).Times(1);

    interactor->deleteNote(id);
}

TEST_F(NotesInteractorImplTest, whenDeleteNewDraftIsInvokedThenRepositoryDeletesNewDraft) {
    EXPECT_CALL(*draftsRepository, deleteNew()).Times(1);

    interactor->deleteNewDraft();
}

TEST_F(NotesInteractorImplTest, givenNoteIdWhenDeleteExistingDraftIsInvokedThenRepositoryDeletesDraftWithId) {
    int id = 3;
    EXPECT_CALL(*draftsRepository, deleteExisting(id)).Times(1);

    interactor->deleteExistingDraft(id);
}

TEST_F(NotesInteractorImplTest, whenPersistChangesIsInvokedThenChangesArePersistedOnRepository) {
    EXPECT_CALL(*draftsRepository, persist()).Times(1);

    interactor->persistChanges();
}
