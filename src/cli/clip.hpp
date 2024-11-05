#ifndef PASSMAN_CLIP_H
#define PASSMAN_CLIP_H

#include "command.hpp"
#include <stdlib.h>
#include <string>
#include "vault.hpp"

class Clip : public Command
{
  public:
    Clip(CLI::App* app);
    void setup();
    int execute(Vault vault, int timeout);
}

#endif
