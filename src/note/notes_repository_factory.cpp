#include "database_client.hpp"
#include "notes_repository_factory.hpp"
#include "notes_repository_impl.hpp"

std::shared_ptr<NotesRepository> NotesRepositoryFactory::create() {
    auto db = Db::Client::get();
    return std::make_shared<NotesRepositoryImpl>(db);
}
