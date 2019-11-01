#pragma once

#include <ctime>
#include <string>

namespace Time {

class Clock {
   public:
    virtual time_t currentTimeSeconds() = 0;
};
}
#include <string>
#include <functional>
#include <string>
#include <memory>
#include <string>

namespace Db {

class Cursor {
   public:
    virtual bool next() = 0;

    template<typename T>
    T get(int colIndex);

   protected:
    virtual void ensureNextWasInvoked() = 0;

    virtual void ensureIndexInBounds(int colIndex) = 0;

    virtual int getInt(int colIndex) = 0;

    virtual double getDouble(int colIndex) = 0;

    virtual std::string getString(int colIndex) = 0;

    virtual bool getBool(int colIndex) = 0;
};
}
#if __has_include(<optional>)
#include <optional>
namespace stdx = std;
#elif __has_include(<experimental/optional>)
#include <experimental/optional>
namespace stdx = std::experimental;
#else
#error Must have an optional type, either from <optional> or from <experimental/optional>.
#endif
namespace Db {

class Statement {
   public:
    template<typename T>
    void bind(int colIndex, T value);

    template<typename T>
    T execute();

   protected:
    virtual void executeVoid() = 0;

    virtual stdx::optional<int> executeOptionalInt() = 0;

    virtual int executeInt() = 0;

    virtual stdx::optional<std::string> executeOptionalString() = 0;

    virtual std::shared_ptr<Cursor> executeCursor() = 0;

    virtual void bindInt(int colIndex, int value) = 0;

    virtual void bindDouble(int colIndex, double value) = 0;

    virtual void bindString(int colIndex, std::string value) = 0;

    virtual void bindBool(int colIndex, bool value) = 0;
};
}
namespace Db {

class Database {
   public:
    virtual void executeTransaction(std::function<void()> transact) const = 0;

    virtual std::shared_ptr<Statement> createStatement(std::string sql) const = 0;
};
}
#include <string>


namespace Db {

class Client {
   public:
    static void create(std::string dbPath);

    static std::shared_ptr<Database> get();

    static void release();

   private:
    Client() = default;

    static std::shared_ptr<Database> databaseInstance;
};
}
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
#include <string>
#include <string>

namespace Time::Format {

typedef std::string ISO_8601;

ISO_8601 format(time_t time);
}
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
#include <string>


namespace NoteDb {

const int version = 2;

void initialize(std::string path);

  namespace {

void createSchema(const std::shared_ptr<Db::Database> &db);
}
}
#include <vector>




class NotesInteractor {
   public:
    virtual void insertNote(Draft note) = 0;

    virtual void updateNote(int id, Draft note) = 0;

    virtual void updateNewDraftTitle(std::string title) = 0;

    virtual void updateNewDraftDescription(std::string description) = 0;

    virtual void updateExistingDraftTitle(int id, std::string title) = 0;

    virtual void updateExistingDraftDescription(int id, std::string description) = 0;

    virtual std::vector<Note> getAllNotes() = 0;

    virtual std::vector<Note> getNotesByText(std::string text) = 0;

    virtual stdx::optional<Draft> getNewDraft() = 0;

    virtual stdx::optional<Draft> getExistingDraft(int id) = 0;

    virtual void deleteNote(int id) = 0;

    virtual void deleteNewDraft() = 0;

    virtual void deleteExistingDraft(int id) = 0;

    virtual void persistChanges() = 0;
};
#include <memory>


class NotesInteractorFactory {
   public:
    static std::shared_ptr<NotesInteractor> create();
};
