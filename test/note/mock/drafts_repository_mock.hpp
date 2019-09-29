#pragma once

#include <gmock/gmock.h>
#include "note/drafts_repository.hpp"

class DraftsRepositoryMock : public DraftsRepository {
   public:
    MOCK_METHOD(void, updateNewTitle, (std::string title), (override));

    MOCK_METHOD(void, updateNewDescription, (std::string description), (override));

    MOCK_METHOD(void, updateExistingTitle, (int id, std::string title), (override));

    MOCK_METHOD(void, updateExistingDescription, (int id, std::string description), (override));

    MOCK_METHOD(void, persist, (), (override));

    MOCK_METHOD(void, deleteAll, (), (override));

    MOCK_METHOD(void, deleteNew, (), (override));

    MOCK_METHOD(void, deleteExisting, (int id), (override));

    MOCK_METHOD(std::optional<Draft>, getNew, (), (override));

    MOCK_METHOD(std::optional<Draft>, getExisting, (int id), (override));
};
