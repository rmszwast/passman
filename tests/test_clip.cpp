/* #include "../src/cli/create.hpp"
#include "../src/utils/error.hpp"
#include "../src/utils/getPass.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/trompeloeil.hpp>

namespace clipTests {
class getPass_mock : public getPass_interface
{
  public:
    MAKE_MOCK1(getPass, std::string(std::string), override);
};

namespace fs = std::filesystem;
std::string path = "test-vault";
getPass_mock mock;

TEST_CASE("Given a preexisting vault, a vault is not created and an error "
          "message is printed to stderr",
          "[create tests]")
{
    // Fail if vault at path exists prior to starting tests
    if (fs::exists(fs::path{ path })) {
        printErr(path + " already exists. Remove before starting tests.");
        exit(EXIT_FAILURE);
    }
    // Create preexisting vault
    std::ofstream ofs;
    ofs.open(path);
    // Redirect stderr to stringstream
    std::stringstream local;
    auto cerr_buff = std::cerr.rdbuf();
    std::cerr.rdbuf(local.rdbuf());

    createVault(path, mock);
    REQUIRE(local.str() == stringErr(path + ": file already exists\n"));

    // Restore stderr
    std::cerr.rdbuf(cerr_buff);
    // Delete vault
    fs::remove(fs::path{ path });
}

TEST_CASE("Given a nonexisting vault, test branching logic", "[create tests]")
{
    // Redirect stderr to stringstream
    std::stringstream local;
    auto cerr_buff = std::cerr.rdbuf();
    std::cerr.rdbuf(local.rdbuf());

    SECTION("Given Matching passwords")
    {
        SECTION("Given a valid path, vault is created and success message is "
                "printed to stdout")
        {
            // Redirect stdout to stringstream
            std::stringstream local;
            auto cout_buff = std::cout.rdbuf();
            std::cout.rdbuf(local.rdbuf());
            {
                REQUIRE_CALL(mock, getPass("Enter password to encrypt vault"))
                  .RETURN("pa$$w0rd");
                REQUIRE_CALL(mock, getPass("Re-enter password"))
                  .RETURN("pa$$w0rd");
                createVault(path, mock);
            }
            REQUIRE(fs::exists(fs::path{ path }));
            REQUIRE(local.str() == path + " successfully created\n");
            std::cout.rdbuf(cout_buff);
            fs::remove(fs::path{ path });
        }

        SECTION("Given an invalid path, a vault is not created and an error "
                "message is printed to stderr")
        {
            std::string path = "test-vault/";
            {
                REQUIRE_CALL(mock, getPass("Enter password to encrypt vault"))
                  .RETURN("pa$$w0rd");
                REQUIRE_CALL(mock, getPass("Re-enter password"))
                  .RETURN("pa$$w0rd");
                createVault(path, mock);
            }
            REQUIRE(!fs::exists(fs::path{ path }));
            REQUIRE(local.str() == stringErr(path + ": invalid path\n"));
        }
    }

    SECTION("Given different passwords, a vault is not created and an error "
            "message is printed to stderr")
    {
        {
            REQUIRE_CALL(mock, getPass("Enter password to encrypt vault"))
              .RETURN("pa$$w0rd111");
            REQUIRE_CALL(mock, getPass("Re-enter password"))
              .RETURN("pa$$w0rd222");
            createVault(path, mock);
        }
        REQUIRE(!fs::exists(fs::path{ path }));
        REQUIRE(local.str() == stringErr("passwords do not match\n"));
    }

    // Restore stderr
    std::cerr.rdbuf(cerr_buff);
}
} */