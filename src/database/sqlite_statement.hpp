#pragma once

#include <string>
#include "core/include_macros.hpp"
#include "smart_c_statement.hpp"
#include "sqlite3/sqlite3.h"
#include AMALGAMATION(database_statement.hpp)

namespace Db::Sql {

class Statement : public Db::Statement {
   public:
    Statement(sqlite3 *db, std::string sql);

   protected:
    void executeVoid() override;

    stdx::optional<int> executeOptionalInt() override;

    int executeInt() override;

    stdx::optional<std::string> executeOptionalString() override;

    std::shared_ptr<Db::Cursor> executeCursor() override;

    void bindInt(int colIndex, int value) override;

    void bindDouble(int colIndex, double value) override;

    void bindString(int colIndex, std::string value) override;

    void bindBool(int colIndex, bool value) override;

    virtual int reset();

    virtual int clearBindings();

   private:
    sqlite3 *db{};
    SmartCStatement stmt;
};
}  // namespace Db::Sql