#pragma once

#include <string>
#include "sqlite3/sqlite3.h"
#include "database_cursor.hpp"
#include "smart_c_statement.hpp"

namespace Db::Sql {

class Cursor : public Db::Cursor {
   public:
    Cursor(sqlite3 *db, const SmartCStatement &stmt);

    ~Cursor();

    bool next() override;

   protected:
    void ensureNextWasInvoked() override;

    void ensureIndexInBounds(int colIndex) override;

    int getInt(int colIndex) override;

    double getDouble(int colIndex) override;

    std::string getString(int colIndex) override;

    bool getBool(int colIndex) override;

   private:
    sqlite3 *db{};
    SmartCStatement stmt;
    int columnCount;
    bool hadNext;
};
}