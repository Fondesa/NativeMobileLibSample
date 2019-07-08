#include "foo.hpp"

Foo::Foo(std::string value) {
    this->value = std::move(value);
}

std::string Foo::getValue() const {
    return "pre_" + value + "_post";
}
