#pragma once

#include <string>
#include "time_format.hpp"

class Note {
   public:
    Note(int id, std::string title, std::string description, Time::Format::ISO_8601 lastUpdateDate);

    int getId() const;

    std::string getTitle() const;

    std::string getDescription() const;

    Time::Format::ISO_8601 getLastUpdateDate() const;

    friend bool operator==(const Note &first, const Note &second);

   private:
    int id;
    std::string title;
    std::string description;
    Time::Format::ISO_8601 lastUpdateDate;
};