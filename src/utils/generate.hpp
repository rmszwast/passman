#ifndef GENERATOR_H
#define GENERATOR_H

#include "../cli/command.hpp"
#include "constants.hpp"
#include <random>
#include <string>
#include <vector>

#include <exception>

// Represents an error occurring when there is a conflict/other issue w/ flags
class ConflictingFlagsException : public std::exception
{
    std::string message;

  public:
    explicit ConflictingFlagsException(const std::string& msg)
      : message(msg)
    {
    }

    const char* what() const noexcept override { return message.c_str(); }
};

class PasswordGenerator
{
  private:
    // All data members as settings for generating with their default
    int minLength = 18;
    int maxLength = 24;
    bool requireUppercase = false;
    bool noUppercase = false;
    bool requireLowercase = false;
    bool noLowercase = false;
    bool requireNumbers = false;
    bool noNumbers = false;
    bool requireSpecialChars = false;
    bool noSpecialChars = false;
    bool noDictionaryWords = false;
    bool noConsecutiveRepeats = false;
    bool useDiceware = false;

    bool fullDiceware = false;
    int dicewareWordCount = 5;

    // Internal helper functions:

    // Used to throw an error if there are any conflicting requirements
    void validate() const;

    // Core function that creates the string password by externally available
    // generate
    std::string create() const;

    // Other helpers used internally by the generate function
    std::vector<std::vector<std::string>> readWordsFromFile() const;
    std::string getSpacer(std::mt19937& gen) const;
    std::string getRandomStringOfLength(std::mt19937& gen,
                                        const std::string& str,
                                        int len,
                                        char lastChar) const;
    std::string getRandomStringOfLengthNoDict(std::mt19937& gen,
                                              const std::string& str1,
                                              const std::string& str2,
                                              int len,
                                              char lastChar) const;
    int getFinalLength(std::mt19937& gen) const;

    std::string dicewareOnly() const;

  public:
    // Main generate function to be used by other files
    std::string generate(const Flags_t& flags, bool maxUsed, bool minUsed);
};

// Below, standalone functions to be used by actual commands
void
addPartialPasswordFlags(CLI::App* generateCmd, Flags_t* flags);
void
addPartialDicewareFlags(CLI::App* generateCmd, Flags_t* flags);
void
addAllPasswordFlags(CLI::App* generateCmd, Flags_t* flags);
void
addAllDicewareFlags(CLI::App* generateCmd, Flags_t* flags);
void
generatePassword(CLI::App* printCmd, std::string* outPassword, Flags_t* flags);
void
generateDiceware(std::string* outPassword, Flags_t* flags);

void
addPasswordAndDicewareFlags(CLI::App* generateCmd, Flags_t* flags);

#endif // GENERATOR_H
