#ifndef EXAMPLE_H
#define EXAMPLE_H

#include "command.hpp"
#include "password.hpp"

class Example : public Command
{
  public:
    Example(CLI::App* app);
    void setup();
};

#endif
