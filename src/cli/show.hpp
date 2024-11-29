#ifndef SHOW_H
#define SHOW_H

#include "../utils/passGetter.hpp"
#include "../utils/vaultReader.hpp"
#include "command.hpp"

class Show : public Command
{
  public:
    Show(CLI::App* app);
    void setup();
};

void
showEntry(std::string path,
          Entry_t entry,
          Flags_t flags,
          VaultReaderInterface& vaultReader,
          PassGetterInterface& passGetter);

#endif