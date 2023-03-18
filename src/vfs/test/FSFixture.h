#ifndef _VFS_TEST_FS_FIXTURE_
#define _VFS_TEST_FS_FIXTURE_

#include <cstdint>
#include <string>

namespace FSFixture {
    void CreateAndMount(uint32_t slot = 0);

    void UnmountAndRelease(uint32_t slot = 0);

    void CreateFile(const std::string& path, const std::string& content);
}  // namespace FSFixture

#endif  // _VFS_TEST_FS_FIXTURE_
