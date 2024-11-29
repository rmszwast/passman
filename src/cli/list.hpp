#ifndef LIST_H
#define LIST_H

#include "../utils/passGetter.hpp"
#include "../utils/vaultReader.hpp"
#include "command.hpp"

class List : public Command
{
  public:
    List(CLI::App* app);
    void setup();
};

void
listEntries(std::string path,
            Flags_t flags,
            VaultReaderInterface& vaultReader,
            PassGetterInterface& passGetter);

#endif