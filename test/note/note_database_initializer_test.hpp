#pragma once

#include <gtest/gtest.h>

class NoteDatabaseInitializerTest : public ::testing::Test {
   protected:
    inline static const std::string testDbPath = "note_database_initializer_test.db";

    static void changeVersion(int version);

    void TearDown() override;
};