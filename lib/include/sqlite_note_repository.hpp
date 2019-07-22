#pragma once

#include <algorithm>
#include <iostream>
#include <vector>
#include "note_repository.hpp"
#include "sqlite_database.hpp"

class SQLiteNoteRepository : public NoteRepository {
   public:
    SQLiteNoteRepository(const SQLiteDatabase &handler);

    void insert(DraftNote note) override;

    void remove(int id) override;

    void update(int id, DraftNote note) override;

    std::vector<Note> getAll() override;

   private:
    const SQLiteDatabase &handler;
    std::vector<Note> notes;
    int currentId = 0;
};