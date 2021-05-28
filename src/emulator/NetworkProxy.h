#ifndef _NETWORK_PROXY_H_
#define _NETWORK_PROXY_H_

#include "EmCommon.h"

class NetworkProxy {
   public:
    NetworkProxy() = default;

    void Open();

    Err Close();

    int OpenCount();

   private:
    void ConnectSuccess();
    void ConnectAbort();

   private:
    int openCount{0};

   private:
    NetworkProxy(const NetworkProxy&) = delete;
    NetworkProxy(NetworkProxy&&) = delete;
    NetworkProxy& operator=(const NetworkProxy&) = delete;
    NetworkProxy& operator=(NetworkProxy&&) = delete;
};

extern NetworkProxy& gNetworkProxy;

#endif  // _NETWORK_PROXY_H_
