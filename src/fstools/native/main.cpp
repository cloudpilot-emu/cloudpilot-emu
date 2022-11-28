#include <iostream>

#include "argparse.h"

using namespace std;
using argparse::ArgumentParser;

const char* SUBCOMMAND_FSCK = "fsck";
const char* SUBCOMMAND_FORMAT = "format";
const char* ARGUMENT_IMAGE = "image";

void runFsck(const ArgumentParser& cmd) { cout << "fsck not implemented" << endl; }

void runFormat(const ArgumentParser& cmd) { cout << "format not implemented" << endl; }

int main(int argc, const char** argv) {
    ArgumentParser program("fstools");
    program.add_description("Check and format card images.");

    ArgumentParser fsckCommand(SUBCOMMAND_FSCK);
    fsckCommand.add_description("fsck card image");
    fsckCommand.add_argument(ARGUMENT_IMAGE).help("card image").required();

    ArgumentParser formatCommand(SUBCOMMAND_FORMAT);
    formatCommand.add_description("format card image (big floppy)");
    formatCommand.add_argument(ARGUMENT_IMAGE).help("card image").required();

    program.add_subparser(fsckCommand);
    program.add_subparser(formatCommand);

    try {
        program.parse_args(argc, argv);
    } catch (const runtime_error& e) {
        cerr << e.what() << endl << endl;
        cerr << program;

        exit(1);
    }

    if (program.is_subcommand_used(SUBCOMMAND_FSCK))
        runFsck(fsckCommand);
    else if (program.is_subcommand_used(SUBCOMMAND_FORMAT))
        runFormat(formatCommand);
    else
        cout << program;
}
