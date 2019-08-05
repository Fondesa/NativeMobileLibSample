#pragma once

#include <string>

class DraftNote {
   private:
    std::string title;
    std::string description;

   public:
    DraftNote(std::string title, std::string description);

    std::string getTitle() const;

    std::string getDescription() const;

    void setTitle(std::string title);

    void setDescription(std::string description);
};