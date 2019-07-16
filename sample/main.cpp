#include <iostream>
#include "in_memory_note_repository.hpp"

void printNotes(std::vector<std::shared_ptr<Note>> notes);

int main() {
    auto repository = std::make_unique<InMemoryNoteRepository>();

    auto first = std::make_unique<DraftNote>("First title", "First description");
    repository->insert(*first);
    auto second = std::make_unique<DraftNote>("Second title", "Second description");
    repository->insert(*second);

    printNotes(repository->getAll());

    repository->remove(0);

    printNotes(repository->getAll());

    return 0;
}

void printNotes(std::vector<std::shared_ptr<Note>> notes) {
    std::cout << "There are the following notes:\n"
              << std::endl;
    for (auto it = notes.begin(); it != notes.end(); ++it) {
        auto note = *it;
        std::cout << "-\tTitle: "
                  << note->getTitle()
                  << std::endl
                  << "\tDescription: "
                  << note->getDescription()
                  << std::endl;
        if (it != notes.end() - 1) {
            std::cout << std::endl;
        }
    }
}