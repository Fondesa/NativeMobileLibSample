#include "sqlite_handler.hpp"
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

SQLiteException::SQLiteException(sqlite3 *db) {
    msg = sqlite3_errmsg(db);
}

const char *SQLiteException::what() const noexcept {
    return msg;
}