#include "cli/command.hpp"
#include <CLI/CLI.hpp>
#include <string>

int
main(int argc, char** argv)
{
    CLI::App app;
    Commands::setupCommands(&app);
    CLI11_PARSE(app, argc, argv);

    return 0;
}