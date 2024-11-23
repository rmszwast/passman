#include "generate.hpp"
#include "../cli/command.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Error handling for conflicting or impossible requirements, including detailed
// message
void
PasswordGenerator::validate() const
{
    if (minLength < 0) {
        throw std::runtime_error("ERROR: Minimum length must be at least 0.");
    }

    if (maxLength < 1) {
        throw std::runtime_error("ERROR: Max length must be at least 1.");
    }

    if (minLength > maxLength) {
        throw std::runtime_error(
          "ERROR: Minimum length is greater than maximum length.");
    }

    if (useDiceware && (noDictionaryWords || noConsecutiveRepeats)) {
        throw std::runtime_error(
          "ERROR: Invalid use of diceware with specified constraints.");
    }

    if (useDiceware && noUppercase && noLowercase) {
        throw std::runtime_error("ERROR: Must allow either uppercase or "
                                 "lowercase letters when using diceware.");
    }

    if ((requireUppercase && noUppercase) ||
        (requireLowercase && noLowercase) || (requireNumbers && noNumbers) ||
        (requireSpecialChars && noSpecialChars)) {
        throw std::runtime_error(
          "ERROR: Conflicting requirements for character types.");
    }

    if (noUppercase && noLowercase && noNumbers && noSpecialChars) {
        throw std::runtime_error(
          "ERROR: At least one character type must be allowed.");
    }

    if (noNumbers && noSpecialChars && noDictionaryWords) {
        throw std::runtime_error("ERROR: Cannot have no numbers, special "
                                 "characters, and no dictionary words.");
    }

    if (useDiceware && maxLength < 5) {
        throw std::runtime_error(
          "ERROR: Maximum length must be at least 5 when using diceware.");
    }

    int requirementsMet = 0;
    if (requireUppercase)
        requirementsMet++;
    if (requireLowercase)
        requirementsMet++;
    if (requireNumbers)
        requirementsMet++;
    if (requireSpecialChars)
        requirementsMet++;

    if (requirementsMet == 4 && maxLength < 4) {
        throw std::runtime_error("ERROR: Max length must be at least 4 with "
                                 "all character requirements.");
    }

    if (requirementsMet == 3 && maxLength < 3) {
        throw std::runtime_error("ERROR: Max length must be at least 3 with "
                                 "three character requirements.");
    }

    if (requirementsMet == 2 && maxLength < 2) {
        throw std::runtime_error("ERROR: Max length must be at least 2 with "
                                 "two character requirements.");
    }
}

