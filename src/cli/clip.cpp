#include "clip.hpp"
#include "command.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "entry.hpp"
#include <vector>
#include <iostream>
#include <stdio.h>
#include <unistd.h>

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
    app->add_option("clip", "clip = copy to clipboard");
    /**
     * Should we inherit the following from Command?
     * timout
     * attributeOption
     * TotpOption
     * bestMatchOption
     * currentVault
     * positionalArguments
     * optionalArguments
     * options
     *
     */
}

/* TODO: Implement argument parsing, timeout functionality,
option selection, exit handling, display to user */

int
Clip::execute(Entry entry, int timeout = DEFAULT_CLIPBOARD_TIMEOUT)
{
  /**
   * Need to modify to escape special characters that
   * may be in password. Also need to consider using 
   * exec family functions instead of system
   */
  if (timeout <= 0) {
    return 0;
  }
  std::string password = entry.getPassword;
  //sanitize password here
  std::string cmd = "echo " + password + " | xclip -selection clipboard";
  int res = system(cmd.c_str());
  if (res != 0) {
    std::cout << "Command failed with code: " << res << std::endl;
  }
  while (timeout > 0) {
    std::cout << "clearing clipboard in " << timeout << " seconds";
    sleep(1);
    timeout--;
    fflush(stdout);
  }
  std::cout << "\r\033[K";
  std::cout.flush();
  std::cout << "Clipboard cleared" << std::endl;
  system("xclip /dev/null");
  system("xclip -selection clipboard /dev/null");


  return 0;
}
