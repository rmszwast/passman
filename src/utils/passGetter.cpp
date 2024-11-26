#include "passGetter.hpp"
#include "error.hpp"
#include <iostream>
#include <termios.h>
#include <unistd.h>

std::string
PassGetter::get(std::string prompt)
{
    std::string pass = "";
    termios oflags, nflags;
    tcgetattr(STDIN_FILENO, &oflags);
    nflags = oflags;

    // Print prompt
    std::cout << prompt + ": ";

    // Disable echo to stdout
    nflags.c_lflag ^= ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &nflags);

    // Get input
    std::getline(std::cin, pass);
    std::cout << "\n";

    // Restore echo to stdout
    tcsetattr(STDIN_FILENO, TCSANOW, &oflags);

    if (pass.length() > 4095) {
        throw std::runtime_error{ "input is too large" };
    }

    return pass;
};
