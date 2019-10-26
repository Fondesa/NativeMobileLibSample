#include "sqlite_statement.hpp"
#include <vector>
#include "sqlite_cursor.hpp"
#include "sqlite_exception.hpp"
#include "core/exception_macros.hpp"

namespace Db::Sql {

Statement::Statement(sqlite3 *db, std::string sql) : db(db), stmt(db, sql) {}

void Statement::executeVoid() {
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        THROW(Db::Sql::Exception(db));
    }
    if (clearBindings() != SQLITE_OK || reset() != SQLITE_OK) {
        THROW(Db::Sql::Exception(db));
    }
}

stdx::optional<int> Statement::executeOptionalInt() {
    int status = sqlite3_step(stmt);
    auto result = stdx::optional<int>();
    if (status == SQLITE_DONE) {
        // If there are no rows to read, the value is absent.
        return result;
    }
    if (status != SQLITE_ROW) {
        THROW(Db::Sql::Exception(db));
    }
    int columnType = sqlite3_column_type(stmt, 0);
    if (columnType != SQLITE_INTEGER) {
        THROW(Db::Sql::Exception(
                  "The result should be of type " +
                      std::to_string(SQLITE_INTEGER) +
                      " instead of " +
                      std::to_string(columnType)));
    }
    result = sqlite3_column_int(stmt, 0);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        THROW(Db::Sql::Exception(db));
    }
    if (clearBindings() != SQLITE_OK || reset() != SQLITE_OK) {
        THROW(Db::Sql::Exception(db));
    }
    return result;
}

int Statement::executeInt() {
    auto result = executeOptionalInt();
    if (!result) {
        THROW(Db::Sql::Exception("The statement didn't return any row."));
    }
    return *result;
}

stdx::optional<std::string> Statement::executeOptionalString() {
    int status = sqlite3_step(stmt);
    auto result = stdx::optional<std::string>();
    if (status == SQLITE_DONE) {
        // If there are no rows to read, the value is absent.
        return result;
    }
    if (status != SQLITE_ROW) {
        THROW(Db::Sql::Exception(db));
    }
    int columnType = sqlite3_column_type(stmt, 0);
    if (columnType != SQLITE_TEXT) {
        THROW(Db::Sql::Exception(
                  "The result should be of type " +
                      std::to_string(SQLITE_TEXT) +
                      " instead of " +
                      std::to_string(columnType)));
    }
    auto text = sqlite3_column_text(stmt, 0);
    result = std::string(reinterpret_cast<const char *>(text));
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        THROW(Db::Sql::Exception(db));
    }
    if (clearBindings() != SQLITE_OK || reset() != SQLITE_OK) {
        THROW(Db::Sql::Exception(db));
    }
    return result;
} // LCOV_EXCL_BR_LINE

std::shared_ptr<Db::Cursor> Statement::executeCursor() {
    return std::make_shared<Cursor>(db, stmt);
}

void Statement::bindInt(int colIndex, int value) {
    int rc = sqlite3_bind_int(stmt, colIndex, value);
    if (rc != SQLITE_OK) {
        THROW(Db::Sql::Exception(db));
    }
}

void Statement::bindDouble(int colIndex, double value) {
    int rc = sqlite3_bind_double(stmt, colIndex, value);
    if (rc != SQLITE_OK) {
        THROW(Db::Sql::Exception(db));
    }
}

void Statement::bindString(int colIndex, std::string value) {
    auto movedValue = std::move(value);
    int rc = sqlite3_bind_text(stmt, colIndex, movedValue.c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        THROW(Db::Sql::Exception(db));
    }
}

void Statement::bindBool(int colIndex, bool value) {
    int intValue = (value) ? 1 : 0;
    bindInt(colIndex, intValue);
}

int Statement::reset() {
    return sqlite3_reset(stmt);
}

int Statement::clearBindings() {
    return sqlite3_clear_bindings(stmt);
}
}  // namespace Db::Sql