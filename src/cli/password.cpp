#include "password.hpp"
#include "../utils/generate.hpp"
#include <iostream>

namespace {
int minLength{ 18 }, maxLength{ 24 };
bool upperRequired{ false }, lowerRequired{ false }, numRequired{ false },
  specialRequired{ false };
bool upperNotAllowed{ false }, lowerNotAllowed{ false }, numNotAllowed{ false },
  specialNotAllowed{ false };
bool noDictionaryWords{ false }, noRepeatCharacters{ false }, diceware{ false };
int dicewareWordCount{ 6 }; // Default word count for Diceware
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
    generateCmd->add_option("-m,--min", minLength, "Minimum password length")
      ->default_val(18);
    generateCmd->add_option("-x,--max", maxLength, "Maximum password length")
      ->default_val(24);
    generateCmd->add_flag(
      "-c,--capital", upperRequired, "Require uppercase letters");
    generateCmd->add_flag(
      "-l,--lower", lowerRequired, "Require lowercase letters");
    generateCmd->add_flag("-n,--num", numRequired, "Require numbers");
    generateCmd->add_flag(
      "-s,--special", specialRequired, "Require special characters");
    generateCmd->add_flag(
      "-C,--no-capital", upperNotAllowed, "Prohibit uppercase letters");
    generateCmd->add_flag(
      "-L,--no-lower", lowerNotAllowed, "Prohibit lowercase letters");
    generateCmd->add_flag("-N,--no-num", numNotAllowed, "Prohibit numbers");
    generateCmd->add_flag(
      "-S,--no-special", specialNotAllowed, "Prohibit special characters");
    generateCmd->add_flag(
      "-w,--words", noDictionaryWords, "Disallow dictionary words");
    generateCmd->add_flag(
      "-r,--repeat", noRepeatCharacters, "Disallow repeated characters");
    generateCmd->add_flag("-d,--diceware", diceware, "Use diceware generation");

    generateCmd->callback([=]() {
        // Adjust lengths based on input flags
        if (generateCmd->get_option("-m")->count() > 0 &&
            generateCmd->get_option("-x")->count() == 0) {
            maxLength = minLength + 5; // Set max to min + 5
        }
        if (generateCmd->get_option("-x")->count() > 0 &&
            generateCmd->get_option("-m")->count() == 0) {
            if (maxLength < 5) {
                minLength = maxLength;
            } else {
                minLength = std::max(maxLength - 2, 1);
            }
        }

        PasswordGenerator generator;
        generator.setMinLength(minLength)
          .setMaxLength(maxLength)
          .setRequireUppercase(upperRequired)
          .setRequireLowercase(lowerRequired)
          .setRequireNumbers(numRequired)
          .setRequireSpecialChars(specialRequired)
          .setNoUppercase(upperNotAllowed)
          .setNoLowercase(lowerNotAllowed)
          .setNoNumbers(numNotAllowed)
          .setNoSpecialChars(specialNotAllowed)
          .setNoDictionaryWords(noDictionaryWords)
          .setNoConsecutiveRepeats(noRepeatCharacters)
          .setUseDiceware(diceware);

        std::cout << "Generated Password: " << generator.generate()
                  << std::endl;
    });

    // Subcommand for Diceware password generation
    auto dicewareCmd =
      app->add_subcommand("diceware", "Generate a Diceware password");
    dicewareCmd
      ->add_option("-w,--word-count",
                   dicewareWordCount,
                   "Number of words for Diceware password")
      ->default_val(6);

    dicewareCmd->callback([=]() {
        PasswordGenerator generator;
        generator.setFullDiceware(true).setDicewareCount(dicewareWordCount);
        std::cout << "Generated Diceware Password: " << generator.generate()
                  << std::endl;
    });
}
