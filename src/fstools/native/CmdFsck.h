#ifndef _CMD_FSCK_H_
#define _CMD_FSCK_H_

#include <cstdint>
#include <memory>
#include <string>

#include "argparse.h"

class CmdFsk {
   public:
    explicit CmdFsk(const argparse::ArgumentParser& cmd);

    bool Run();

   private:
    std::string imageFile;
    std::unique_ptr<uint8_t[]> data;

    std::string writeFile;
};

#endif  // _CMD_FSCK_H_
