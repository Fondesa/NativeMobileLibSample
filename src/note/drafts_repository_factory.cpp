#include "drafts_repository_factory.hpp"
#include "drafts_repository_impl.hpp"
#include "database_client.hpp"

std::shared_ptr<DraftsRepository> DraftsRepositoryFactory::create() {
    auto db = Db::Client::get();
    return std::make_shared<DraftsRepositoryImpl>(db);
}
