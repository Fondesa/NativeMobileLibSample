#include <utility>

#include <database_client.hpp>
#include "database_exception.hpp"
#include "initializer.hpp"
#include "iostream"

namespace initializers {

void initializeDatabase(std::string path) {
    // Create the database.
    Db::DatabaseClient::create(std::move(path));
    // Obtain the database instance.
    auto db = Db::DatabaseClient::get();

    auto readVersionStmt = db->createStatement("PRAGMA user_version");
    auto readVersionCursor = readVersionStmt->execute<std::shared_ptr<Db::DatabaseCursor>>();

    int currentVersion = 0;
    if (readVersionCursor->next()) {
        currentVersion = readVersionCursor->get<int>(0);
    }

    if (dbVersion == currentVersion) {
        // The database didn't change its version.
        return;
    }

    if (dbVersion < currentVersion) {
        throw Db::DatabaseException(std::string("Can't downgrade database from version ") +
            std::to_string(currentVersion) +
            " to version " +
            std::to_string(dbVersion));
    }

    if (currentVersion == 0) {
        std::cout << "Creating the database schema" << std::endl;
        // Create the database schema.
        createSchema(db);
    } else if (dbVersion > currentVersion) {
        std::cout << "Updating the schema from version "
                  << std::to_string(currentVersion)
                  << " to version "
                  << std::to_string(dbVersion)
                  << std::endl;
        // TODO: update schema
    }

    auto writeVersionStmt = db->createStatement("PRAGMA user_version = " + std::to_string(dbVersion));
    writeVersionStmt->execute<void>();
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
