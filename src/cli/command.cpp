#include "command.hpp"
#include "create.hpp"
#include "remove.hpp"
#include "vault.hpp"

Command::Command(CLI::App* app)
  : app(app) {};

void
Commands::setupCommands(CLI::App* app)
{
    Vault vault = Vault(app);
    vault.setup();
    Create create = Create(vault.app);
    create.setup();
    Remove remove = Remove(vault.app);
    remove.setup();
};
