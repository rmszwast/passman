#ifndef VAULTREADER_H
#define VAULTREADER_H

#include <stdint.h>
#include <string>
#include <vector>

class VaultReaderInterface
{
  public:
    virtual std::vector<uint8_t> read(const std::string path) = 0;
};

class VaultReader : public VaultReaderInterface
{
  public:
    std::vector<uint8_t> read(const std::string path);
};

#endif