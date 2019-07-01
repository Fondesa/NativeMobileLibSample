#ifndef FOO_HPP
#define FOO_HPP

#include <string>

class Foo {
private:
    std::string value;

public:
    Foo(std::string value);
    
    std::string getValue() const;
};

#endif //FOO_HPP
