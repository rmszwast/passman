#ifndef CREATE_H
#define CREATE_H

#include "../utils/passGetter.hpp"
#include "../utils/vaultWriter.hpp"
#include "command.hpp"

class Create : public Command
{
  public:
    Create(CLI::App* app);
    void setup();
};

// Creates a password vault at path.
// User must enter matching passwords.
// Prints a success message to stdout if the operation was successful.
// Prints an error to stderr if the operation was unsuccessful.
void
createVault(std::string path,
            VaultWriterInterface& vaultWriter,
            PassGetterInterface& pass);

#endif