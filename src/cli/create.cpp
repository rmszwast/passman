#include "create.hpp"
#include "../utils/encryptor_handler.hpp"
#include "../utils/error.hpp"
#include <fstream>

namespace {
std::string path;
VaultWriter vaultWriter;
PassGetter passGetter;
}

Create::Create(CLI::App* app)
  : Command(app) {};

void
Create::setup()
{
    name = "create";
    description = "Create a vault";
    app = app->add_subcommand(name, description)
            ->usage("passman vault create [OPTIONS] PATH")
            ->callback([]() { createVault(path, vaultWriter, passGetter); });
    app->add_option("path", path, "Path of the vault")->required();
};

void
createVault(std::string path,
            VaultWriterInterface& vaultWriter,
            PassGetterInterface& passGetter)
{
    namespace fs = std::filesystem;
    std::ofstream ofs;

    // Check if file already exists
    if (fs::exists(fs::path{ path })) {
        printErr(path + ": " + "file already exists");
        return;
    }

    // Prompt user for master password creation
    std::string pass;
    try {
        auto pass1 = passGetter.get("Enter password to encrypt vault");
        auto pass2 = passGetter.get("Re-enter password");
        if (pass1 != pass2) {
            printErr("passwords do not match");
            return;
        } else {
            pass = pass1;
        }
    } catch (std::runtime_error err) {
        printErr(err.what());
    }

    // Encrypt the file
    EncryptorHandler encryptor;
    auto encoded = encryptor.encrypt(pass);

    // Write encrypted data to vault
    try {
        vaultWriter.write(path, encoded);
    } catch (std::runtime_error err) {
        printErr(err.what());
        return;
    }

    // Print success message
    std::cout << path << " successfully created" << std::endl;
};
