#ifndef PASSGETTER_H
#define PASSGETTER_H

#include <string>

class PassGetterInterface
{
  public:
    virtual std::string get(std::string) = 0;
};

class PassGetter : public PassGetterInterface
{
  public:
    // Gets user input without echoing to stdout.
    std::string get(std::string);
};

#endif
