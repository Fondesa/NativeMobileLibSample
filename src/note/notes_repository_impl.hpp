#pragma once

#include "notes_repository.hpp"
#include "database.hpp"
#include "clock.hpp"

class NotesRepositoryImpl : public NotesRepository {
   public:
    NotesRepositoryImpl(std::shared_ptr<Db::Database> db, std::shared_ptr<Time::Clock> clock);

    void insert(Draft note) override;

    void deleteWithId(int id) override;

    void update(int id, Draft note) override;

    void deleteAll() override;

    std::vector<Note> getAll() override;

    std::vector<Note> getByText(std::string text) override;

   private:
    std::shared_ptr<Db::Database> db;
    std::shared_ptr<Time::Clock> clock;
};