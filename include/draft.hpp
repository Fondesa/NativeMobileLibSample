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

    void setTitle(std::string title);

    void setDescription(std::string description);
};