#ifndef _LOG_DOMAIN_H_
#define _LOG_DOMAIN_H_

#include <cstdint>

namespace logging {

    enum Domain : uint32_t { domainNetlib = 0x1, domainDebugger = 0x2, domainInstaller = 0x04 };

};

#endif  //  _LOG_DOMAIN_H_