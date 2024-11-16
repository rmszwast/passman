#ifndef ENCRYPTOR_HANDLER_H
#define ENCRYPTOR_HANDLER_H

#include "encryptor.hpp"

#include <botan/aead.h>
#include <botan/assert.h>
#include <botan/auto_rng.h>
#include <botan/hash.h>
#include <botan/hex.h>
#include <botan/pwdhash.h>

#include <cstdint>
#include <vector>

// Helper class allows string interaction with Encryptor class
class EncryptorHandler {
public:
  EncryptorHandler();
  std::vector<uint8_t> encrypt(std::string_view password,
                               const std::string_view stringToEncrypt = "");
  std::string decrypt(std::string_view password,
                      std::span<const uint8_t> encryptedData);

private:
  Encryptor encryptor_;
};

#endif