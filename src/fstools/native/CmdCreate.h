#ifndef _CMD_CREATE_H_
#define _CMD_CREATE_H_

#include <string>

#include "argparse.h"

class CmdCreate {
   public:
    explicit CmdCreate(const argparse::ArgumentParser& cmd);

    bool Run();

   private:
    unsigned int size;
    std::string imageFile;
};

#endif  // _CMD_CREATE_H_
