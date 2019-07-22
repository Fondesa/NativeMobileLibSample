#pragma once

#include <string>
#include "sqlite3.h"

class SQLiteHandler {
   public:
    explicit SQLiteHandler(std::string dbPath);

    ~SQLiteHandler();

    void executeTransaction(std::function<void()> transact);

   private:
    sqlite3 *db{};
};

class SQLiteException : public std::exception {
   public:
    explicit SQLiteException(sqlite3 *db);
    explicit SQLiteException(sqlite3_stmt *stmt);

    const char *what() const noexcept override;

   private:
    const char *msg;
};