#pragma once

#include <gtest/gtest.h>

class NoteDatabaseInitializerTest : public ::testing::Test {
   protected:
#if __cpp_inline_variables
    inline static const std::string testDbPath = "note_database_initializer_test.db";
#else
    static const std::string testDbPath;
#endif

    static void changeVersion(int version);

    void TearDown() override;
};