#include <utility>

#include "sqlite_exception.hpp"

namespace Db {

SQLiteException::SQLiteException(std::string msg) : DatabaseException(std::move(msg)) {}

SQLiteException::SQLiteException(sqlite3 *db) : DatabaseException(sqlite3_errmsg(db)) {}

SQLiteException::SQLiteException(sqlite3_stmt *stmt) : SQLiteException(sqlite3_db_handle(stmt)) {}
}