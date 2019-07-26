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
    auto readVersionCursor = readVersionStmt->execute<std::shared_ptr<Db::Cursor>>();

    int currentVersion = 0;
    if (readVersionCursor->next()) {
        currentVersion = readVersionCursor->get<int>(0);
    }

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
        } else if (version > currentVersion) {
            std::cout << "Updating the schema from version "
                      << std::to_string(currentVersion)
                      << " to version "
                      << std::to_string(version)
                      << std::endl;
            // TODO: update schema
        }

        auto writeVersionStmt = db->createStatement("PRAGMA user_version = " + std::to_string(version));
        writeVersionStmt->execute<void>();
    });
}

/* PRIVATE */ namespace {

void createSchema(const std::shared_ptr<Db::Database> &db) {
    auto createTableStmt = db->createStatement("CREATE TABLE notes ("
                                               "title TEXT NOT NULL, "
                                               "description TEXT NOT NULL"
                                               ")");
    createTableStmt->execute<void>();
}
}
}
