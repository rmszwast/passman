#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include <botan/aead.h>
#include <botan/assert.h>
#include <botan/auto_rng.h>
#include <botan/hash.h>
#include <botan/hex.h>
#include <botan/pwdhash.h>

#include <cstdint>
#include <iostream>
#include <vector>

// Encrypts and decrypts byte data
class Encryptor {
public:
  Encryptor();
  std::vector<uint8_t> encrypt(std::string_view password,
                               Botan::secure_vector<uint8_t> ciphertext =
                                   Botan::secure_vector<uint8_t>{});
  Botan::secure_vector<uint8_t> decrypt(std::string_view password,
                                        std::span<const uint8_t> encryptedData);

private:
  static constexpr size_t SALT_STR_LENGTH = 16;

  Botan::secure_vector<uint8_t> deriveKey(std::string_view password,
                                          std::span<const uint8_t> salt,
                                          size_t keyLength);
  std::unique_ptr<Botan::AEAD_Mode> setUpAEAD(std::string_view password,
                                              std::span<const uint8_t> salt,
                                              Botan::Cipher_Dir selectedMode);
  Botan::AutoSeeded_RNG generateRandomSeed();
};

#endif
