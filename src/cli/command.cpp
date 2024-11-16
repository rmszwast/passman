#include "command.hpp"
#include "create.hpp"
#include "vault.hpp"

Command::Command(CLI::App* app)
  : app(app) {};

void
Commands::setupCommands(CLI::App* app)
{
    app->failure_message(CLI::FailureMessage::help);
    Vault vault = Vault(app);
    vault.setup();
    Create create = Create(vault.app);
    create.setup();
};
