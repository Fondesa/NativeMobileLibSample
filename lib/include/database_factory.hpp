#pragma once

#include <string>
#include "database.hpp"

class DatabaseFactory {
   public:
    static std::shared_ptr<Database> createDatabase(std::string dbPath);
};