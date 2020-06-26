#ifndef _CLOUDPILOT_H_
#define _CLOUDPILOT_H_

class Cloudpilot {
   public:
    void* malloc(long size);
    void free(void* buffer);
};

#endif  // _CLOUDPILOT_H_
