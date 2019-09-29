#pragma once

#include <gmock/gmock.h>
#include "note/notes_repository.hpp"

class NotesRepositoryMock : public NotesRepository {
   public:
    MOCK_METHOD(void, insert, (Draft note), (override));
    MOCK_METHOD(void, deleteWithId, (int id), (override));
    MOCK_METHOD(void, update, (int id, Draft note), (override));
    MOCK_METHOD(void, deleteAll, (), (override));
    MOCK_METHOD(std::vector<Note>, getAll, (), (override));
    MOCK_METHOD(std::vector<Note>, getByText, (std::string text), (override));
};
