#pragma once

#include <gtest/gtest.h>
#include "note/notes_repository_impl.hpp"
#include "database.hpp"

class NotesRepositoryImplTest : public ::testing::Test {
   protected:
    std::shared_ptr<NotesRepositoryImpl> repository;
    std::shared_ptr<Db::Database> db;

    void SetUp() override;

    void TearDown() override;

    int getNotesCount();

    int getLastRowId();
};