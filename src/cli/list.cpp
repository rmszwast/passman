#include "list.hpp"
#include "../utils/encryptor_handler.hpp"
#include "../utils/error.hpp"
#include <yaml-cpp/yaml.h>

namespace {
std::string path;
Flags_t flags;
VaultReader vaultReader;
PassGetter passGetter;
}

List::List(CLI ::App* app)
  : Command(app) {};

void
List::setup()
{
    name = "list";
    description = "List vault entries";
    app =
      app->add_subcommand(name, description)
        ->usage("passman entry list [OPTIONS] PATH")
        ->alias("ls")
        ->callback([]() { listEntries(path, flags, vaultReader, passGetter); });
    app->add_option("path", path, "Path of the vault")->required();
};

void
listEntries(std::string path,
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
    auto node = YAML::Load(decoded);

    // Print vault entries
    for (YAML::const_iterator it = node.begin(); it != node.end(); ++it) {
        std::cout << it->first << std::endl;
    }
};