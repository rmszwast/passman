#include <botan/aead.h>
#include <botan/assert.h>
#include <botan/auto_rng.h>
#include <botan/hash.h>
#include <botan/hex.h>
#include <botan/pwdhash.h>

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

constexpr size_t SALT_STR_LENGTH = 16;

// Set master password
std::string setPassword() {
  std::string password = "";
  std::cout << "Set your master password: ";
  std::getline(std::cin, password);
  std::cout.flush();

  return password;
}

// Get user data to encrypt
std::string setMessage() {
  std::string message = "";
  std::cout << "Message to encrypt: ";
  std::cout.flush();
  std::getline(std::cin, message);

  return message;
}

// Create key for encryption algorithm using salt
Botan::secure_vector<uint8_t> generateKey(std::string_view password,
                                          std::span<const uint8_t> salt,
                                          size_t keyLength) {

  // Set paramaters for algorithm
  const std::string algorithm = "Argon2id";     // select pwkd algorithm
  constexpr size_t memoryCapacity = 256 * 1024; // kiB
  constexpr size_t iterations = 4;              // how many times to iterate
  constexpr size_t parallelism =
      2; // use multiple threads for parallelism so key derivation is faster

  // Create pbkdf object
  auto pbkdfObject =
      Botan::PasswordHashFamily::create_or_throw(algorithm)->from_params(
          memoryCapacity, iterations, parallelism);
  BOTAN_ASSERT_NONNULL(pbkdfObject); // Confirm botan object was created

  // Create key object using password and salt
  Botan::secure_vector<uint8_t> key(keyLength);
  pbkdfObject->derive_key(key.data(), key.size(), password.data(),
                          password.length(), salt.data(), SALT_STR_LENGTH);

  return key;
}

// Set up environment for encryption
std::unique_ptr<Botan::AEAD_Mode> setUpAEAD(std::string_view password,
                                            std::span<const uint8_t> salt,
                                            Botan::Cipher_Dir selectedMode) {
  // Create instance of AEAD obeject
  auto aead = Botan::AEAD_Mode::create_or_throw("AES-256/GCM", selectedMode);
  BOTAN_ASSERT_NONNULL(aead);

  // Retrieve key and nonce length for specifid encryption algorithm
  const size_t keyLength = aead->key_spec().maximum_keylength();
  const size_t nonceLength = aead->default_nonce_length();

  // Generate key based on parameters for specified encryption algorithm and
  // password + salt
  const auto keyData = generateKey(password, salt, (keyLength + nonceLength));
  BOTAN_ASSERT_NOMSG(keyData.size() == keyLength + nonceLength);

  // Extract key and nonce
  const auto key = std::span{keyData}.first(keyLength);
  const auto nonce = std::span{keyData}.last(nonceLength);

  // Initialize AEAD object to start encryption or decryption
  aead->set_key(key.data(), key.size());
  aead->start(nonce.data(), nonce.size());

  return aead;
}

// Encrypt data with PBKDF algorithm
std::vector<uint8_t> encrypt(std::string_view password,
                             Botan::RandomNumberGenerator &randomNum,
                             std::span<const uint8_t> dataToEncrypt) {

  const auto salt = randomNum.random_vec(
      SALT_STR_LENGTH); // Generate salt based on random value

  // Hash password using SHA-3(256)
  auto hashedPasswordFunction =
      Botan::HashFunction::create_or_throw("SHA-3(256)");
  Botan::secure_vector<uint8_t> hashedPassword =
      hashedPasswordFunction->process(
          reinterpret_cast<const uint8_t *>(password.data()), password.size());

  // Set up AEAD environment for encryption
  auto aead = setUpAEAD(
      std::string_view(reinterpret_cast<const char *>(hashedPassword.data()),
                       hashedPassword.size()),
      salt, Botan::Cipher_Dir::ENCRYPTION);

  // Initialize secure vector with data and encrypt
  Botan::secure_vector<uint8_t> ciphertext(dataToEncrypt.begin(),
                                           dataToEncrypt.end());
  aead->finish(ciphertext);

  // Hash ciphertext using SHA-3(256)
  auto hashedCiphertextFunction =
      Botan::HashFunction::create_or_throw("SHA-3(256)");
  Botan::secure_vector<uint8_t> hashedCiphertext =
      hashedCiphertextFunction->process(ciphertext.data(), ciphertext.size());

  // Create a new vector to hold the salt, ciphertext, and hash
  std::vector<uint8_t> hashedSaltAndCipherResult;
  hashedSaltAndCipherResult.reserve(SALT_STR_LENGTH + ciphertext.size() +
                                    hashedCiphertext.size()); //
  hashedSaltAndCipherResult.insert(hashedSaltAndCipherResult.end(),
                                   salt.begin(), salt.end());
  hashedSaltAndCipherResult.insert(hashedSaltAndCipherResult.end(),
                                   ciphertext.begin(), ciphertext.end());
  hashedSaltAndCipherResult.insert(hashedSaltAndCipherResult.end(),
                                   hashedCiphertext.begin(),
                                   hashedCiphertext.end());

  return hashedSaltAndCipherResult;
}

