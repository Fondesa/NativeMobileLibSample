#include "database_client.hpp"
#include "sqlite_database.hpp"
#include <iostream>
#include "core/illegal_state_exception.hpp"

void DatabaseClient::create(std::string dbPath) {
    if (databaseInstance != nullptr) {
        std::cout << "WARNING: the database is already created." << std::endl;
        return;
    }
    databaseInstance = std::make_shared<SQLiteDatabase>(dbPath);
}

std::shared_ptr<Database> DatabaseClient::get() {
    if (databaseInstance == nullptr) {
        throw IllegalStateException("The database should be created before.");
    }
    return databaseInstance;
}

std::shared_ptr<Database> DatabaseClient::databaseInstance;
