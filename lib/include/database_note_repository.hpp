#pragma once

#include <algorithm>
#include <iostream>
#include <vector>
#include "note_repository.hpp"
#include "database.hpp"

class DatabaseNoteRepository : public NoteRepository {
   public:
    explicit DatabaseNoteRepository(std::shared_ptr<Database> db);

    void insert(DraftNote note) override;

    void remove(int id) override;

    void update(int id, DraftNote note) override;

    std::vector<Note> getAll() override;

   private:
    std::shared_ptr<Database> db;
};