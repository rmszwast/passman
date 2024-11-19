#include "entry.hpp"

Entry::Entry(CLI::App* app)
  : Command(app) {};

void
Entry::setup()
{
    name = "entry";
    description = "Manage entries";
    app = app->add_subcommand(name, description)
            ->usage("passman entry COMMAND [OPTIONS]")
            ->require_subcommand();
};
