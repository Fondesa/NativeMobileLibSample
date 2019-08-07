#pragma once

#include "draft.hpp"
#include "note.hpp"

class NotesRepository {
   public:
    virtual void insert(Draft note) = 0;

    virtual void remove(int id) = 0;

    virtual void update(int id, Draft note) = 0;

    virtual void clear() = 0;

    virtual std::vector<Note> getAll() = 0;
};
