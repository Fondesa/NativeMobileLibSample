#include "database_statement.hpp"

/* TEMPLATES */

template<>
void DatabaseStatement::execute() {
    executeVoid();
}

template<>
std::shared_ptr<DatabaseCursor> DatabaseStatement::execute() {
    return executeCursor();
}

template<>
void DatabaseStatement::bind(int colIndex, int value) {
    bindInt(colIndex, value);
}

template<>
void DatabaseStatement::bind(int colIndex, double value) {
    bindDouble(colIndex, value);
}

template<>
void DatabaseStatement::bind(int colIndex, std::string value) {
    bindString(colIndex, std::move(value));
}

template<>
void DatabaseStatement::bind(int colIndex, bool value) {
    bindBool(colIndex, value);
}
