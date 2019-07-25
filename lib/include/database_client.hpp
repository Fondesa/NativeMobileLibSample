#pragma once

#include <string>
#include "database.hpp"

namespace Db {

class DatabaseClient {
   public:
    static void create(std::string dbPath);

    static std::shared_ptr<Database> get();

   private:
    DatabaseClient() = default;

    static std::shared_ptr<Database> databaseInstance;
};
}