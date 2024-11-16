#ifndef COMMAND_H
#define COMMAND_H

#include <CLI/CLI.hpp>
#include <string>

class Command
{
  public:
    Command(CLI::App* app);
    CLI::App* app;
    std::string name;
    std::string description;
    virtual void setup() = 0;
};

namespace Commands {
void
setupCommands(CLI::App* app);
}

#endif