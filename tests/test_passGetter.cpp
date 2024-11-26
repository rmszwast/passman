#include "../src/utils/passGetter.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_templated.hpp>
#include <catch2/trompeloeil.hpp>
#include <iostream>

namespace getPassTests {
TEST_CASE("Given a call to getPass", "[getPass tests]")
{
    // Redirect stdin to stringstream
    std::stringstream ss_cin;
    auto cin_buff = std::cin.rdbuf();
    std::cin.rdbuf(ss_cin.rdbuf());

    // Redirect stdout to stringstream
    std::stringstream ss_cout;
    auto cout_buff = std::cout.rdbuf();
    std::cout.rdbuf(ss_cout.rdbuf());

    PassGetter passGetter;

    SECTION("getPass returns the input password on success")
    {
        std::string pass = "pa$$w0rd";
        ss_cin << pass;
        REQUIRE(passGetter.get("") == pass);
    }

    SECTION("getPass throws an error for password lengths > 4095 chars")
    {
        // Create input > 4095 chars and store to stringstream
        std::string pass;
        pass.append(4096, 'f');
        ss_cin << pass;

        REQUIRE_THROWS_AS(passGetter.get(""), std::runtime_error);
        std::cin.seekg(0);
        REQUIRE_THROWS_WITH(passGetter.get(""), "input is too large");
    }

    SECTION("getPass prints a prompt containing the argument received")
    {
        std::string pass = "pa$$w0rd";
        ss_cin << pass;
        std::string prompt = "Enter password to encrypt vault";
        passGetter.get(prompt);
        REQUIRE(ss_cout.str() == prompt + ": \n");
    }

    // Restore streams
    std::cin.rdbuf(cin_buff);
    std::cout.rdbuf(cout_buff);
}
}