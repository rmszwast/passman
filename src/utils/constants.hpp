#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

namespace Constants {
// Five different classes of character that could be used in a password
static const std::string Lowercase = "abcdefghijklmnopqrstuvwxyz";
static const std::string Uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const std::string Numbers = "0123456789";
static const std::string Spacers = "-_=+";
static const std::string Specials = "!@#$%^&*()[]{};:,.<>?";

// For the words file (diceware), how many of each length of word there are
// (avoid re-counting each time)
static const int TwoCount = 29;
static const int ThreeCount = 83;
static const int FourCount = 463;
static const int FiveCount = 929;
static const int SixCount = 1373;
}

#endif // CONSTANTS_H