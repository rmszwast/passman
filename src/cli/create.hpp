#ifndef CREATE_H
#define CREATE_H

#include "command.hpp"

class Create : public Command
{
  public:
    Create(CLI::App* app);
    void setup();
};

#endif