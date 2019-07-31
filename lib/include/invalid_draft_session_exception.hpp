#pragma once

#include <exception>
#include <string>

class InvalidDraftSessionException : public std::exception {
   public:
    explicit InvalidDraftSessionException(std::string msg);

    const char *what() const noexcept override;

   private:
    std::string msg;
};