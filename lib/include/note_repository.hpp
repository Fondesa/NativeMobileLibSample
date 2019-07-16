#pragma once

#include "draft_note.hpp"
#include "note.hpp"

class NoteRepository {
   public:
    virtual void insert(const DraftNote &note) = 0;

    virtual void remove(int id) = 0;

    virtual void update(int id, const DraftNote &note) = 0;

    virtual std::vector<std::shared_ptr<Note>> getAll() = 0;
};
