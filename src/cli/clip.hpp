#ifndef PASSMAN_CLIP_H
#define PASSMAN_CLIP_H
#include "../utils/error.hpp"
#include "../utils/passGetter.hpp"
#include "../utils/vaultReader.hpp"
#include "command.hpp"
#include <string>

class Clip : public Command
{
  public:
    Clip(CLI::App* app);
    void setup();
};

void
clipPassword(std::string path,
             Entry_t entry,
             Flags_t flags,
             VaultReaderInterface& vaultReader,
             PassGetterInterface& passGetter,
             int manualTimeout);

#endif
