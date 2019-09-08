#pragma once

#include <string>
#include "sqlite3/sqlite3.h"
#include "database_statement.hpp"
#include "smart_c_statement.hpp"

namespace Db::Sql {

class Statement : public Db::Statement {
   public:
    Statement(sqlite3 *db, std::string sql);

   protected:
    void executeVoid() override;

    std::optional<int> executeInt() override;

    std::optional<std::string> executeString() override;

    std::shared_ptr<Db::Cursor> executeCursor() override;

    void bindInt(int colIndex, int value) override;

    void bindDouble(int colIndex, double value) override;

    void bindString(int colIndex, std::string value) override;

    void bindBool(int colIndex, bool value) override;

   private:
    SmartCStatement stmt;
};
}