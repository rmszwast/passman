#include "create.hpp"
#include "../utils/error.hpp"
#include "../utils/getPass.hpp"
#include <fstream>

std::string path;

Create::Create(CLI::App* app)
  : Command(app) {};

void
Create::setup()
{
    name = "create";
    description = "Create a vault";
    app = app->add_subcommand(name, description)
            ->usage("passman vault create [OPTIONS] PATH")
            ->callback(f);
    app->add_option("path", path, "The path of the vault")->required();
};

void
createVault(std::string path, getPass_interface& api)
{
    namespace fs = std::filesystem;
    std::ofstream ofs;

    // Check if file already exists
    try {
        if (fs::exists(fs::path{ path })) {
            throw std::invalid_argument{ "file already exists" };
        }
    } catch (std::invalid_argument& err) {
        printErr(path + ": " + err.what());
        return;
    }

    // Prompt for password creation
    try {
        auto pass1 = api.getPass("Enter password to encrypt vault");
        auto pass2 = api.getPass("Re-enter password");
        if (pass1 != pass2) {
            printErr("passwords do not match");
            return;
        }
    } catch (std::runtime_error err) {
        printErr(err.what());
    }

    // Check if file can be opened
    ofs.open(path, std::ofstream::out);
    try {
        if (!ofs.is_open()) {
            throw std::invalid_argument{ "invalid path" };
        }
    } catch (std::invalid_argument& err) {
        printErr(path + ": " + err.what());
        return;
    }

    // TODO: integrate encryption

    // Write to file and close
    ofs << "Hello from password vault!" << std::endl;
    ofs.close();
    if (!ofs.good()) {
        printErr("error: failed to close file");
        return;
    }

    // Print success message
    std::cout << path << " successfully created" << std::endl;
};

getPass_api api;
std::function<void()> f = []() { createVault(path, api); };