// Helper function to efficiently convert word to full uppercase/capital
std::string
toUpperCase(const std::string& input)
{
    std::string result = input;
    std::transform(result.begin(),
                   result.end(),
                   result.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return result;
}

// Helper function to efficiently convert word to full lowercase
std::string
toLowerCase(const std::string& input)
{
    std::string result = input;
    std::transform(result.begin(),
                   result.end(),
                   result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

// Reads txt file of words for diceware
std::vector<std::vector<std::string>>
PasswordGenerator::readWordsFromFile() const
{
    std::vector<std::vector<std::string>> wordLists(
      5); // Initialize with 5 lists
    std::ifstream file("src/utils/words.txt");

    if (!file.is_open()) {
        std::cerr << "Error opening file src/words.txt." << std::endl;
        return wordLists; // Return empty vectors if file can't be opened
    }

    std::string line;
    int listIndex = 0;

    while (std::getline(file, line)) {
        if (line == "____") {
            listIndex++; // Move to the next list on delimiter
            if (listIndex >= 5)
                break; // Stop if more than 5 lists are found
        } else {
            wordLists[listIndex].push_back(line); // Add word to current list
        }
    }

    file.close();
    return wordLists;
}

// Randomly generates a word length from 2 -> the highest possible length, based
// on concentration of words at each length
int
getArrayForLength(int count, std::mt19937& gen)
{
    int sum = 0;

    // Calculate the sum of counts up to the provided count
    if (count >= 2)
        sum += Constants::TwoCount;
    if (count >= 3)
        sum += Constants::ThreeCount;
    if (count >= 4)
        sum += Constants::FourCount;
    if (count >= 5)
        sum += Constants::FiveCount;
    if (count >= 6)
        sum += Constants::SixCount;

    std::uniform_int_distribution<int> dist(0, sum - 1);
    int randomNum = dist(gen);

    // Determine which count it falls into
    if (randomNum < Constants::TwoCount) {
        return 2;
    } else if (randomNum < Constants::TwoCount + Constants::ThreeCount) {
        return 3;
    } else if (randomNum < Constants::TwoCount + Constants::ThreeCount +
                             Constants::FourCount) {
        return 4;
    } else if (randomNum < Constants::TwoCount + Constants::ThreeCount +
                             Constants::FourCount + Constants::FiveCount) {
        return 5;
    } else {
        return 6;
    }
}

// Randomly chooses password length based on max and min characters
int
PasswordGenerator::getFinalLength(std::mt19937& gen) const
{
    if (minLength > maxLength) {
        return minLength;
    }

    std::uniform_int_distribution<> dis(minLength, maxLength);
    return dis(gen);
}

// Randomly chooses a spacer between chunks/words to clearly separate them
std::string
PasswordGenerator::getSpacer(std::mt19937& gen) const
{
    if (!noSpecialChars) {
        std::uniform_int_distribution<int> dist(0,
                                                Constants::Spacers.size() - 1);
        return std::string(1, Constants::Spacers[dist(gen)]);
    } else if (!noNumbers) {
        std::uniform_int_distribution<int> dist(0, 1);
        return dist(gen) == 0 ? "0" : "1";
    } else {
        return "";
    }
}

// (Additional helper) Randomly selects a word from specific word list for
// diceware
std::string
getWordFromList(std::mt19937& gen, const std::vector<std::string>& vec)
{
    std::uniform_int_distribution<size_t> dist(0, vec.size() - 1);
    return vec[dist(gen)];
}

// (Additional helper) Randomly selects a single character string from a longer
// strict as supplied
std::string
getRandomString(std::mt19937& gen, const std::string& str)
{
    std::uniform_int_distribution<size_t> dist(0, str.size() - 1);
    std::string result(1, str[dist(gen)]);
    return result;
}

// Assembles a string of specific length via iterative random character
// selection
std::string
PasswordGenerator::getRandomStringOfLength(std::mt19937& gen,
                                           const std::string& str,
                                           int len,
                                           char lastChar) const
{
    std::string result;
    bool useHalfLogic = requireLowercase && requireUppercase && len >= 2;
    int midpoint = str.size() / 2;

    for (int i = 0; i < len; ++i) {
        std::string modifiedStr = str;

        if (noConsecutiveRepeats) {
            char charToRemove = (i == 0) ? lastChar : result.back();
            size_t pos = modifiedStr.find(charToRemove);
            if (pos != std::string::npos) {
                modifiedStr.erase(pos, 1);
            }
        }

        // Ensure the first and second characters are selected from specific
        // halves
        if (useHalfLogic) {
            if (i == 0) {
                // Select from the first half of the string
                std::string firstHalf = modifiedStr.substr(0, midpoint);
                result += getRandomString(gen, firstHalf);
                continue;
            } else if (i == 1) {
                // Select from the second half of the string
                std::string secondHalf = modifiedStr.substr(midpoint);
                result += getRandomString(gen, secondHalf);
                continue;
            }
        }

        // For other characters, use the full modifiedStr
        result += getRandomString(gen, modifiedStr);
    }

    return result;
}

// Assembles a string of specific length via iterative random character
// selection, for no dictionary words requirement
std::string
PasswordGenerator::getRandomStringOfLengthNoDict(std::mt19937& gen,
                                                 const std::string& str1,
                                                 const std::string& str2,
                                                 int len,
                                                 char lastChar) const
{
    std::string result;
    bool useHalfLogic = requireLowercase && requireUppercase &&
                        len >= 3; // Updated condition for len >= 3
    int midpoint1 = str1.size() / 2;

    for (int i = 0; i < len; ++i) {
        // Determine which string to use based on whether i is even or odd
        const std::string& currentStr = (i % 2 == 0) ? str1 : str2;
        std::string modifiedStr = currentStr;

        // Apply noConsecutiveRepeats logic only when i is 0
        if (i == 0 && noConsecutiveRepeats) {
            size_t pos = modifiedStr.find(lastChar);
            if (pos != std::string::npos) {
                modifiedStr.erase(pos, 1);
            }
        }

        // Ensure the first and third (even) characters are selected from
        // specific halves of str1
        if (useHalfLogic && (i == 0 || i == 2)) {
            if (i == 0) {
                // Select from the first half of str1
                std::string firstHalf = modifiedStr.substr(0, midpoint1);
                result += getRandomString(gen, firstHalf);
                continue;
            } else if (i == 2) {
                // Select from the second half of str1
                std::string secondHalf = modifiedStr.substr(midpoint1);
                result += getRandomString(gen, secondHalf);
                continue;
            }
        }

        // For all other cases, use the entire modifiedStr
        result += getRandomString(gen, modifiedStr);
    }

    return result;
}

// Specially required pattern for extensive requirements and smallest possible
// password
std::string
specialPatternGen(std::mt19937& gen, int length, bool numbers)
{
    if (length == 4) {
        return getRandomString(gen, Constants::Uppercase) +
               getRandomString(gen, Constants::Specials) +
               getRandomString(gen, Constants::Lowercase) +
               getRandomString(gen, Constants::Numbers);
    } else if (numbers) {
        return getRandomString(gen, Constants::Uppercase) +
               getRandomString(gen, Constants::Numbers) +
               getRandomString(gen, Constants::Lowercase);
    } else {
        return getRandomString(gen, Constants::Uppercase) +
               getRandomString(gen, Constants::Specials) +
               getRandomString(gen, Constants::Lowercase);
    }
}

// Main function to create and return the password
std::string
PasswordGenerator::create() const
{
    // Used in all random number generation to maintain cryptographic security
    // while avoiding constantly re-initializing these
    std::random_device rd;
    std::mt19937 gen(rd());

    if (useDiceware) {

        int length = getFinalLength(gen);

        bool reserveLast = false;
        if (requireNumbers && !noSpecialChars) {
            reserveLast = true;
            if (length == maxLength) {
                length--;
            }
        }

        std::vector<std::vector<std::string>> words = readWordsFromFile();
        std::string wordCase = "Snake";
        if (noUppercase) {
            wordCase = "Lower";
        } else if (noLowercase) {
            wordCase = "Upper";
        }

        std::string spacer = getSpacer(gen);
        std::string password = "";

        while (password.length() < length) {
            int charsLeft = length - password.length();
            if (charsLeft >= 3 || (spacer == "" && charsLeft >= 2)) {
                if (wordCase == "Snake") {
                    password += getWordFromList(
                      gen, words[getArrayForLength(charsLeft, gen) - 2]);
                } else if (wordCase == "Lower") {
                    password += toLowerCase(getWordFromList(
                      gen, words[getArrayForLength(charsLeft, gen) - 2]));
                } else {
                    password += toUpperCase(getWordFromList(
                      gen, words[getArrayForLength(charsLeft, gen) - 2]));
                }
            } else if (charsLeft >= 2 || (spacer == "" && charsLeft >= 1)) {
                if (wordCase == "Snake") {
                    password += getRandomString(
                      gen, Constants::Lowercase + Constants::Uppercase);
                } else if (wordCase == "Lower") {
                    password += getRandomString(gen, Constants::Lowercase);
                } else {
                    password += getRandomString(gen, Constants::Uppercase);
                }
            }
            password += spacer;
        }

        if (reserveLast) {
            password += getRandomString(gen, Constants::Numbers);
        }

        return password;
    } else {
        int length = getFinalLength(gen);

        bool reserveLast = false;
        if (requireNumbers && !noSpecialChars) {
            reserveLast = true;
            if (length == maxLength) {
                length--;
            }
        }

        std::string charChoice;
        if (!noUppercase && !noLowercase) {
            charChoice = Constants::Lowercase + Constants::Uppercase;
        } else if (noUppercase && !noLowercase) {
            charChoice = Constants::Lowercase;
        } else if (!noUppercase) {
            charChoice = Constants::Uppercase;
        } else if (!noNumbers) {
            charChoice = Constants::Numbers;
        } else {
            charChoice = Constants::Specials;
        }

        std::string charChoice2;
        if ((!noUppercase || !noLowercase) && noDictionaryWords) {
            if (!noNumbers && !noSpecialChars) {
                charChoice2 = Constants::Numbers + Constants::Specials;
            } else if (!noNumbers) {
                charChoice2 = Constants::Numbers;
            } else {
                charChoice2 = Constants::Specials;
            }
            if (requireUppercase && requireLowercase && requireNumbers &&
                requireSpecialChars && length == 4) {
                return specialPatternGen(gen, length, true);
            } else if (requireUppercase && requireLowercase &&
                       (requireNumbers || requireSpecialChars) && length == 3) {
                return specialPatternGen(gen, length, requireNumbers);
            }
        }

        std::string spacer = getSpacer(gen);
        std::string password = "";

        while (password.length() < length) {
            int charsLeft = length - password.length();
            if (charsLeft >= 3 || (spacer == "" && charsLeft >= 2)) {
                char lastChar = password.empty() ? '\x1F' : password.back();
                // Above, null separator is default char for no consecutive
                // repeat work, since it's never present here

                if (noDictionaryWords) {
                    password += getRandomStringOfLengthNoDict(
                      gen,
                      charChoice,
                      charChoice2,
                      getArrayForLength(charsLeft, gen) - 1,
                      lastChar);
                } else {
                    password += getRandomStringOfLength(
                      gen,
                      charChoice,
                      getArrayForLength(charsLeft, gen) - 1,
                      lastChar);
                }

            } else if (charsLeft >= 2 || (spacer == "" && charsLeft >= 1)) {
                char lastChar = password.empty() ? '\x1F' : password.back();
                if (noDictionaryWords) {
                    password += getRandomStringOfLengthNoDict(
                      gen, charChoice, charChoice2, 1, lastChar);
                } else {
                    password +=
                      getRandomStringOfLength(gen, charChoice, 1, lastChar);
                }
            }
            if (noConsecutiveRepeats && spacer != "") {
                char lastChar = password.empty() ? '\x1F' : password.back();
                password +=
                  getRandomStringOfLength(gen, charChoice, 1, lastChar);
            } else {
                password += spacer;
            }
        }

        if (reserveLast) {
            password += getRandomString(gen, Constants::Numbers);
        }

        return password;
    }
}

std::string
PasswordGenerator::dicewareOnly() const
{
    if (dicewareWordCount < 1) {
        throw std::runtime_error("ERROR: Word count must be at least 1.");
    }

    std::vector<std::vector<std::string>> words = readWordsFromFile();

    std::random_device rd;
    std::mt19937 gen(rd());

    std::string password;
    for (int i = 0; i < dicewareWordCount; ++i) {
        password += getWordFromList(gen, words[getArrayForLength(6, gen) - 2]);
        password += " ";
    }

    return password;
}

std::string
PasswordGenerator::generate(const Flags_t& flags, bool maxUsed, bool minUsed)
{
    // Directly modify the member variables without using setter functions
    minLength = flags.minLength;
    maxLength = flags.maxLength;

    if (minUsed && !maxUsed) {
        maxLength = minLength + 5;
    } else if (maxUsed && !minUsed) {
        if (maxLength < 5) {
            minLength = maxLength;
        } else {
            minLength = std::max(maxLength - 2, 1);
        }
    }

    requireUppercase = flags.upperRequired;
    noUppercase = flags.upperNotAllowed;
    requireLowercase = flags.lowerRequired;
    noLowercase = flags.lowerNotAllowed;
    requireNumbers = flags.numRequired;
    noNumbers = flags.numNotAllowed;
    requireSpecialChars = flags.specialRequired;
    noSpecialChars = flags.specialNotAllowed;
    noDictionaryWords = flags.noDictionaryWords;
    noConsecutiveRepeats = flags.noRepeatCharacters;
    useDiceware = flags.dicewarePassword;
    fullDiceware = flags.dicewareOnly;
    dicewareWordCount = flags.dicewareLength;

    if (fullDiceware) {
        return dicewareOnly();
    }

    validate();

    return create();
}
