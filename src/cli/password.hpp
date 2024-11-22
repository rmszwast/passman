#ifndef PASSWORD_H
#define PASSWORD_H

#include "command.hpp"

class Password : public Command
{
  public:
    Password(CLI::App* app);
    void setup();

  private:
    void generatePassword(int minLength,
                          int maxLength,
                          bool upperRequired,
                          bool lowerRequired,
                          bool numRequired,
                          bool specialRequired,
                          bool upperNotAllowed,
                          bool lowerNotAllowed,
                          bool numNotAllowed,
                          bool specialNotAllowed,
                          bool noDictionaryWords,
                          bool noRepeatCharacters);

    void generateDicewarePassword(int dicewareWordCount);
};

#endif
