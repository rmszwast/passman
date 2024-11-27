#ifndef PASSWORD_H
#define PASSWORD_H

#include "command.hpp"

class Password : public Command
{
  public:
    Password(CLI::App* app);
    void setup();
};

#endif
