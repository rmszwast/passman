#include "vault.hpp"

Vault::Vault(CLI::App* app)
  : Command(app) {};

void
Vault::setup()
{
    name = "vault";
    description = "Manage vaults";
    app = app->add_subcommand(name, description)
            ->usage("passman vault COMMAND [OPTIONS]")
            ->require_subcommand();
};