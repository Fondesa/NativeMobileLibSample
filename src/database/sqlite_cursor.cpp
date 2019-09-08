#include "sqlite_cursor.hpp"
#include "sqlite_exception.hpp"

namespace Db::Sql {

Cursor::Cursor(sqlite3 *db, const SmartCStatement &stmt) : db(db), stmt(stmt) {
    columnCount = sqlite3_column_count(stmt);
    hadNext = false;
}

bool Cursor::next() {
    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        if (clearBindings() != SQLITE_OK || reset() != SQLITE_OK) {
            throw Db::Sql::Exception(db);
        }
        return hadNext = false;
    }
    if (rc != SQLITE_ROW) {
        hadNext = false;
        throw Db::Sql::Exception(db);
    }

    return hadNext = true;
}

void Cursor::ensureNextWasInvoked() {
    if (!hadNext) {
        throw Db::Sql::Exception("The next() method returned false so it's " +
            std::string("not possible to get the value of the columns"));
    }
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
    int columnType = sqlite3_column_type(stmt, colIndex);
    if (columnType != SQLITE_INTEGER) {
        throw Db::Sql::Exception("The column at index " +
            std::to_string(colIndex) +
            " should be of type " +
            std::to_string(SQLITE_INTEGER) +
            " instead of " +
            std::to_string(columnType));
    }
    return sqlite3_column_int(stmt, colIndex);
}

double Cursor::getDouble(int colIndex) {
    int columnType = sqlite3_column_type(stmt, colIndex);
    if (columnType != SQLITE_FLOAT) {
        throw Db::Sql::Exception("The column at index " +
            std::to_string(colIndex) +
            " should be of type " +
            std::to_string(SQLITE_FLOAT) +
            " instead of " +
            std::to_string(columnType));
    }
    return sqlite3_column_double(stmt, colIndex);
}

std::string Cursor::getString(int colIndex) {
    int columnType = sqlite3_column_type(stmt, colIndex);
    if (columnType != SQLITE_TEXT) {
        throw Db::Sql::Exception("The column at index " +
            std::to_string(colIndex) +
            " should be of type " +
            std::to_string(SQLITE_TEXT) +
            " instead of " +
            std::to_string(columnType));
    }
    auto text = sqlite3_column_text(stmt, colIndex);
    return std::string(reinterpret_cast<const char *>(text));
}

bool Cursor::getBool(int colIndex) {
    return getInt(colIndex) != 0;
}
int Cursor::reset() {
    return sqlite3_reset(stmt);
}

int Cursor::clearBindings() {
    return sqlite3_clear_bindings(stmt);
}
}