#include <utility>
#include "sqlite_exception.hpp"

namespace Db::Sql {

Exception::Exception(std::string msg) : Db::Exception(std::move(msg)) {}

Exception::Exception(sqlite3 *db) : Db::Sql::Exception(sqlite3_errmsg(db)) {}
}