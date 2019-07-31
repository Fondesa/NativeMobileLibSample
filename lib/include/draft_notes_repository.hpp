#pragma once

#include <string>

class DraftNotesRepository {
   public:
    virtual void beginCreationSession() = 0;

    virtual void beginUpdateSession(int id) = 0;

    virtual void endSession() = 0;

    virtual void updateTitle(std::string title) = 0;

    virtual void updateDescription(std::string title) = 0;

    virtual void clearDrafts() = 0;

    virtual void removeDraftCreation() = 0;

    virtual void removeDraftUpdate(int id) = 0;
};
