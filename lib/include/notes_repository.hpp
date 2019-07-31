#pragma once

#include "draft_note.hpp"
#include "note.hpp"

class NotesRepository {
   public:
    virtual void insert(DraftNote note) = 0;

    virtual void remove(int id) = 0;

    virtual void update(int id, DraftNote note) = 0;

    virtual void clear() = 0;

    virtual std::vector<Note> getAll() = 0;
};
