#include "notes_interactor_impl.hpp"
#include "notes_repository_factory.hpp"
#include "drafts_repository_factory.hpp"
#include "notes_interactor_factory.hpp"

std::shared_ptr<NotesInteractor> NotesInteractorFactory::create() {
    auto notesRepository = NotesRepositoryFactory::create();
    auto draftsRepository = DraftsRepositoryFactory::create();
    return std::make_shared<NotesInteractorImpl>(notesRepository, draftsRepository);
}
