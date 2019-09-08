#pragma once

#include "sqlite3/sqlite3.h"
#include <string>

namespace Db::Sql {

class SmartCStatement {
   public:
    // Prepare the statement and initialize its reference counter
    SmartCStatement(sqlite3 *db, const std::string &query);
    // Copy constructor increments the ref counter
    SmartCStatement(const SmartCStatement &other);
    // Decrement the ref counter and finalize the sqlite3_stmt when it reaches 0
    ~SmartCStatement();

#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-explicit-constructor"
    /// Inline cast operator returning the pointer to SQLite Statement Object
    operator sqlite3_stmt *() const;
#pragma clang diagnostic pop

   private:
    /// @{ Unused/forbidden copy/assignment operator
    SmartCStatement &operator=(const SmartCStatement &other);
    /// @}

   private:
    sqlite3_stmt *originalStmt;      //!< Pointer to SQLite Statement Object
    unsigned int *refCount;  //!< Pointer to the heap allocated reference counter of the sqlite3_stmt
    //!< (to share it with Column objects)

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedStructInspection"
    // Expose the private variables to the following test method.
    // This is not a good approach generally but otherwise we can't simulate errors using sqlite3_step().
    friend class SQLiteCursorTest_givenErrorInSqliteStepWhenNextIsInvokedThenExceptionIsThrown_Test;
#pragma clang diagnostic pop
};
}