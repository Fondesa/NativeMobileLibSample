#pragma once

#include <gtest/gtest.h>
#include "note/notes_interactor_impl.hpp"
#include "mock/notes_repository_mock.hpp"
#include "mock/drafts_repository_mock.hpp"

class NotesInteractorImplTest : public ::testing::Test {
   protected:
    std::shared_ptr<NotesInteractorImpl> interactor;
    std::shared_ptr<NotesRepositoryMock> notesRepository;
    std::shared_ptr<DraftsRepositoryMock> draftsRepository;

    void SetUp() override;

    void TearDown() override;
};