#include "database_client.hpp"
#include <iostream>
#include "database_exception.hpp"
#include "sqlite_database.hpp"
#include "core/exception_macros.hpp"

namespace Db {

void Client::create(std::string dbPath) {
    if (databaseInstance != nullptr) {
        std::cout << "WARNING: the database is already created." << std::endl;
        return;
    }
    // We just ignore the lint error to avoid to cast both flags to unsigned.
    databaseInstance = std::make_shared<Sql::Database>(dbPath, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
}

std::shared_ptr<Database> Client::get() {
    if (databaseInstance == nullptr) {
        THROW(Exception("The database should be created before."));
    }
    return databaseInstance;
}

void Client::release() {
    if (databaseInstance != nullptr) {
        databaseInstance.reset();
    }
}

std::shared_ptr<Database> Client::databaseInstance;
}  // namespace Db
