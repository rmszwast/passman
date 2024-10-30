#ifndef REMOVE_H
#define REMOVE_H

#include "command.hpp"

class Remove : public Command
{
  public:
    Remove(CLI::App* app);
    void setup();
};

#endif