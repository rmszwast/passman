#include "error.hpp"
#include <iostream>

std::string
stringErr(std::string msg)
{
    return "\033[31merror: \033[0m" + msg;
};

void
printErr(std::string msg)
{
    std::cerr << stringErr(msg) << std::endl;
};
