#ifndef _VFS_H_
#define _VFS_H_

class Vfs {
   public:
    void* Malloc(long size);
    void Free(void* buffer);
    void* Nullptr();
};

#endif  // _VFS_H_
