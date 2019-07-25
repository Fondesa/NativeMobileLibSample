#pragma once

#include <exception>
#include <string>

class DatabaseException : public std::exception {
   public:
    explicit DatabaseException(std::string msg);

    const char *what() const noexcept override;

   private:
    std::string msg;
};