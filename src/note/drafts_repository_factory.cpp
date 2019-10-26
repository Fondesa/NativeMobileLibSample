#include "drafts_repository_factory.hpp"
#include "drafts_repository_impl.hpp"
#include "core/include_macros.hpp"
#include AMALGAMATION(database_client.hpp)

std::shared_ptr<DraftsRepository> DraftsRepositoryFactory::create() {
    auto db = Db::Client::get();
    return std::make_shared<DraftsRepositoryImpl>(db);
}
