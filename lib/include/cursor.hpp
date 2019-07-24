#pragma once

#pragma once

#include <string>
#include "sqlite3.h"

class Cursor {
   public:
    virtual bool next() = 0;

    template<typename T>
    T get(int colIndex);

   protected:
    virtual void ensureIndexInBounds(int colIndex) = 0;

    virtual int getInt(int colIndex) = 0;

    virtual double getDouble(int colIndex) = 0;

    virtual std::string getString(int colIndex) = 0;

    virtual bool getBool(int colIndex) = 0;
};