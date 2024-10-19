#include "yamlex.h"
#include <CLI/CLI.hpp>
#include <string>

int
main(int argc, char** argv)
{
    std::string input_file_name, output_file_name;
    int level{ 5 }, subopt{ 0 };

    // app caption
    CLI::App app{ "CLI11 help" };

    app.require_subcommand(1);

    // Subcommand for encoding
    CLI::App* const encode =
      app.add_subcommand("e", "encode")->ignore_case(); // ignore case
    encode->add_option("input", input_file_name, "input file")
      ->option_text(" ")
      ->required()
      ->check(CLI::ExistingFile); // file must exist
    encode->add_option("output", output_file_name, "output file")
      ->option_text(" ")
      ->required(); // required option
    encode->add_option("-l, --level", level, "encoding level")
      ->option_text("[1..9]")
      ->check(CLI::Range(1, 9))
      ->default_val(5); // limit parameter range
    encode->add_flag("-R, --remove",
                     "remove input file"); // no parameter option
    encode->add_flag("-s, --suboption", subopt, "suboption")->option_text(" ");

    // Subcommand for decoding
    CLI::App* const decode = app.add_subcommand("d", "decode")->ignore_case();
    decode->add_option("input", input_file_name, "input file")
      ->option_text(" ")
      ->required()
      ->check(CLI::ExistingFile);
    decode->add_option("output", output_file_name, "output file")
      ->option_text(" ")
      ->required();

    // Subcommand for creating and loading vault
    CLI::App* const example =
      app.add_subcommand("example", "create and load a vault");

    // Usage message modification
    std::string usage_msg = "Usage: " + std::string(argv[0]) +
                            " <command> [options] <input-file> <output-file>";
    app.usage(usage_msg);

    // flag to display full help at once
    app.set_help_flag("");
    app.set_help_all_flag("-h, --help");

    CLI11_PARSE(app, argc, argv);

    // Check if the 'example' subcommand was invoked
    if (*example) {
        createAndLoadVault(); // Call the function
        return 0;             // Exit after running the function
    }

    return 0; // Default return
}