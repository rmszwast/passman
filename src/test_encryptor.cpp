#include <cstdint>
#define CATCH_CONFIG_MAIN // Define main
#include "encryptor.hpp"
#include <catch2/catch.hpp>

TEST_CASE("Encryption and Decryption Operations") {
  // Test variables
  Encryptor encryptor;
  std::string message = "This is the message";
  std::string password = "ComplicatedPassword";
  std::string incorrectPassword = "incorrectPassword";
  Botan::secure_vector<uint8_t> messageAsVector(
      reinterpret_cast<const uint8_t *>(message.data()),
      reinterpret_cast<const uint8_t *>(message.data()) + message.size());

  // Encryption unit tests
  SECTION(
      "Calling encrypt twice with same values produces different ciphertext") {
    REQUIRE((encryptor.encrypt(password, messageAsVector)) !=
            encryptor.encrypt(password, messageAsVector));
  }

  // Decryption unit tests
  SECTION("Decrypting with correct password passes") {
    auto ciphertext = encryptor.encrypt(password, messageAsVector);
    REQUIRE(encryptor.decrypt(password, ciphertext) == messageAsVector);
  }

  SECTION("Decrypting with incorrect password fails") {
    auto ciphertext = encryptor.encrypt(password, messageAsVector);

    REQUIRE_THROWS(encryptor.decrypt(incorrectPassword, ciphertext));
  }

  SECTION(
      "Decrypting with message length that is less than salt length fails") {
    std::vector<uint8_t> tooShortMessage = {
        32}; // Initialize with ASCII space character

    REQUIRE_THROWS_AS(encryptor.decrypt(password, tooShortMessage),
                      std::invalid_argument);
  }

  SECTION("Decrypting with correct password and tampered data fails") {
    auto ciphertext = encryptor.encrypt(password, messageAsVector);

    // Tamper with ciphertext by appending some data to the end
    std::span<const uint8_t> ciphertextSpan(ciphertext);
    std::vector<uint8_t> dataToAdd = {19, 4, 5, 7, 11};
    std::vector<uint8_t> tamperedCipherText(ciphertextSpan.begin(),
                                            ciphertextSpan.end());
    tamperedCipherText.insert(tamperedCipherText.end(), dataToAdd.begin(),
                              dataToAdd.end());
    REQUIRE_THROWS(encryptor.decrypt(password, tamperedCipherText));
  }
}