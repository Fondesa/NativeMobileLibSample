#pragma once

#include <exception>
#include "sqlite3.h"

class SQLiteException : public std::exception {
   public:
    explicit SQLiteException(sqlite3 *db);
    explicit SQLiteException(sqlite3_stmt *stmt);

    const char *what() const noexcept override;

   private:
    const char *msg;
};