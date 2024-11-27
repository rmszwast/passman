#include "diceware.hpp"
#include "../utils/generate.hpp"
#include <iostream>

namespace {
Flags_t flags; // Use the Flags_t struct to store all the flags
std::string phrase;
}

Diceware::Diceware(CLI::App* app)
  : Command(app) {};

void
Diceware::setup()
{
    name = "diceware";
    description = "Diceware passphrase generation command";
    auto dicewareCmd = app->add_subcommand(name, description);

    addPartialDicewareFlags(dicewareCmd, &flags);

    dicewareCmd->callback([=]() {
        flags.dicewareOnly = true;
        try {
            generateDiceware(&phrase, &flags);
            std::cout << "Generated diceware passphrase: " << phrase
                      << std::endl;
        } catch (const ConflictingFlagsException& e) {
            std::cerr << "Flag/Option Issue: " << e.what() << std::endl;
            return;
        } catch (const std::exception& e) {
            std::cerr << "Unexpected Error: " << e.what() << std::endl;
            return;
        }
    });
}
