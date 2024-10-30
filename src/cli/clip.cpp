#include "clip.hpp"
#include "command.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#define DEFAULT_CLIPBOARD_TIMEOUT 10

Clip::Clip(CLI::App* app)
  : Command(app) {};

void
Clip::setup()
{
    name = "clip";
    description = "Copy entry information to the clipboard";
    app = app->add_subcommand(name, description);
    app->usage("passman clip COMMAND [OPTIONS]");
    app->add_option("clip"
                    "clip = copy to clipboard");
    /**
     * Should we inherit the following from Command?
     * timout
     * attributeOption
     * TotpOption
     * bestMatchOption
     *
     */
}

int
Clip::executeWithVault(Vault vault, CommandLineParser parser)
{
    auto& out =
}
