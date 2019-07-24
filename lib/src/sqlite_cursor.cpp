#include "sqlite_cursor.hpp"
#include "sqlite_exception.hpp"

SQLiteCursor::SQLiteCursor(sqlite3_stmt *stmt) : stmt(stmt) {
    columnCount = sqlite3_column_count(stmt);
}

SQLiteCursor::~SQLiteCursor() {
    sqlite3_clear_bindings(stmt);
    sqlite3_reset(stmt);
}

bool SQLiteCursor::next() {
    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        return false;
    }
    if (rc != SQLITE_ROW) {
        throw SQLiteException(stmt);
    }
    return true;
}

void SQLiteCursor::ensureIndexInBounds(int colIndex) {
    if (colIndex >= columnCount) {
        throw SQLiteException("The column index " +
            std::to_string(colIndex) +
            " should be less than " +
            std::to_string(columnCount));
    }
}

int SQLiteCursor::getInt(int colIndex) {
    return sqlite3_column_int(stmt, colIndex);
}

double SQLiteCursor::getDouble(int colIndex) {
    return sqlite3_column_double(stmt, colIndex);
}

std::string SQLiteCursor::getString(int colIndex) {
    auto text = sqlite3_column_text(stmt, colIndex);
    return std::string(reinterpret_cast<const char *>(text));
}

bool SQLiteCursor::getBool(int colIndex) {
    return getInt(colIndex) != 0;
}