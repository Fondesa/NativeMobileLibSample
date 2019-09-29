#pragma once

#include <string>

class Draft {
   private:
    std::string title;
    std::string description;

   public:
    Draft(std::string title, std::string description);

    std::string getTitle() const;

    std::string getDescription() const;

    friend bool operator==(const Draft &first, const Draft &second);
};