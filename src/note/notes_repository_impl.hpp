#pragma once

#include <vector>
#include "notes_repository.hpp"
#include "database.hpp"

class NotesRepositoryImpl : public NotesRepository {
   public:
    explicit NotesRepositoryImpl(std::shared_ptr<Db::Database> db);

    void insert(Draft note) override;

    void deleteWithId(int id) override;

    void update(int id, Draft note) override;

    void deleteAll() override;

    std::vector<Note> getAll() override;

    std::vector<Note> getByText(std::string text) override;

   private:
    std::shared_ptr<Db::Database> db;
};