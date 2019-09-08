#include "smart_c_statement.hpp"
#include "sqlite_exception.hpp"

namespace Db::Sql {

SmartCStatement::SmartCStatement(sqlite3 *db, std::string &query) :
    originalStmt(nullptr),
    refCount(nullptr) {

    int rc = sqlite3_prepare_v2(db, query.c_str(), static_cast<int>(query.size()), &originalStmt, nullptr);
    if (rc != SQLITE_OK) {
        throw Db::Sql::Exception(db);
    }
    // Initialize the reference counter of the sqlite3_stmt :
    // used to share the mStmtPtr between Statement and Column objects;
    // This is needed to enable Column objects to live longer than the Statement object it refers to.
    refCount = new unsigned int(1);
}

SmartCStatement::SmartCStatement(const SmartCStatement &other) :
    originalStmt(other.originalStmt),
    refCount(other.refCount) {
    // Increment the reference counter of the sqlite3_stmt,
    // asking not to finalize the sqlite3_stmt during the lifetime of the new object
    ++(*refCount);
}

SmartCStatement::SmartCStatement(SmartCStatement &&other) noexcept :
    originalStmt(other.originalStmt),
    refCount(other.refCount) {
    other.originalStmt = nullptr;
    other.refCount = nullptr;
}

SmartCStatement::~SmartCStatement() {
    if (refCount && --(*refCount) == 0) {
        // If count reaches zero, finalize the sqlite3_stmt, as no Statement nor Column object use it anymore.
        // No need to check the return code, as it is the same as the last statement evaluation.
        sqlite3_finalize(originalStmt);

        // and delete the reference counter
        delete refCount;
        refCount = nullptr;
        originalStmt = nullptr;
    }
}

SmartCStatement::operator sqlite3_stmt *() const {
    return originalStmt;
}
}