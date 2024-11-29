#include "edit.hpp"
#include "../utils/encryptor_handler.hpp"
#include "../utils/error.hpp"
#include "../utils/generate.hpp"
#include <yaml-cpp/yaml.h>

namespace {
std::string path;
Entry_t entry;
Flags_t flags;
VaultReaderWriter vaultReaderWriter;
PassGetter passGetter;
}

Edit::Edit(CLI::App* app)
  : Command(app) {};

void
Edit::setup()
{
    name = "edit";
    description = "Edit an entry";
    app = app->add_subcommand(name, description)
            ->usage("passman entry edit [OPTIONS] PATH ENTRY")
            ->callback([]() {
                editEntry(path, entry, flags, vaultReaderWriter, passGetter);
            });
    app->add_option(
      "--username, --user", entry.username, "Username for the entry");
    app->add_option("--url", entry.URL, "URL for the entry");
    app->add_option("--notes", entry.notes, "Notes for the entry");
    app->add_option("path", path, "Path of the vault")->required();
    app->add_option("entry", entry.name, "Name of the entry")->required();
    app->add_flag("-p, --prompt-password",
                  flags.promptBool,
                  "Prompt for the entry's password");
    app->add_flag("-g, --generate-password",
                  flags.genBool,
                  "Generate a password for the entry");
    addPartialPasswordFlags(app, &flags);
    addPartialDicewareFlags(app, &flags);
};

void
editEntry(std::string path,
          Entry_t entry,
          Flags_t flags,
          VaultReaderWriterInterface& vaultReaderWriter,
          PassGetterInterface& passGetter)
{

    // Flag validation
    if (flags.genBool && flags.promptBool) {
        printErr(
          "--prompt-password and --generate-password flags cannot be used "
          "simultaneously");
        return;
    }

    // Validate update parameters are provided
    if (entry.username.empty() && entry.password.empty() &&
        (entry.URL.empty()) && (entry.notes.empty()) && !flags.promptBool &&
        !flags.genBool) {
        printErr("At least one of the following entry parameters must be "
                 "updated: username, password, url, notes");
        return;
    }

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

    // Get or generate entry password
    if (flags.promptBool) {
        try {
            entry.password = passGetter.get("Enter password for " + entry.name);
        } catch (std::runtime_error err) {
            printErr(err.what());
            return;
        }
    } else if (flags.genBool) {
        PasswordGenerator passgen;
        try {
            entry.password = passgen.generate(flags);
        } catch (ConflictingFlagsException err) {
            printErr(err.what());
            return;
        }
    }

    // Update entry
    if (!entry.username.empty())
        node[entry.name]["Username"] = entry.username;

    if (!entry.password.empty())
        node[entry.name]["Password"] = entry.password;

    if (!entry.URL.empty())
        node[entry.name]["URL"] = entry.URL;

    if (!entry.notes.empty())
        node[entry.name]["Notes"] = entry.notes;

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
    std::cout << entry.name << " successfully updated" << std::endl;
}