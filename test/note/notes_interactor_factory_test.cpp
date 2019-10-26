#include "core/include_macros.hpp"
#include "notes_interactor_factory_test.hpp"
#include "note/notes_interactor_impl.hpp"
#include AMALGAMATION(database_client.hpp)
#include AMALGAMATION(notes_interactor_factory.hpp)

void NotesInteractorFactoryTest::SetUp() {
    Db::Client::create(":memory:");
}

void NotesInteractorFactoryTest::TearDown() {
    Db::Client::release();
}

TEST_F(NotesInteractorFactoryTest, whenCreateIsInvokedThenNotesInteractorIsCreated) {
    auto repository = NotesInteractorFactory::create();

    ASSERT_TRUE(repository != nullptr);
    EXPECT_TRUE(std::dynamic_pointer_cast<NotesInteractorImpl>(repository));
}
