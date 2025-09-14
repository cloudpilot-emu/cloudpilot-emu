#ifndef _UARM_WEB_BRIDGE_
#define _UARM_WEB_BRIDGE_

class Bridge {
   public:
    Bridge() = default;

    static void* Malloc(unsigned int size);
    static void Free(void* ptr);
};

#endif  // _UARM_WEB_BRIDGE_