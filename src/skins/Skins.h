#ifndef _SKINS_H_
#define _SKINS_H_

#include <cstddef>
#include <cstdint>

namespace skins {
    uint8_t* GetSkin(const char* name, size_t& len);
}

#endif  // _SKINS_H_
