#pragma once

#include <string>
#include "database_statement.hpp"

class Database {
   public:
    virtual void executeTransaction(std::function<void()> transact) const = 0;

    virtual std::shared_ptr<DatabaseStatement> createStatement(std::string sql) const = 0;
};