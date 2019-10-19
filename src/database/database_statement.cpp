#include "database_statement.hpp"

namespace Db {

/* TEMPLATES */

template<>
void Statement::execute() {
    executeVoid();
}

template<>
stdx::optional<int> Statement::execute() {
    return executeOptionalInt();
}

template<>
int Statement::execute() {
    return executeInt();
}

template<>
stdx::optional<std::string> Statement::execute() {
    return executeOptionalString();
}

template<>
std::shared_ptr<Cursor> Statement::execute() {
    return executeCursor();
}

template<>
void Statement::bind(int colIndex, int value) {
    bindInt(colIndex, value);
}

template<>
void Statement::bind(int colIndex, double value) {
    bindDouble(colIndex, value);
}

template<>
void Statement::bind(int colIndex, std::string value) {
    bindString(colIndex, std::move(value));
}

template<>
void Statement::bind(int colIndex, bool value) {
    bindBool(colIndex, value);
}
}