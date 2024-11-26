#include "vaultReaderWriter.hpp"

std::vector<uint8_t>
VaultReaderWriter::read(const std::string path)
{
    return VaultReader::read(path);
};

void
VaultReaderWriter::write(const std::string path,
                         const std::vector<uint8_t> data)
{
    return VaultWriter::write(path, data);
};
