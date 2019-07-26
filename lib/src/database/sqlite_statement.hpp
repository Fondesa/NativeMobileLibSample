#pragma once

#include <string>
#include "sqlite3/sqlite3.h"
#include "database_statement.hpp"

namespace Db::Sql {

class Statement : public Db::Statement {
   public:
    Statement(sqlite3 *db, std::string sql);

    ~Statement();

   protected:
    void executeVoid() override;

    int executeInt() override;

    std::shared_ptr<Db::Cursor> executeCursor() override;

    void bindInt(int colIndex, int value) override;

    void bindDouble(int colIndex, double value) override;

    void bindString(int colIndex, std::string value) override;

    void bindBool(int colIndex, bool value) override;

   private:
    sqlite3_stmt *stmt{};
};
}