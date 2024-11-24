#include "vaultReader.hpp"
#include <filesystem>
#include <fstream>

std::vector<uint8_t>
VaultReader::read(const std::string path)
{
    // Check if file exists
    namespace fs = std::filesystem;
    if (!fs::exists(fs::path{ path })) {
        throw std::runtime_error{ "file not found" };
    }

    // Open file
    std::ifstream ifs;
    ifs.open(path, std::ifstream::in);
    if (!ifs.is_open()) {
        throw std::runtime_error{ "failed to open file" };
    }

    // Read file
    std::ostringstream oss;
    oss << ifs.rdbuf();
    ifs.close();
    if (!ifs.good()) {
        throw std::runtime_error{ "failed to close file" };
    }

    // Return file contents
    auto str = oss.str();
    return std::vector<uint8_t>(str.begin(), str.end());
};