#include <utility>

#include <database_client.hpp>
#include "initializer.hpp"

namespace initializers {

void initializeDatabase(std::string path) {
    // Create the database.
    DatabaseClient::create(std::move(path));
    // Obtain the database instance.
    auto db = DatabaseClient::get();
    // TODO: handle db version.
    // Create the database schema.
    createSchema(db);
}

/* PRIVATE */ namespace {

void createSchema(const std::shared_ptr<Database> &db) {
    // TODO: remove "IF NOT EXISTS"
    auto createTableStmt = db->createStatement("CREATE TABLE IF NOT EXISTS notes ("
                                               "title TEXT NOT NULL, "
                                               "description TEXT NOT NULL"
                                               ")");
    createTableStmt->execute<void>();
}
}
}
