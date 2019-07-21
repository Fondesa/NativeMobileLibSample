#include "sqlite_handler.hpp"
#include <iostream>
#include <stdexcept>

SQLiteHandler::SQLiteHandler(std::string dbPath) {
    int canNotOpen = sqlite3_open(dbPath.c_str(), &db);
    if (canNotOpen) {
        throw std::invalid_argument("Can't open database.");
    }
    std::cout << "Opened database successfully" << std::endl;
}

SQLiteHandler::~SQLiteHandler() {
    sqlite3_close(db);
    std::cout << "Database closed" << std::endl;
}