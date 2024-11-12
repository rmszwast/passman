#ifndef VAULT_H
#define VAULT_H

#include "command.hpp"

class Vault : public Command
{
  public:
    Vault(CLI::App* app);
    void setup();
};

#endif