#include "database_client.hpp"
#include "notes_repository_factory_test.hpp"
#include "note/notes_repository_factory.hpp"
#include "note/notes_repository_impl.hpp"

void NotesRepositoryFactoryTest::SetUp() {
    Db::Client::create(":memory:");
}

void NotesRepositoryFactoryTest::TearDown() {
    Db::Client::release();
}

TEST_F(NotesRepositoryFactoryTest, whenCreateIsInvokedThenNotesRepositoryIsCreated) {
    auto repository = NotesRepositoryFactory::create();

    ASSERT_TRUE(repository != nullptr);
    EXPECT_TRUE(std::dynamic_pointer_cast<NotesRepositoryImpl>(repository));
}
