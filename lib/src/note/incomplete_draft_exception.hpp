#pragma once

#include <exception>
#include <string>
#include "mutable_draft.hpp"

class IncompleteDraftException : public std::exception {
   public:
    explicit IncompleteDraftException(const MutableDraft &draft);

    IncompleteDraftException(int id, const MutableDraft &draft);

    const char *what() const noexcept override;

   private:
    std::string msg;
};