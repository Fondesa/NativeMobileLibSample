#include "database_client.hpp"
#include "notes_interactor_factory.hpp"
#include "notes_interactor_factory_test.hpp"
#include "note/notes_interactor_impl.hpp"

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
