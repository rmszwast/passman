#ifndef VAULTREADERWRITER_H
#define VAULTREADERWRITER_H

#include "vaultReader.hpp"
#include "vaultWriter.hpp"
#include <stdint.h>
#include <string>
#include <vector>

class VaultReaderWriterInterface
  : public VaultReaderInterface
  , public VaultWriterInterface
{};

class VaultReaderWriter
  : public VaultReaderWriterInterface
  , public VaultReader
  , public VaultWriter
{
  public:
    std::vector<uint8_t> read(const std::string path);
    void write(const std::string path, const std::vector<uint8_t> data);
};

#endif