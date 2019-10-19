#include "smart_c_statement.hpp"
#include "sqlite_exception.hpp"
#include "core/exception_macros.hpp"

namespace Db::Sql {

SmartCStatement::SmartCStatement(sqlite3 *db, const std::string &query) :
    originalStmt(nullptr),
    refCount(nullptr) {

    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &originalStmt, nullptr);
    if (rc != SQLITE_OK) {
        THROW(Db::Sql::Exception(db));
    }
    if (!originalStmt) {
        // When the query is empty for example, SQLite returns SQLITE_OK without initializing the statement.
        THROW(Db::Sql::Exception("Can't generate any statement from the query \"" + query + "\"."));
    }
    // The reference count is 1 since we have to count the object which created this statement.
    refCount = new unsigned int(1);
}

SmartCStatement::SmartCStatement(const SmartCStatement &other) :
    originalStmt(other.originalStmt),
    refCount(other.refCount) {
    // Increment the reference counter of the sqlite3_stmt since it has been copied somewhere else.
    ++(*refCount);
}

SmartCStatement::~SmartCStatement() {
    if (refCount && --(*refCount) == 0) {
        // If the count reaches zero, finalize the sqlite3_stmt, as it's not owned by someone anymore.
        // Since we are in the destructor, there's no need to check the result code of this API because it won't be used.
        sqlite3_finalize(originalStmt);
        // Delete the reference counter.
        delete refCount;
        refCount = nullptr;
        originalStmt = nullptr;
    }
}

unsigned int SmartCStatement::useCount() {
    // We don't need to check the validity of the pointer since this method requires a valid instance of SmartCStatement
    // to be invoked and if SmartCStatement is initialized then refCount is initialized too.
    return *refCount;
}

SmartCStatement::operator sqlite3_stmt *() const {
    return originalStmt;
}
}