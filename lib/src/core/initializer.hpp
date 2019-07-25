#pragma once

#include <string>
#include "database.hpp"

namespace initializers {

const int dbVersion = 1;

void initializeDatabase(std::string path);

/* PRIVATE */ namespace {

void createSchema(const std::shared_ptr<Db::Database> &db);
}
}