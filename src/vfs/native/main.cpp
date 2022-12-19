#include <iostream>

#include "argparse.h"

using namespace std;
using argparse::ArgumentParser;

constexpr const char* ARGUMENT_IMAGE = "image";

int main(int argc, const char** argv) {
    ArgumentParser program("vfs");
    program.add_description("Edit a card image.");
    program.add_argument(ARGUMENT_IMAGE).help("image file").required();

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
}
