#include "database_client.hpp"
#include "database_exception.hpp"
#include "note_database_initializer.hpp"
#include "iostream"

namespace NoteDb {

void initialize(std::string path) {
    // Create the database.
    Db::Client::create(std::move(path));
    // Obtain the database instance.
    auto db = Db::Client::get();

    auto readVersionStmt = db->createStatement("PRAGMA user_version");
    auto currentVersion = readVersionStmt->execute<int>();
    if (version == currentVersion) {
        // The database didn't change its version.
        return;
    }

    if (version < currentVersion) {
        throw Db::Exception(std::string("Can't downgrade database from version ") +
            std::to_string(currentVersion) +
            " to version " +
            std::to_string(version));
    }

    db->executeTransaction([&] {
        if (currentVersion == 0) {
            std::cout << "Creating the database schema" << std::endl;
            // Create the database schema.
            createSchema(db);
        }

        auto writeVersionStmt = db->createStatement("PRAGMA user_version = " + std::to_string(version));
        writeVersionStmt->execute<void>();
    });
}

/* PRIVATE */ namespace {

/**
 * Creates the database schema.
 * This method runs in a database transaction.
 *
 * @param db the database instance used to create the statements.
 */
void createSchema(const std::shared_ptr<Db::Database> &db) {
    db->createStatement(
        "CREATE TABLE notes ("
        "title TEXT NOT NULL, "
        "description TEXT NOT NULL"
        ")"
    )->execute<void>();

    db->createStatement(
        "CREATE TABLE pending_drafts_update ("
        "title TEXT NOT NULL, "
        "description TEXT NOT NULL"
        ")"
    )->execute<void>();

    db->createStatement(
        "CREATE TABLE pending_draft_creation ("
        "id INTEGER PRIMARY KEY CHECK (id = 0), "
        "title TEXT NOT NULL, "
        "description TEXT NOT NULL"
        ")"
    )->execute<void>();
}
}
}
