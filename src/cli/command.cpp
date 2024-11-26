#include "command.hpp"
#include "add.hpp"
#include "clip.hpp"
#include "create.hpp"
#include "entry.hpp"
#include "list.hpp"
#include "show.hpp"
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
    Add add = Add(entry.app);
    add.setup();
    // List list = List(entry.app);
    // list.setup();
    Show show = Show(entry.app);
    show.setup();
    Clip clip = Clip(entry.app);
    clip.setup();
};