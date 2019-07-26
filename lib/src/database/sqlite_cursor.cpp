#include "sqlite_cursor.hpp"
#include "sqlite_exception.hpp"

namespace Db::Sql {

Cursor::Cursor(sqlite3_stmt *stmt) : stmt(stmt) {
    columnCount = sqlite3_column_count(stmt);
}

Cursor::~Cursor() {
    sqlite3_clear_bindings(stmt);
    sqlite3_reset(stmt);
}

bool Cursor::next() {
    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        return false;
    }
    if (rc != SQLITE_ROW) {
        throw Db::Sql::Exception(stmt);
    }
    return true;
}

void Cursor::ensureIndexInBounds(int colIndex) {
    if (colIndex >= columnCount) {
        throw Db::Sql::Exception("The column index " +
            std::to_string(colIndex) +
            " should be less than " +
            std::to_string(columnCount));
    }
}

int Cursor::getInt(int colIndex) {
    return sqlite3_column_int(stmt, colIndex);
}

double Cursor::getDouble(int colIndex) {
    return sqlite3_column_double(stmt, colIndex);
}

std::string Cursor::getString(int colIndex) {
    auto text = sqlite3_column_text(stmt, colIndex);
    return std::string(reinterpret_cast<const char *>(text));
}

bool Cursor::getBool(int colIndex) {
    return getInt(colIndex) != 0;
}
}