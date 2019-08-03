#pragma once

#include <optional>
#include "draft_notes_repository.hpp"
#include "database.hpp"

class InMemToDbDraftNotesRepository : public DraftNotesRepository {
   public:
    explicit InMemToDbDraftNotesRepository(std::shared_ptr<Db::Database> db);

    void beginCreationSession() override;

    void beginUpdateSession(Note note) override;

    void endSession() override;

    void updateTitle(std::string title) override;

    void updateDescription(std::string description) override;

    void clearDrafts() override;

    void removeDraftCreation() override;

    void removeDraftUpdate(int id) override;

    std::optional<DraftNote> getDraftCreationNote() override;
    std::optional<DraftNote> getDraftUpdateNote(int id) override;

   private:
    std::shared_ptr<Db::Database> db;
    std::string &draftTitle;
    std::string &draftDescription;
    std::string _title;
    std::string _description;
    std::optional<Note> noteInUpdate;
    bool isInSession;

    void updateNoteInDb();

    void createNoteInDb();
};