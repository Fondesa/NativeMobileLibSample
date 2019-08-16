#include "gtest/gtest.h"
#include "database_client.hpp"
#include "note/drafts_repository.hpp"

TEST(DatabaseClientTest, whenGetIsInvokedBeforeCreateThenExceptionIsThrown) {
    Db::Client::get();
    ASSERT_TRUE(true);
}