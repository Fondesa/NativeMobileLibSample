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

#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"
    // We just ignore the lint error to avoid to cast both flags to unsigned.
    databaseInstance = std::make_shared<Sql::Database>(dbPath, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
#pragma clang diagnostic pop
}

std::shared_ptr<Database> Client::get() {
    if (databaseInstance == nullptr) {
        throw Exception("The database should be created before.");
    }
    return databaseInstance;
}

void Client::release() {
    if (databaseInstance != nullptr) {
        databaseInstance.reset();
    }
}

std::shared_ptr<Database> Client::databaseInstance;
}
