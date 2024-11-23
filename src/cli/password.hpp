#ifndef PASSWORD_H
#define PASSWORD_H

#include "command.hpp"

class Password : public Command
{
  public:
    Password(CLI::App* app);
    void setup();
};

void
addPartialPasswordFlags(CLI::App* generateCmd, Flags_t* flags);
void
addAllPasswordFlags(CLI::App* generateCmd, Flags_t* flags);

#endif
