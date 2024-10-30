#include "create.hpp"

Create::Create(CLI::App* app)
  : Command(app) {};

void
Create::setup()
{
    name = "create";
    description = "Create a vault";
    app = app->add_subcommand(name, description);
    app->usage("passman vault create [OPTIONS] PATH");
    app->add_option("path", "The path of the vault");
};