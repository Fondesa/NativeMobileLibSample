#include "database_factory.hpp"
#include "sqlite_database.hpp"

std::shared_ptr<Database> DatabaseFactory::createDatabase(std::string dbPath) {
    return std::make_unique<SQLiteDatabase>(dbPath);
}
