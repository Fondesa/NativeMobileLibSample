#include "database_client.hpp"
#include "sqlite_database.hpp"
#include "database_exception.hpp"
#include <iostream>

namespace Db {

void Client::create(std::string dbPath) {
    if (databaseInstance != nullptr) {
        std::cout << "WARNING: the database is already created." << std::endl;
        return;
    }
    databaseInstance = std::make_shared<Sql::Database>(dbPath);
}

std::shared_ptr<Database> Client::get() {
    if (databaseInstance == nullptr) {
        throw Exception("The database should be created before.");
    }
    return databaseInstance;
}

std::shared_ptr<Database> Client::databaseInstance;
}
