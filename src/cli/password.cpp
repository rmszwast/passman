#include "password.hpp"
#include "../utils/generate.hpp"
#include <iostream>

namespace {
Flags_t flags; // Use the Flags_t struct to store all the flags
std::string password;
}

Password::Password(CLI::App* app)
  : Command(app) {};

void
Password::setup()
{
    name = "generate";
    description = "Password generation command";
    auto generateCmd = app->add_subcommand(name, description);

    addPartialPasswordFlags(generateCmd, &flags);

    generateCmd->callback([=]() {
        try {
            generatePassword(&password, &flags);
            std::cout << "Generated password: " << password << std::endl;
        } catch (const ConflictingFlagsException& e) {
            std::cerr << "Flag/Option Issue: " << e.what() << std::endl;
            return;
        } catch (const std::exception& e) {
            std::cerr << "Unexpected Error: " << e.what() << std::endl;
            return;
        }
    });
}
