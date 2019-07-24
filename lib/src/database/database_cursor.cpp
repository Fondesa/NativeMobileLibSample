#include "database_cursor.hpp"
#include "sqlite_exception.hpp"

/* TEMPLATES */

template<>
int DatabaseCursor::get(int colIndex) {
    ensureIndexInBounds(colIndex);

    return getInt(colIndex);
}

template<>
double DatabaseCursor::get(int colIndex) {
    ensureIndexInBounds(colIndex);

    return getDouble(colIndex);
}

template<>
std::string DatabaseCursor::get(int colIndex) {
    ensureIndexInBounds(colIndex);

    return getString(colIndex);
}

template<>
bool DatabaseCursor::get(int colIndex) {
    ensureIndexInBounds(colIndex);

    return getBool(colIndex);
}