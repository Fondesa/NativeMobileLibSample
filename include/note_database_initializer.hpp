#pragma once

#include <string>
#include "database.hpp"

namespace NoteDb {

const int version = 2;

void initialize(std::string path);

/* PRIVATE */ namespace {

void createSchema(const std::shared_ptr<Db::Database> &db);
}
}