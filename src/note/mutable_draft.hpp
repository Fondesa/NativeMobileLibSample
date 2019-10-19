#pragma once

#include <string>
#include <draft.hpp>
#include "std_optional_compat.hpp"

class MutableDraft {
   public:
    std::string requireTitle() const;

    std::string requireDescription() const;

    void updateTitle(std::string title);

    void updateDescription(std::string description);

    bool hasTitle() const;

    bool hasDescription() const;

    bool isIncomplete() const;

    Draft toDraft();

    friend bool operator==(const MutableDraft &first, const MutableDraft &second);

   private:
    stdx::optional<std::string> title;
    stdx::optional<std::string> description;
};