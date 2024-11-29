#ifndef EDIT_H
#define EDIT_H

#include "../utils/passGetter.hpp"
#include "../utils/vaultReaderWriter.hpp"
#include "command.hpp"

class Edit : public Command
{

  public:
    Edit(CLI::App* app);
    void setup();
};

void
editEntry(std::string path,
          Entry_t entry,
          Flags_t flags,
          VaultReaderWriterInterface& vaultReaderWriter,
          PassGetterInterface& passGetter);

#endif