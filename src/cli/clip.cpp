#include "clip.hpp"
#include "lib/yaml-cpp/include/yaml-cpp/yaml.h"
#include "../utils/error.hpp"
#include "../utils/encryptor_handler.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <vector>
#include <fstream>

#define DEFAULT_CLIPBOARD_TIMEOUT 10

namespace {
    std::string path;
    Entry_t entry;
    Flags_t flags;
    VaultReader vaultReader;
    PassGetter passGetter;
}

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
}

void
Clip(std::string path,
    Entry_t entry,
    Flags_t flags,
    VaultReaderInterface& vaultReader, 
    PassGetterInterface& passGetter)
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

    //unpack password and store in variable
    YAML::Node onode = inode[entry.name];
    std::string password = onode["Password"];

    int timeout = DEFAULT_CLIPBOARD_TIMEOUT;
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
            std::cerr << "Failed to copy to clipboard. Please ensure xclip is installed (try: sudo apt-get install xclip)" << std::endl;
            return;
        }
        else {
            // clear clipboard after set amount of time
            while (timeout >= 0) {
                std::cout << "\r Clearing clipboard in " << timeout << " seconds ";
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
    }
    catch (const std::runtime_error& e) {
    }
    return;
}
