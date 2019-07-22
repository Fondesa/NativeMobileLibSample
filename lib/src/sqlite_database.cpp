#include "sqlite_database.hpp"
#include "sqlite_exception.hpp"
#include <iostream>
#include <stdexcept>

SQLiteHandler::SQLiteHandler(std::string dbPath) {
    auto path = std::move(dbPath);
    int canNotOpen = sqlite3_open(path.c_str(), &db);
    if (canNotOpen) {
        throw SQLiteException(db);
    }
    std::cout << "Opened database successfully" << std::endl;
}

SQLiteHandler::~SQLiteHandler() {
    sqlite3_close(db);
    std::cout << "Database closed" << std::endl;
}

void SQLiteHandler::executeTransaction(std::function<void()> transact) {
    auto transaction = std::move(transact);
    int rc = sqlite3_exec(db, "BEGIN TRANSACTION", nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        throw SQLiteException(db);
    }

    // Execute the transaction.
    transaction();

    rc = sqlite3_exec(db, "END TRANSACTION", nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        throw SQLiteException(db);
    }
}