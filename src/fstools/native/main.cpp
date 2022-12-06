#include <iostream>

#include "CmdFsck.h"
#include "argparse.h"
#include "cli.h"

using namespace std;
using argparse::ArgumentParser;

void runFormat(const ArgumentParser& cmd) { cout << "format not implemented" << endl; }

int main(int argc, const char** argv) {
    ArgumentParser program("fstools");
    program.add_description("Check and format card images.");

    ArgumentParser fsckCommand(SUBCOMMAND_FSCK);
    fsckCommand.add_description("fsck card image");
    fsckCommand.add_argument(ARGUMENT_IMAGE).help("card image").required();
    fsckCommand.add_argument("--write", "-w").help("write (potentially) modified image to file");

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

        return 1;
    }

    if (program.is_subcommand_used(SUBCOMMAND_FSCK))
        return CmdFsk(fsckCommand).Run() ? 0 : 1;
    else if (program.is_subcommand_used(SUBCOMMAND_FORMAT))
        runFormat(formatCommand);
    else
        cout << program;

    return 0;
}
