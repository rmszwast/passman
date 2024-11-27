#ifndef DICEWARE_H
#define DICEWARE_H

#include "command.hpp"

class Diceware : public Command
{
  public:
    Diceware(CLI::App* app);
    void setup();
};

#endif
