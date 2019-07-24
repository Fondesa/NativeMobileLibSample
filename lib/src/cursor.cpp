#include "cursor.hpp"
#include "sqlite_exception.hpp"

/* TEMPLATES */

template<>
int Cursor::get(int colIndex) {
    ensureIndexInBounds(colIndex);

    return getInt(colIndex);
}

template<>
double Cursor::get(int colIndex) {
    ensureIndexInBounds(colIndex);

    return getDouble(colIndex);
}

template<>
std::string Cursor::get(int colIndex) {
    ensureIndexInBounds(colIndex);

    return getString(colIndex);
}

template<>
bool Cursor::get(int colIndex) {
    ensureIndexInBounds(colIndex);

    return getBool(colIndex);
}