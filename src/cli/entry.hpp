#ifndef ENTRY_H
#define ENTRY_H

#include "command.hpp"

class Entry : public Command
{
  public:
    Entry(CLI::App* app);
    void setup();
};

#endif