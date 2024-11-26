#include "show.hpp"
#include "../utils/encryptor_handler.hpp"
#include "../utils/error.hpp"
#include "yaml-cpp/yaml.h"
#include <fstream>

namespace {
std::string path;
Entry_t entry;
Flags_t flags;
VaultReader vaultReader;
PassGetter passGetter;
}

Show::Show(CLI::App* app)
  : Command(app) {};

void
Show::setup()
{
    name = "show";
    description = "Show an entry's information";
    app = app->add_subcommand(name, description)
            ->usage("passman entry show [OPTIONS] PATH ENTRY")
            ->callback(
              []() { showEntry(path, entry, flags, vaultReader, passGetter); });
    app->add_flag("-s, --show-protected",
                  flags.showBool,
                  "Show the entry's password in clear text");
    app->add_option("path", path, "Path of the vault")->required();
    app->add_option("entry", entry.name, "Name of the entry")->required();
};

void
showEntry(std::string path,
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

    // Print entry information
    YAML::Node onode = inode[entry.name];
    if (!flags.showBool) {
        onode["Password"] = "****";
    }
    std::cout << "\033[4;92m" + entry.name + ":\n\033[0m" << onode << std::endl;
};