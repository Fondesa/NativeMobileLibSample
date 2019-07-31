#pragma once

#include <algorithm>
#include <iostream>
#include <vector>
#include "notes_repository.hpp"
#include "database.hpp"

class DatabaseNotesRepository : public NotesRepository {
   public:
    explicit DatabaseNotesRepository(std::shared_ptr<Db::Database> db);

    void insert(DraftNote note) override;

    void remove(int id) override;

    void update(int id, DraftNote note) override;

    void clear() override;

    std::vector<Note> getAll() override;

   private:
    std::shared_ptr<Db::Database> db;
};