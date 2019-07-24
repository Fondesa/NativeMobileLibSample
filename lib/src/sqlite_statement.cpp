#include "sqlite_cursor.hpp"
#include "sqlite_statement.hpp"
#include "sqlite_exception.hpp"
#include <vector>

SQLiteStatement::SQLiteStatement(sqlite3 *db, std::string sql) {
    auto movedSql = std::move(sql);
    int rc = sqlite3_prepare_v2(db, movedSql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw SQLiteException(db);
    }
}

SQLiteStatement::~SQLiteStatement() {
    sqlite3_finalize(stmt);
}

int SQLiteStatement::step() {
    return sqlite3_step(stmt);
}

int SQLiteStatement::clearBindings() {
    return sqlite3_clear_bindings(stmt);
}

int SQLiteStatement::reset() {
    return sqlite3_reset(stmt);
}

/* TEMPLATES */

template<>
void SQLiteStatement::execute() {
    if (step() != SQLITE_DONE) {
        throw SQLiteException(stmt);
    }
    if (clearBindings() != SQLITE_OK) {
        throw SQLiteException(stmt);
    }
    if (reset() != SQLITE_OK) {
        throw SQLiteException(stmt);
    }
}

template<>
SQLiteCursor SQLiteStatement::execute() {
    return SQLiteCursor(stmt);
}

template<>
void SQLiteStatement::bind(int colIndex, int value) {
    int rc = sqlite3_bind_int(stmt, colIndex, value);
    if (rc != SQLITE_OK) {
        throw SQLiteException(stmt);
    }
}

template<>
void SQLiteStatement::bind(int colIndex, double value) {
    int rc = sqlite3_bind_double(stmt, colIndex, value);
    if (rc != SQLITE_OK) {
        throw SQLiteException(stmt);
    }
}

template<>
void SQLiteStatement::bind(int colIndex, std::string value) {
    auto movedValue = std::move(value);
    int rc = sqlite3_bind_text(stmt, colIndex, movedValue.c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        throw SQLiteException(stmt);
    }
}

template<>
void SQLiteStatement::bind(int colIndex, bool value) {
    int intValue = (value) ? 1 : 0;
    bind(colIndex, intValue);
}