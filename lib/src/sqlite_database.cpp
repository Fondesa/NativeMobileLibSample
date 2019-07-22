#include "sqlite_database.hpp"
#include "sqlite_exception.hpp"
#include <iostream>
#include <stdexcept>

SQLiteDatabase::SQLiteDatabase(std::string dbPath) {
    auto movedPath = std::move(dbPath);
    int canNotOpen = sqlite3_open(movedPath.c_str(), &db);
    if (canNotOpen) {
        throw SQLiteException(db);
    }
    std::cout << "Opened database successfully" << std::endl;
}

SQLiteDatabase::~SQLiteDatabase() {
    sqlite3_close(db);
    std::cout << "Database closed" << std::endl;
}

void SQLiteDatabase::executeTransaction(std::function<void()> transact) {
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

SQLiteStatement SQLiteDatabase::createStatement(std::string sql) {
    auto movedSql = std::move(sql);
    return SQLiteStatement(db, movedSql);
}
