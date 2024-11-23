#include "example.hpp"
#include "../utils/generate.hpp"
#include "password.hpp"
#include <iostream>

namespace {
Flags_t flags; // Use the Flags_t struct to store all the flags
}

Example::Example(CLI::App* app)
  : Command(app) {};

void
Example::setup()
{
    name = "example";
    description = "Outputs 'example'";
    app = app->add_subcommand(name, description);

    auto printCmd = app->add_subcommand("print", "Outputs 'example'");
    addAllPasswordFlags(printCmd, &flags);

    printCmd->callback([=]() {
        std::cout << "example" << std::endl;

        if (flags.genBool) {
            PasswordGenerator generator;
            try {
                std::string password =
                  generator.generate(flags,
                                     printCmd->get_option("-x")->count() > 0,
                                     printCmd->get_option("-m")->count() > 0);

                std::cout << "Generated Password: " << password << std::endl;
            } catch (const std::runtime_error& e) {
                std::cerr << "Generating Error: " << e.what() << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "Unexpected: " << e.what() << std::endl;
            }
        }
    });
}
