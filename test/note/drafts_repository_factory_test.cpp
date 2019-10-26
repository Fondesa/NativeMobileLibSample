#include "core/include_macros.hpp"
#include "drafts_repository_factory_test.hpp"
#include "note/drafts_repository_factory.hpp"
#include "note/drafts_repository_impl.hpp"
#include AMALGAMATION(database_client.hpp)

void DraftsRepositoryFactoryTest::SetUp() {
    Db::Client::create(":memory:");
}

void DraftsRepositoryFactoryTest::TearDown() {
    Db::Client::release();
}

TEST_F(DraftsRepositoryFactoryTest, whenCreateIsInvokedThenDraftsRepositoryIsCreated) {
    auto repository = DraftsRepositoryFactory::create();

    ASSERT_TRUE(repository != nullptr);
    EXPECT_TRUE(std::dynamic_pointer_cast<DraftsRepositoryImpl>(repository));
}
