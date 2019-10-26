#pragma once

#include <exception>
#include <string>
#include "core/include_macros.hpp"
#include "sqlite3/sqlite3.h"
#include AMALGAMATION(database_exception.hpp)

namespace Db::Sql {

class Exception : public Db::Exception {
   public:
    explicit Exception(std::string msg);
    explicit Exception(sqlite3 *db);
};
}