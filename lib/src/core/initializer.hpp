#pragma once

#include <string>
#include "database.hpp"

namespace initializers {

void initializeDatabase(std::string path);

/* PRIVATE */ namespace {

void createSchema(const std::shared_ptr<Database> &db);
}
}