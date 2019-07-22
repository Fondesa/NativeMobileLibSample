#include "sqlite_exception.hpp"

SQLiteException::SQLiteException(sqlite3 *db) {
    msg = sqlite3_errmsg(db);
}

SQLiteException::SQLiteException(sqlite3_stmt *stmt) : SQLiteException(sqlite3_db_handle(stmt)) {}

const char *SQLiteException::what() const noexcept {
    return msg;
}