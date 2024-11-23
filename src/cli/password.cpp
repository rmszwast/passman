#include "password.hpp"
#include "../utils/generate.hpp"
#include <iostream>

namespace {
Flags_t flags; // Use the Flags_t struct to store all the flags
}

void
addPartialPasswordFlags(CLI::App* generateCmd, Flags_t* flags)
{
    generateCmd
      ->add_option("-m,--min", flags->minLength, "Minimum password length")
      ->default_val(18);
    generateCmd
      ->add_option("-x,--max", flags->maxLength, "Maximum password length")
      ->default_val(24);
    generateCmd->add_flag(
      "-c,--capital", flags->upperRequired, "Require uppercase letters");
    generateCmd->add_flag(
      "-l,--lower", flags->lowerRequired, "Require lowercase letters");
    generateCmd->add_flag("-n,--num", flags->numRequired, "Require numbers");
    generateCmd->add_flag(
      "-s,--special", flags->specialRequired, "Require special characters");
    generateCmd->add_flag(
      "-C,--no-capital", flags->upperNotAllowed, "Prohibit uppercase letters");
    generateCmd->add_flag(
      "-L,--no-lower", flags->lowerNotAllowed, "Prohibit lowercase letters");
    generateCmd->add_flag(
      "-N,--no-num", flags->numNotAllowed, "Prohibit numbers");
    generateCmd->add_flag("-S,--no-special",
                          flags->specialNotAllowed,
                          "Prohibit special characters");
    generateCmd->add_flag(
      "-w,--words", flags->noDictionaryWords, "Disallow dictionary words");
    generateCmd->add_flag(
      "-r,--repeat", flags->noRepeatCharacters, "Disallow repeated characters");
    generateCmd->add_flag(
      "-d,--diceware", flags->dicewarePassword, "Use diceware generation");
}

// Define the addGenerateFlagAndPasswordFlags function
void
addAllPasswordFlags(CLI::App* generateCmd, Flags_t* flags)
{
    // Set the -g / --generate flag to the genBool field of flags
    generateCmd->add_flag(
      "-g,--generate", flags->genBool, "Enable password generation");

    addPartialPasswordFlags(generateCmd, flags);
}

Password::Password(CLI::App* app)
  : Command(app) {};

void
Password::setup()
{
    name = "password";
    description = "Password generation command";
    app = app->add_subcommand(name, description);

    // Subcommand for password generation
    auto generateCmd = app->add_subcommand("generate", "Generate a password");
    addPartialPasswordFlags(generateCmd, &flags);

    generateCmd->callback([=]() {
        PasswordGenerator generator;

        // Directly pass the flags and the option counts
        try {
            std::string password =
              generator.generate(flags,
                                 generateCmd->get_option("-x")->count() > 0,
                                 generateCmd->get_option("-m")->count() > 0);

            std::cout << "Generated Password: " << password << std::endl;
        } catch (const std::runtime_error& e) {
            std::cerr << "Generating Error: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Unexpected: " << e.what() << std::endl;
        }
    });

    // Subcommand for Diceware password generation
    auto dicewareCmd =
      app->add_subcommand("diceware", "Generate a Diceware password");
    dicewareCmd
      ->add_option("-w,--word-count",
                   flags.dicewareLength,
                   "Number of words for Diceware password")
      ->default_val(6);

    dicewareCmd->callback([=]() {
        PasswordGenerator generator;

        // Generate Diceware password without the extra flags
        std::string dicewarePassword = generator.generate(flags, false, false);

        std::cout << "Generated Diceware Password: " << dicewarePassword
                  << std::endl;
    });
}
