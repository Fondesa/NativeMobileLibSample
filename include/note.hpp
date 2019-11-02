#pragma once

#include <string>
#include <ctime>

class Note {
   public:
    Note(int id, std::string title, std::string description, std::time_t lastUpdateDate);

    [[nodiscard]] int getId() const;

    [[nodiscard]] std::string getTitle() const;

    [[nodiscard]] std::string getDescription() const;

    [[nodiscard]] std::time_t getLastUpdateTime() const;

    friend bool operator==(const Note &first, const Note &second);

   private:
    int id;
    std::string title;
    std::string description;
    std::time_t lastUpdateDate;
};