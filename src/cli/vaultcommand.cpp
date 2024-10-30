#include "vaultcommand.hpp"
#include <string>

VaultCommand::VaultCommand() {}

int
VaultCommand::execute(const& arguments)
{
    return executeWithVault(database, parser);
}