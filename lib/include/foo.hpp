#pragma once

#include <string>

class Foo {
private:
    std::string value;

public:
    Foo(std::string value);
    
    std::string getValue() const;
};