// Decrypt data with PBKDF algorithm
Botan::secure_vector<uint8_t> decrypt(std::string_view password,
                                      std::span<const uint8_t> encryptedData) {

  // If ciphertext is less than salt length, cannot authenticate, throw error
  if (encryptedData.size() < SALT_STR_LENGTH + 32) {
    std::cout << ("Encrypted data is wrong length");
  }

  // Salt is at beginning of encrypted data, extract
  const auto salt = encryptedData.first(SALT_STR_LENGTH);

  const auto ciphertext = encryptedData.subspan(
      SALT_STR_LENGTH, encryptedData.size() - SALT_STR_LENGTH -
                           32); // Extract ciphertext (32 bytes for SHA-3)
  const auto hash = encryptedData.last(32);

  // Hash the password to derive the key
  auto hashedPasswordFunction =
      Botan::HashFunction::create_or_throw("SHA-3(256)");
  Botan::secure_vector<uint8_t> hashedPassword =
      hashedPasswordFunction->process(
          reinterpret_cast<const uint8_t *>(password.data()), password.size());

  // Set up AEAD environment for decryption
  auto aead = setUpAEAD(
      std::string_view(reinterpret_cast<const char *>(hashedPassword.data()),
                       hashedPassword.size()),
      salt, Botan::Cipher_Dir::DECRYPTION);

  // Initialize secure vector with data and decrypt
  Botan::secure_vector<uint8_t> decryptedData(ciphertext.begin(),
                                              ciphertext.end());

  try {
    aead->finish(decryptedData);

  } catch (const std::exception &e) {
    std::cerr << "Invalid password\n" << e.what() << std::endl;
    return Botan::secure_vector<unsigned char>();
  }

  // Hash the decrypted ciphertext
  auto hashedCiphertextFunction =
      Botan::HashFunction::create_or_throw("SHA-3(256)");
  Botan::secure_vector<uint8_t> hashedCiphertext =
      hashedCiphertextFunction->process(decryptedData);

  // Verify the hash
  if (!std::equal(hashedCiphertext.begin(), hashedCiphertext.end(),
                  hash.begin()) == false) {
    throw std::runtime_error(
        "Hash verification failed, data may be tampered with.");
  }

  return decryptedData;
}

// Writes encrypted data to yaml file
bool writeToYaml(const std::string &filename,
                 const std::vector<uint8_t> dataToWrite) {

  std::ofstream outFile(filename, std::ios::binary);

  if (!outFile) {
    std::cerr << "Error opening file" << std::endl;
  }
  outFile.write(reinterpret_cast<const char *>(dataToWrite.data()),
                dataToWrite.size());

  outFile.close();

  return true;
}

// Reads encrypted data from yaml file
std::vector<uint8_t> readFromYaml(const std::string &filename) {
  std::ifstream inFile(filename, std::ios::binary);

  if (!inFile) {
    std::cerr << "Error opening file" << std::endl;
  }
  std::vector<uint8_t> buffer;

  // Find size of data and resize buffer
  inFile.seekg(0, std::ios::end);
  std::streamsize dataSize = inFile.tellg();
  inFile.seekg(0, std::ios::beg);
  buffer.resize(dataSize);

  inFile.read(reinterpret_cast<char *>(buffer.data()), dataSize);

  inFile.close();

  return buffer;
}

int main() {
  Botan::AutoSeeded_RNG
      randomSeed; // Generate random seed to create random salt
  std::string filename = "test.yaml";

  const std::string password = setPassword();
  const std::string message = setMessage();
  system("clear");

  // Encrypt user password and message
  const auto ciphertext = encrypt(
      password, randomSeed,
      std::span<const uint8_t>(
          reinterpret_cast<const uint8_t *>(message.data()), message.size()));

  std::cout << "Here is your message in ciphertext: "
            << Botan::hex_encode(ciphertext) << "\n\n";

  // Write encryption to file
  writeToYaml(filename, ciphertext);

  // Extract encrypted data from file
  std::vector<uint8_t> data = readFromYaml(filename);

  // Reconfirm user password before decryption
  std::string retypedPassword = "";
  std::cout << "Retype your password to decrypt the data: ";
  std::cin >> retypedPassword;
  std::cout << "\n";

  // Decrypt data if password matches
  const auto decryptedMessage = decrypt(retypedPassword, data);

  try {
    BOTAN_ASSERT_NOMSG(
        message.size() == decryptedMessage.size() &&
        std::equal(message.begin(), message.end(), decryptedMessage.begin()));
  } catch (const std::exception &e) {
    std::cerr << "Decryption failed\n" << e.what() << std::endl;
    return 1;
  }
  std::cout << "Here is your decrypted message: "
            << std::string(decryptedMessage.begin(), decryptedMessage.end())
            << "\n";

  return 0;
}