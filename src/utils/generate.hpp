#ifndef GENERATOR_H
#define GENERATOR_H

#include "constants.hpp"
#include <random>
#include <string>
#include <vector>

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
    // Separate setters for min/max requirements
    PasswordGenerator& setMinLength(int min);
    PasswordGenerator& setMaxLength(int max);

    // Separate setters for uppercase requirement
    PasswordGenerator& setRequireUppercase(bool value);
    PasswordGenerator& setNoUppercase(bool value);

    // Separate setters for lowercase requirement
    PasswordGenerator& setRequireLowercase(bool value);
    PasswordGenerator& setNoLowercase(bool value);

    // Separate setters for numbers requirement
    PasswordGenerator& setRequireNumbers(bool value);
    PasswordGenerator& setNoNumbers(bool value);

    // Separate setters for special characters requirement
    PasswordGenerator& setRequireSpecialChars(bool value);
    PasswordGenerator& setNoSpecialChars(bool value);

    // Setters for misc boolean settings
    PasswordGenerator& setNoDictionaryWords(bool value);
    PasswordGenerator& setNoConsecutiveRepeats(bool value);
    PasswordGenerator& setUseDiceware(bool value);

    PasswordGenerator& setDicewareCount(int ct);
    PasswordGenerator& setFullDiceware(bool value);

    // Main generate function to be used by other files
    std::string generate() const;
};

#endif // GENERATOR_H
