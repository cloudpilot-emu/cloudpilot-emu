#ifndef _VFS_CLI_H_
#define _VFS_CLI_H_

#include <string>

#include "CardImage.h"
#include "CardVolume.h"

class VfsCli {
   public:
    VfsCli(const std::string& imageFile);

    bool Run();

   private:
    const std::string imageFile;
};

#endif  // _VFS_CLI_H_
