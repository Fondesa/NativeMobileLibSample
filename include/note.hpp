#pragma once

#include <string>

class Note {
   private:
    int id;
    std::string title;
    std::string description;

   public:
    Note(int id, std::string title, std::string description);

    int getId() const;

    std::string getTitle() const;

    std::string getDescription() const;

    friend bool operator==(const Note &first, const Note &second);
};