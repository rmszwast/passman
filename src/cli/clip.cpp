#include "clip.hpp"
#include "../utils/encryptor_handler.hpp"
#include "../utils/error.hpp"
#include "yaml-cpp/yaml.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <vector>

#define DEFAULT_CLIPBOARD_TIMEOUT 10
#define MIN_TIME 1
#define MAX_TIME 30

namespace {
std::string path;
Entry_t entry;
Flags_t flags;
VaultReader vaultReader;
PassGetter passGetter;
int manualTimeout;
}

Clip::Clip(CLI::App* app)
  : Command(app) {};

void
Clip::setup()
{
    name = "clip";
    description = "Copy entry information to the clipboard";
    app = app->add_subcommand(name, description)
            ->usage("passman clip [OPTIONS] PATH ENTRY")
            ->callback([]() {
                clipPassword(
                  path, entry, flags, vaultReader, passGetter, manualTimeout);
            });
    app->add_option("-t, --timeout",
                    manualTimeout,
                    "Enter a timeout length in secs (default = 10)");
    app->add_option("path", path, "Path of the vault")->required();
    app->add_option("entry", entry.name, "Name of the entry")->required();
}

void
clipPassword(std::string path,
             Entry_t entry,
             Flags_t flags,
             VaultReaderInterface& vaultReader,
             PassGetterInterface& passGetter,
             int manualTimeout)
{

    // Read vault data
    std::vector<uint8_t> encoded;
    try {
        encoded = vaultReader.read(path);
    } catch (std::runtime_error err) {
        printErr(err.what());
        return;
    }

    // Prompt user for master password
    std::string pass;
    try {
        pass = passGetter.get("Enter password to decrypt " + path);
    } catch (std::runtime_error err) {
        printErr(err.what());
        return;
    }

    // Decrypt file
    EncryptorHandler encryptor;
    std::string decoded;
    try {
        decoded = encryptor.decrypt(pass, encoded);

    } catch (Botan::Invalid_Authentication_Tag& err) {
        printErr("decryption failure: invalid password: if this reoccurs, then "
                 "the vault may be corrupt");
        return;
    }

    // Load decrypted data to a node
    auto inode = YAML::Load(decoded);

    // Check if entry exists
    if (!inode[entry.name]) {
        printErr("no entry named " + entry.name);
        return;
    }

    // unpack password and store in variable
    std::string password = inode[entry.name]["Password"].as<std::string>();

    // set timeout for clearing clipboard
    int timeout;
    std::cout << std::to_string(manualTimeout) << std::endl;
    if (manualTimeout < MIN_TIME || manualTimeout > MAX_TIME) {
        timeout = DEFAULT_CLIPBOARD_TIMEOUT;
    } else {
        timeout = manualTimeout;
    }

    // sanitize password here
    std::string chars_to_escape = "\\\"$`<>|;&";
    for (char c : chars_to_escape) {
        size_t pos = 0;
        while ((pos = password.find(c, pos)) != std::string::npos) {
            password.insert(pos, "\\");
            pos += 2;
        }
    }
    // copy to clipboard
    std::string cmd = "echo " + password + " | xclip -selection clipboard";
    try {
        int res = system(cmd.c_str());
        if (res != 0) {
            std::cerr << "Failed to copy to clipboard. Please ensure xclip is "
                         "installed (try: sudo apt-get install xclip)"
                      << std::endl;
            return;
        } else {
            // clear clipboard after set amount of time
            while (timeout >= 0) {
                std::cout << "\r Clearing clipboard in " << timeout
                          << " seconds ";
                sleep(1);
                timeout--;
                fflush(stdout);
            }
            std::cout << "\r\033[K";
            fflush(stdout);
            std::cout << "Clipboard cleared" << std::endl;
            system("xclip /dev/null");
            system("xclip -selection clipboard /dev/null");
        }
    } catch (const std::runtime_error& e) {
        printErr(e.what());
        return;
    }
    return;
}
