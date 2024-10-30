#include "remove.hpp"

Remove::Remove(CLI::App* app)
  : Command(app) {};

void
Remove::setup()
{
    name = "rm";
    description = "Remove a vault";
    app = app->add_subcommand(name, description);
    app->usage("passman vault rm [OPTIONS] PATH");
    app->add_option("path", "The path of the vault");
};