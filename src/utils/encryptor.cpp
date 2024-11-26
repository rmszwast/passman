#include "encryptor.hpp"
#include <cstdint>

Encryptor::Encryptor() {};

// Derive key for encryption algorithm using salt
Botan::secure_vector<uint8_t>
Encryptor::deriveKey(std::string_view password,
                     std::span<const uint8_t> salt,
                     size_t keyLength)
{
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
    pbkdfObject->derive_key(key.data(),
                            key.size(),
                            password.data(),
                            password.length(),
                            salt.data(),
                            SALT_STR_LENGTH);

    return key;
}

// Set up environment for encryption
std::unique_ptr<Botan::AEAD_Mode>
Encryptor::setUpAEAD(std::string_view password,
                     std::span<const uint8_t> salt,
                     Botan::Cipher_Dir selectedMode)
{
    // Create instance of AEAD obeject
    auto aead = Botan::AEAD_Mode::create_or_throw("AES-256/GCM", selectedMode);
    BOTAN_ASSERT_NONNULL(aead);

    // Retrieve key and nonce length for specifid encryption algorithm
    const size_t keyLength = aead->key_spec().maximum_keylength();
    const size_t nonceLength = aead->default_nonce_length();

    // Generate key based on parameters for specified encryption algorithm and
    // password + salt
    const auto keyData = deriveKey(password, salt, (keyLength + nonceLength));
    BOTAN_ASSERT_NOMSG(keyData.size() == keyLength + nonceLength);

    // Extract key and nonce
    const auto key = std::span{ keyData }.first(keyLength);
    const auto nonce = std::span{ keyData }.last(nonceLength);

    // Initialize AEAD object to start encryption or decryption
    aead->set_key(key.data(), key.size());
    aead->start(nonce.data(), nonce.size());

    return aead;
}

// Encrypt data
std::vector<uint8_t>
Encryptor::encrypt(std::string_view password,
                   Botan::secure_vector<uint8_t> ciphertext)
{

    Botan::AutoSeeded_RNG randomNum = generateRandomSeed();

    const auto salt = randomNum.random_vec(
      SALT_STR_LENGTH); // Generate salt based on random value

    // Hash password using SHA-3(256)
    auto hashedPasswordFunction =
      Botan::HashFunction::create_or_throw("SHA-3(256)");
    Botan::secure_vector<uint8_t> hashedPassword =
      hashedPasswordFunction->process(
        reinterpret_cast<const uint8_t*>(password.data()), password.size());

    // Set up AEAD environment for encryption
    auto aead = setUpAEAD(
      std::string_view(reinterpret_cast<const char*>(hashedPassword.data()),
                       hashedPassword.size()),
      salt,
      Botan::Cipher_Dir::Encryption);

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
    hashedSaltAndCipherResult.insert(
      hashedSaltAndCipherResult.end(), salt.begin(), salt.end());
    hashedSaltAndCipherResult.insert(
      hashedSaltAndCipherResult.end(), ciphertext.begin(), ciphertext.end());
    hashedSaltAndCipherResult.insert(hashedSaltAndCipherResult.end(),
                                     hashedCiphertext.begin(),
                                     hashedCiphertext.end());

    return hashedSaltAndCipherResult;
}

Botan::AutoSeeded_RNG
Encryptor::generateRandomSeed()
{
    return Botan::AutoSeeded_RNG();
}

// Decrypt data
Botan::secure_vector<uint8_t>
Encryptor::decrypt(std::string_view password,
                   std::span<const uint8_t> encryptedData)
{

    // If ciphertext is less than salt length, cannot authenticate, throw error
    if (encryptedData.size() < SALT_STR_LENGTH + 32) {
        throw std::invalid_argument("Encrypted data is wrong length");
    }

    // Salt is at beginning of encrypted data, extract
    const auto salt = encryptedData.first(SALT_STR_LENGTH);

    const auto ciphertext =
      encryptedData.subspan(SALT_STR_LENGTH,
                            encryptedData.size() - SALT_STR_LENGTH -
                              32); // Extract ciphertext (32 bytes for SHA-3)
    const auto hash = encryptedData.last(32);

    // Hash the password to derive the key
    auto hashedPasswordFunction =
      Botan::HashFunction::create_or_throw("SHA-3(256)");
    Botan::secure_vector<uint8_t> hashedPassword =
      hashedPasswordFunction->process(
        reinterpret_cast<const uint8_t*>(password.data()), password.size());

    // Set up AEAD environment for decryption
    auto aead = setUpAEAD(
      std::string_view(reinterpret_cast<const char*>(hashedPassword.data()),
                       hashedPassword.size()),
      salt,
      Botan::Cipher_Dir::Decryption);

    // Initialize secure vector with data and decrypt
    Botan::secure_vector<uint8_t> decryptedData(ciphertext.begin(),
                                                ciphertext.end());

    // Attempt decryption and authentication
    try {
        aead->finish(decryptedData);
    } catch (const Botan::Integrity_Failure& e) {
        std::cerr << "Invalid password or data may be tampered with: "
                  << e.what() << std::endl;
        throw;
    } catch (const Botan::Exception& e) {
        std::cerr << "Botan error: " << e.what() << std::endl;
        throw;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        throw;
    }

    // Hash the decrypted ciphertext
    auto hashedCiphertextFunction =
      Botan::HashFunction::create_or_throw("SHA-3(256)");
    Botan::secure_vector<uint8_t> hashedCiphertext =
      hashedCiphertextFunction->process(decryptedData);

    // Verify the hash
    if (!std::equal(hashedCiphertext.begin(),
                    hashedCiphertext.end(),
                    hash.begin()) == false) {
        throw std::runtime_error(
          "Hash verification failed, data may be tampered with.");
    }

    return decryptedData;
}