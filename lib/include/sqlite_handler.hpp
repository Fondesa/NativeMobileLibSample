#pragma once

#include <string>
#include "sqlite3.h"

class SQLiteHandler {
   public:
    SQLiteHandler(std::string dbPath);

    ~SQLiteHandler();

   private:
    sqlite3 *db;
};

class SQLiteException : public std::exception {
   public:
    SQLiteException(sqlite3 *db);

    const char *what() const throw();

   private:
    const char *msg;
};