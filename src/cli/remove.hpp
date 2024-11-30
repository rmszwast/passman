#ifndef REMOVE_H
#define REMOVE_H

#include "../utils/passGetter.hpp"
#include "../utils/vaultReaderWriter.hpp"
#include "command.hpp"

class Remove : public Command
{

  public:
    Remove(CLI::App* app);
    void setup();
};

void
removeEntry(std::string path,
            Entry_t entry,
            VaultReaderWriterInterface& vaultReaderWriter,
            PassGetterInterface& passGetter);

#endif