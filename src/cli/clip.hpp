#ifndef PASSMAN_CLIP_H
#define PASSMAN_CLIP_H

#include "command.hpp"
#include <stdlib.h>
#include <string>

class Clip : public Command
{
  public:
    Clip(CLI::App* app);
    int executeWithVault(Vault vault, CommandLineParser parser) void setup();
}

#endif
