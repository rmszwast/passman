#include "remove.hpp"
#include "../utils/encryptor_handler.hpp"
#include "../utils/error.hpp"
#include <yaml-cpp/yaml.h>

namespace {
std::string path;
Entry_t entry;
VaultReaderWriter vaultReaderWriter;
PassGetter passGetter;
}

Remove::Remove(CLI::App* app)
  : Command(app) {};

void
Remove::setup()
{
    name = "remove";
    description = "Remove an entry";
    app = app->add_subcommand(name, description)
            ->usage("passman entry remove [OPTIONS] PATH ENTRY")
            ->alias("rm")
            ->callback([]() {
                removeEntry(path, entry, vaultReaderWriter, passGetter);
            });
    app->add_option("path", path, "Path of the vault")->required();
    app->add_option("entry", entry.name, "Name of the entry")->required();
}

void
removeEntry(std::string path,
            Entry_t entry,
            VaultReaderWriterInterface& vaultReaderWriter,
            PassGetterInterface& passGetter)
{

    // Read vault data
    std::vector<uint8_t> encoded;
    try {
        encoded = vaultReaderWriter.read(path);
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

    // Decrypt vault data
    EncryptorHandler encryptor;
    std::string decoded;
    try {
        decoded = encryptor.decrypt(pass, encoded);

    } catch (Botan::Invalid_Authentication_Tag& err) {
        printErr("decryption failure: invalid password: if this reoccurs, then "
                 "the vault may be corrupt");
        return;
    }

    // Load decrypted vault data to a node
    auto node = YAML::Load(decoded);

    // Check if entry exists
    if (!node[entry.name]) {
        printErr("no entry named " + entry.name);
        return;
    }

    // Remove entry
    node.remove(entry.name);

    // Encrypt data
    std::ostringstream oss;
    oss << node;
    encoded = encryptor.encrypt(pass, oss.str());

    // Write encrypted data to vault
    try {
        vaultReaderWriter.write(path, encoded);
    } catch (std::runtime_error err) {
        printErr(err.what());
        return;
    }

    // Print success message
    std::cout << entry.name << " successfully removed" << std::endl;
}