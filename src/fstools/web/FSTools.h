#ifndef _FSTOOLS_H_
#define _FSTOOLS_H_

class FSTools {
   public:
    void* Malloc(int size);
    void Free(void* buffer);
    void* Nullptr();
};

#endif  // _FSTOOLS_H_
