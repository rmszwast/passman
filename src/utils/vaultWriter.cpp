#include "vaultWriter.hpp"
#include <filesystem>
#include <fstream>

void
VaultWriter::write(const std::string path, const std::vector<uint8_t> data)
{
    // Open file
    std::ofstream ofs;
    ofs.open(path, std::ofstream::trunc);
    if (!ofs.is_open()) {
        throw std::runtime_error{ "failed to open file: " + path };
    }

    // Write to file
    ofs.write(reinterpret_cast<const char*>(data.data()), data.size());
    ofs.close();
    if (!ofs.good()) {
        throw std::runtime_error{ "failed to close file: " + path };
    };
}
