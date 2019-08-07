#pragma once

#include <memory>
#include "drafts_repository.hpp"

class DraftsRepositoryFactory {
   public:
    static std::shared_ptr<DraftsRepository> create();
};