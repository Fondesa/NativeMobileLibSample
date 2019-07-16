#include <iostream>
#include "foo.hpp"

int main() {
    std::unique_ptr<Foo> foo = std::make_unique<Foo>("A dummy string");
    std::cout << foo->getValue() << std::endl;
    return 0;
}
