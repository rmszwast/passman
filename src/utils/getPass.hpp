#ifndef GETPASS_H
#define GETPASS_H

#include <string>

class getPass_interface
{
  public:
    virtual std::string getPass(std::string) = 0;
};

class getPass_api : public getPass_interface
{
  public:
    // Gets user input without echoing to stdout.
    std::string getPass(std::string);
};

#endif