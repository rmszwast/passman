#ifndef ADD_H
#define ADD_H

#include "../utils/passGetter.hpp"
#include "../utils/vaultReaderWriter.hpp"
#include "command.hpp"

class Add : public Command
{
  public:
    Add(CLI::App* app);
    void setup();
};

void
addEntry(std::string path,
         Entry_t entry,
         Flags_t flags,
         VaultReaderWriterInterface& vaultReaderWriter,
         PassGetterInterface& passGetter);

#endif