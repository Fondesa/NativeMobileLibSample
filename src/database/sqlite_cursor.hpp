#pragma once

#include <string>
#include "sqlite3/sqlite3.h"
#include "database_cursor.hpp"
#include "smart_c_statement.hpp"

namespace Db::Sql {

class Cursor : public Db::Cursor {
   public:
    explicit Cursor(const SmartCStatement& stmt);

    ~Cursor();

    bool next() override;

   protected:
    void ensureIndexInBounds(int colIndex) override;

    int getInt(int colIndex) override;

    double getDouble(int colIndex) override;

    std::string getString(int colIndex) override;

    bool getBool(int colIndex) override;

   private:
    SmartCStatement stmt;
    int columnCount;
};
}