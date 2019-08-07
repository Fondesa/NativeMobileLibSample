#pragma once

#include <memory>
#include "notes_repository.hpp"

class NotesRepositoryFactory {
   public:
    static std::shared_ptr<NotesRepository> create();
};