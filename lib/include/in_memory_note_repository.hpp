#pragma once

#include <algorithm>
#include <iostream>
#include <vector>
#include "note_repository.hpp"

class InMemoryNoteRepository : public NoteRepository {
   private:
    std::vector<Note> notes;

   public:
    void insert(const DraftNote &note) override;

    void remove(int id) override;

    void update(int id, const DraftNote &note) override;

    std::vector<Note> getAll() override;
};