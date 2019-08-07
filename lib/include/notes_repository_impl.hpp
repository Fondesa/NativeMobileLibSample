#pragma once

#include <vector>
#include "notes_repository.hpp"
#include "database.hpp"

class NotesRepositoryImpl : public NotesRepository {
   public:
    explicit NotesRepositoryImpl(std::shared_ptr<Db::Database> db);

    void insert(Draft note) override;

    void remove(int id) override;

    void update(int id, Draft note) override;

    void clear() override;

    std::vector<Note> getAll() override;

   private:
    std::shared_ptr<Db::Database> db;
};