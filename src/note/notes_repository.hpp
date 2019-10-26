#pragma once

#include <string>
#include <vector>
#include "core/include_macros.hpp"
#include AMALGAMATION(draft.hpp)
#include AMALGAMATION(note.hpp)

class NotesRepository {
   public:
    virtual void insert(Draft note) = 0;

    virtual void deleteWithId(int id) = 0;

    virtual void update(int id, Draft note) = 0;

    virtual void deleteAll() = 0;

    virtual std::vector<Note> getAll() = 0;

    virtual std::vector<Note> getByText(std::string text) = 0;
};
