#ifndef VAULTWRITER_H
#define VAULTWRITER_H

#include <stdint.h>
#include <string>
#include <vector>

class VaultWriterInterface
{
  public:
    virtual void write(const std::string path,
                       const std::vector<uint8_t> data) = 0;
};

class VaultWriter : public VaultWriterInterface
{
  public:
    void write(const std::string path, const std::vector<uint8_t> data);
};

#endif