#pragma once

#include <string>
#include <optional>
#include <draft.hpp>

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

   private:
    std::optional<std::string> title;
    std::optional<std::string> description;
};