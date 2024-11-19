#ifndef ERROR_H
#define ERROR_H
#include <string>

// Returns msg prepended with text 'error: ' colored red as string.
std::string
stringErr(std::string msg);

// Prints msg to stderr prepended with text 'error: ' colored red.
void
printErr(std::string msg);

#endif