#include "statement.hpp"

/* TEMPLATES */

template<>
void Statement::execute() {
    executeVoid();
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
