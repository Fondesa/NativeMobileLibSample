#include "sqlite_cursor.hpp"
#include "sqlite_statement.hpp"
#include "sqlite_exception.hpp"
#include <vector>

namespace Db::Sql {

Statement::Statement(sqlite3 *db, std::string sql) {
    auto movedSql = std::move(sql);
    int rc = sqlite3_prepare_v2(db, movedSql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw Db::Sql::Exception(db);
    }
}

Statement::~Statement() {
    sqlite3_finalize(stmt);
}

void Statement::executeVoid() {
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        throw Db::Sql::Exception(stmt);
    }
    if (sqlite3_clear_bindings(stmt) != SQLITE_OK) {
        throw Db::Sql::Exception(stmt);
    }
    if (sqlite3_reset(stmt) != SQLITE_OK) {
        throw Db::Sql::Exception(stmt);
    }
}

std::shared_ptr<Db::Cursor> Statement::executeCursor() {
    return std::make_shared<Cursor>(stmt);
}

void Statement::bindInt(int colIndex, int value) {
    int rc = sqlite3_bind_int(stmt, colIndex, value);
    if (rc != SQLITE_OK) {
        throw Db::Sql::Exception(stmt);
    }
}

void Statement::bindDouble(int colIndex, double value) {
    int rc = sqlite3_bind_double(stmt, colIndex, value);
    if (rc != SQLITE_OK) {
        throw Db::Sql::Exception(stmt);
    }

}

void Statement::bindString(int colIndex, std::string value) {
    auto movedValue = std::move(value);
    int rc = sqlite3_bind_text(stmt, colIndex, movedValue.c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        throw Db::Sql::Exception(stmt);
    }

}

void Statement::bindBool(int colIndex, bool value) {
    int intValue = (value) ? 1 : 0;
    bindInt(colIndex, intValue);
}
}