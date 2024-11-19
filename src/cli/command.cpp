#include "command.hpp"
#include "create.hpp"
#include "entry.hpp"
#include "vault.hpp"

Command::Command(CLI::App* app)
  : app(app) {};

void
Commands::setupCommands(CLI::App* app)
{
    // Vault commands
    app->failure_message(CLI::FailureMessage::help);
    Vault vault = Vault(app);
    vault.setup();
    Create create = Create(vault.app);
    create.setup();

    // Entry commands
    Entry entry = Entry(app);
    entry.setup();
};
