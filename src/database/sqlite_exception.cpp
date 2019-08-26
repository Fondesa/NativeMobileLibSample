#include <utility>

#include "sqlite_exception.hpp"

namespace Db::Sql {

Exception::Exception(std::string msg) : Db::Exception(std::move(msg)) {}

Exception::Exception(sqlite3 *db) : Db::Sql::Exception(sqlite3_errmsg(db)) {}

Exception::Exception(sqlite3_stmt *stmt) : Db::Sql::Exception(sqlite3_db_handle(stmt)) {}
}