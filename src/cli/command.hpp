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

struct Flags_t
{
    bool genBool = false;    // -g, --generate-password
    bool promptBool = false; // -p, --prompt-password
    bool showBool = false;   // -s, --show-password
    bool upperRequired = false;
    bool lowerRequired = false;
    bool numRequired = false;
    bool specialRequired = false;
    bool upperNotAllowed = false;
    bool lowerNotAllowed = false;
    bool numNotAllowed = false;
    bool specialNotAllowed = false;
    bool noDictionaryWords = false;
    bool noRepeatCharacters = false;
    bool dicewarePassword = false;
    bool dicewareOnly = false;
    int length = 18;
    int dicewareLength = 5;
};

#endif