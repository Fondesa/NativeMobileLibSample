#pragma once

#include <string>

class Draft {
   private:
    std::string title;
    std::string description;

   public:
    Draft(std::string title, std::string description);

    [[nodiscard]] std::string getTitle() const;

    [[nodiscard]] std::string getDescription() const;

    friend bool operator==(const Draft &first, const Draft &second);
};