#include "sqlite_cursor.hpp"
#include "sqlite_statement.hpp"
#include "sqlite_exception.hpp"
#include <vector>

namespace Db::Sql {

Statement::Statement(sqlite3 *db, std::string sql) : db(db), stmt(db, sql) {}

void Statement::executeVoid() {
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        throw Db::Sql::Exception(db);
    }
    if (sqlite3_clear_bindings(stmt) != SQLITE_OK) {
        throw Db::Sql::Exception(db);
    }
    if (sqlite3_reset(stmt) != SQLITE_OK) {
        throw Db::Sql::Exception(db);
    }
}

std::optional<int> Statement::executeInt() {
    int status = sqlite3_step(stmt);
    auto result = std::optional<int>();
    if (status == SQLITE_DONE) {
        // If there are no rows to read, the value is absent.
        return result;
    }
    if (status != SQLITE_ROW) {
        throw Db::Sql::Exception(db);
    }
    result = sqlite3_column_int(stmt, 0);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        throw Db::Sql::Exception(db);
    }
    if (sqlite3_clear_bindings(stmt) != SQLITE_OK || sqlite3_reset(stmt) != SQLITE_OK) {
        throw Db::Sql::Exception(db);
    }
    return result;
}

std::optional<std::string> Statement::executeString() {
    int status = sqlite3_step(stmt);
    auto result = std::optional<std::string>();
    if (status == SQLITE_DONE) {
        // If there are no rows to read, the value is absent.
        return result;
    }
    if (status != SQLITE_ROW) {
        throw Db::Sql::Exception(db);
    }
    auto text = sqlite3_column_text(stmt, 0);
    result = std::string(reinterpret_cast<const char *>(text));
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        throw Db::Sql::Exception(db);
    }
    if (sqlite3_clear_bindings(stmt) != SQLITE_OK || sqlite3_reset(stmt) != SQLITE_OK) {
        throw Db::Sql::Exception(db);
    }
    return result;
}

std::shared_ptr<Db::Cursor> Statement::executeCursor() {
    return std::make_shared<Cursor>(db, stmt);
}

void Statement::bindInt(int colIndex, int value) {
    int rc = sqlite3_bind_int(stmt, colIndex, value);
    if (rc != SQLITE_OK) {
        throw Db::Sql::Exception(db);
    }
}

void Statement::bindDouble(int colIndex, double value) {
    int rc = sqlite3_bind_double(stmt, colIndex, value);
    if (rc != SQLITE_OK) {
        throw Db::Sql::Exception(db);
    }
}

void Statement::bindString(int colIndex, std::string value) {
    auto movedValue = std::move(value);
    int rc = sqlite3_bind_text(stmt, colIndex, movedValue.c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        throw Db::Sql::Exception(db);
    }

}

void Statement::bindBool(int colIndex, bool value) {
    int intValue = (value) ? 1 : 0;
    bindInt(colIndex, intValue);
}
}