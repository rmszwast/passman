#include <cstdint>
#define CATCH_CONFIG_MAIN // Define main
#include "encryptor.hpp"
#include "encryptor_handler.hpp"

#include <catch2/catch.hpp>

TEST_CASE("String Encryption and Decryotion Operations") {
  // Test variables
  EncryptorHandler encryptorHandler;
  std::string message = "This is the message";
  std::string password = "ComplicatedPassword";
  std::string emptyString = "";

  // Tests
  SECTION("Decrypting with correct password passes") {
    auto ciphertext = encryptorHandler.encrypt(password, message);

    REQUIRE((encryptorHandler.decrypt(password, ciphertext)) == message);
  }

  SECTION("Encrypting with empty string does not fail") {
    auto ciphertext = encryptorHandler.encrypt(password, emptyString);
    REQUIRE(!ciphertext.empty());
  }
}
