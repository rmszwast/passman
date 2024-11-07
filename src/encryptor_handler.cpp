#include "encryptor_handler.hpp"

EncryptorHandler::EncryptorHandler(){};

std::vector<uint8_t>
EncryptorHandler::encrypt(std::string_view password,
                          const std::string_view stringToEncrypt) {

  // Cast string to Botan::secure_vector
  std::span<const uint8_t> bytesToEncrypt = std::span<const uint8_t>(
      reinterpret_cast<const uint8_t *>(stringToEncrypt.data()),
      stringToEncrypt.size());
  Botan::secure_vector<uint8_t> ciphertext(bytesToEncrypt.begin(),
                                           bytesToEncrypt.end());

  return encryptor_.encrypt(password, ciphertext);
}

std::string EncryptorHandler::decrypt(std::string_view password,
                                      std::span<const uint8_t> encryptedData) {
  const auto decryptedMessage = encryptor_.decrypt(password, encryptedData);

  return (std::string(
      decryptedMessage.begin(),
      decryptedMessage.end())); // Convert decryptedMessage to string on return
}