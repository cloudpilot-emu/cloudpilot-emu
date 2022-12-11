#include <iostream>

#include "CmdFsck.h"
#include "CmdMkfs.h"
#include "argparse.h"
#include "cli.h"

using namespace std;
using argparse::ArgumentParser;

int main(int argc, const char** argv) {
    ArgumentParser program("fstools");
    program.add_description("Check and format card images.");

    ArgumentParser fsckCommand(SUBCOMMAND_FSCK);
    fsckCommand.add_description("fsck card image");
    fsckCommand.add_argument(ARGUMENT_IMAGE).help("card image").required();
    fsckCommand.add_argument("--write", "-w").help("write (potentially) modified image to file");

    ArgumentParser mkfsCommand(SUBCOMMAND_MKFS);
    mkfsCommand.add_description("create new, formatted card image");
    mkfsCommand.add_argument(ARGUMENT_SIZE)
        .help("image size (4|8|16|32|64|128)")
        .required()
        .scan<'u', unsigned int>();
    mkfsCommand.add_argument(ARGUMENT_IMAGE).help("image file").required();

    program.add_subparser(fsckCommand);
    program.add_subparser(mkfsCommand);

    try {
        program.parse_args(argc, argv);
    } catch (const runtime_error& e) {
        cerr << e.what() << endl << endl;
        cerr << program;

        return 1;
    } catch (const invalid_argument& e) {
        cerr << e.what() << endl << endl;
        cerr << program;

        return 1;
    }

    if (program.is_subcommand_used(SUBCOMMAND_FSCK))
        return CmdFsk(fsckCommand).Run() ? 0 : 1;
    else if (program.is_subcommand_used(SUBCOMMAND_MKFS))
        return CmdMkfs(mkfsCommand).Run() ? 0 : 1;
    else
        cout << program;

    return 0;
}
