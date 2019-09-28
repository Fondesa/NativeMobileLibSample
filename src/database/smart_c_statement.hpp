#pragma once

#include <string>
#include "sqlite3/sqlite3.h"

namespace Db::Sql {

/**
 * This class is used to share a sqlite3_stmt pointer between different objects and to finalize it when it's
 * no longer owned by someone.
 */
class SmartCStatement {
   public:
    /**
     * The main constructor.
     * Prepares the sqlite3_stmt of the given query and initializes its reference counter.
     *
     * @param db the pointer to the sqlite3 database which prepares the statement from the given query.
     * @param query the query used to create the statement.
     */
    SmartCStatement(sqlite3 *db, const std::string &query);

    /**
     * The copy constructor.
     *
     * @param other the other instance of this class which will be copied.
     */
    SmartCStatement(const SmartCStatement &other);

    /**
     * The destructor.
     * It decrements the reference counter and finalizes the sqlite3_stmt when it reaches 0.
     */
    ~SmartCStatement();

    /**
     * Gets the number of the {@link SmartCStatement} objects referring to the same sqlite3_stmt*.
     *
     * @return the number of the owners of this object.
     */
    unsigned int useCount();

    /**
     * The cast operator returns the pointer to sqlite3_stmt.
     * This is useful to use this type directly with the SQLite C api.
     */
    operator sqlite3_stmt *() const;

   private:
    // Points to the original statement.
    sqlite3_stmt *originalStmt;
    // Points to the heap allocated reference counter of the sqlite3_stmt.
    // It's used to make the owning object to live longer than the object which originally created it.
    // e.g. The Db::Sql::Cursor should live longer than the Db::Sql::Statement which created it.
    unsigned int *refCount;

    // The forbidden assignment operator.
    // Making it private avoids any assignments.
    SmartCStatement &operator=(const SmartCStatement &other);
};
}  // namespace Db::Sql