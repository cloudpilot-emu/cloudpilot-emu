#ifndef _CMD_MKFS_H_
#define _CMD_MKFS_H_

#include <string>

#include "argparse.h"

class CmdMkfs {
   public:
    explicit CmdMkfs(const argparse::ArgumentParser& cmd);

    bool Run();

   private:
    unsigned int size;
    std::string imageFile;
};

#endif  // _CMD_MKFS_H_
