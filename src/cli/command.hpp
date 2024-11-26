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

struct Entry_t
{
    std::string name;
    std::string username;
    std::string password;
    std::string URL;
    std::string notes;
};

struct Flags_t
{
    bool genBool;    // -g, --generate-password
    bool promptBool; // -p, --prompt-password
    bool showBool;   // -s --show-password
};

#endif