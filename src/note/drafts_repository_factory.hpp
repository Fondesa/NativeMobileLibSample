#pragma once

#include <memory>
#include "core/include_macros.hpp"
#include "drafts_repository.hpp"

class DraftsRepositoryFactory {
   public:
    static std::shared_ptr<DraftsRepository> create();
};