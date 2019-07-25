#pragma once

#include <exception>
#include <string>

class IllegalStateException : public std::exception {
   public:
    explicit IllegalStateException(std::string msg);

    const char *what() const noexcept override;

   private:
    const char *msg;
};