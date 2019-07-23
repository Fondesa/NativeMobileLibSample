#include "sqlite_cursor.hpp"
#include "sqlite_exception.hpp"

SQLiteCursor::SQLiteCursor(sqlite3_stmt *stmt) : stmt(stmt) {
    columnCount = sqlite3_column_count(stmt);
}

SQLiteCursor::~SQLiteCursor() noexcept(false) {
    if (sqlite3_clear_bindings(stmt) != SQLITE_OK) {
        throw SQLiteException(stmt);
    }
    if (sqlite3_reset(stmt) != SQLITE_OK) {
        throw SQLiteException(stmt);
    }
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

int SQLiteCursor::getInt(int colIndex) {
    ensureIndexInBounds(colIndex);

    return sqlite3_column_int(stmt, colIndex);
}

double SQLiteCursor::getDouble(int colIndex) {
    ensureIndexInBounds(colIndex);

    return sqlite3_column_double(stmt, colIndex);
}

std::string SQLiteCursor::getString(int colIndex) {
    ensureIndexInBounds(colIndex);

    auto text = sqlite3_column_text(stmt, colIndex);
    return std::string(reinterpret_cast<const char *>(text));
}

void SQLiteCursor::ensureIndexInBounds(int colIndex) {
    if (colIndex >= columnCount) {
        throw SQLiteException("The column index " +
            std::to_string(colIndex) +
            " should be less than " +
            std::to_string(columnCount));
    }
}

