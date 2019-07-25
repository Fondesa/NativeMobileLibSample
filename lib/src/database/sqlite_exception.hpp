#pragma once

#include <exception>
#include <string>
#include "sqlite3/sqlite3.h"
#include "database_exception.hpp"

namespace Db {

class SQLiteException : public DatabaseException {
   public:
    explicit SQLiteException(std::string msg);
    explicit SQLiteException(sqlite3 *db);
    explicit SQLiteException(sqlite3_stmt *stmt);
};
}