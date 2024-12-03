#include "session_file.h"

#include <cstdint>
#include <iostream>
#include <memory>

#include "CPCrc.h"
#include "rle.h"

using namespace std;

namespace {
    constexpr uint32_t MAGIC = 0x19800819;

    uint32_t read32(const Buffer& buffer, size_t& cursor, bool& err) {
        if (cursor + 4 > buffer.size) {
            err = true;
            return 0;
        }

        const auto data = reinterpret_cast<const uint8_t*>(buffer.data);
        const uint32_t result = data[cursor] | (data[cursor + 1] << 8) | (data[cursor + 2] << 16) |
                                (data[cursor + 3] << 24);

        cursor += 4;

        return result;
    }

    bool sessionFile_read_v0(const Buffer sessionData, Buffer* nor, Buffer* nand, Buffer* ram) {
        size_t cursor = 8;
        bool err = false;

        const auto data = reinterpret_cast<uint8_t*>(sessionData.data);

        const uint32_t sizeMetadata = read32(sessionData, cursor, err);

        const uint32_t sizeNorCompressed = read32(sessionData, cursor, err);
        const uint32_t crcNor = read32(sessionData, cursor, err);

        const uint32_t sizeRamCompressed = read32(sessionData, cursor, err);
        const uint32_t crcRam = read32(sessionData, cursor, err);

        const uint32_t sizeNandCompressed = read32(sessionData, cursor, err);
        const uint32_t crcNand = read32(sessionData, cursor, err);

        if (err) {
            cerr << "unable to read session image v0 TOC" << endl;
            return false;
        }

        if (sessionData.size !=
            cursor + sizeMetadata + sizeNorCompressed + sizeRamCompressed + sizeNandCompressed) {
            cerr << "session image v0 size mismatch" << endl;
            return false;
        }

        cursor += sizeMetadata;

        size_t sizeNor;
        unique_ptr<uint8_t[]> dataNor;

        if (!rle_decode({.size = sizeNorCompressed, .data = data + cursor}, dataNor, sizeNor)) {
            cerr << "session image v0: failed to decompress NOR: " << rle_getLastError() << endl;
            return false;
        }

        if (crc::CRC32(dataNor.get(), sizeNor) != crcNor) {
            cerr << "session image v0: NOR CRC mismatch" << endl;
            return false;
        }

        cursor += sizeNorCompressed;

        size_t sizeRam;
        unique_ptr<uint8_t[]> dataRam;

        if (!rle_decode({.size = sizeRamCompressed, .data = data + cursor}, dataRam, sizeRam)) {
            cerr << "session image v0: failed to decompress RAM: " << " rle_getLastError()" << endl;
            return false;
        }

        if (crc::CRC32(dataRam.get(), sizeRam) != crcRam) {
            cerr << "session image v0: RAM CRC mismatch" << endl;
            return false;
        }

        cursor += sizeRamCompressed;

        size_t sizeNand;
        unique_ptr<uint8_t[]> dataNand;

        if (!rle_decode({.size = sizeNandCompressed, .data = data + cursor}, dataNand, sizeNand)) {
            cerr << "session image v0: failed to decompress NAND: " << rle_getLastError() << endl;
            return false;
        }

        if (crc::CRC32(dataNand.get(), sizeNand) != crcNand) {
            cerr << "session image v0: NAND CRC mismatch" << endl;
            return false;
        }

        nor->size = sizeNor;
        nor->data = dataNor.release();

        ram->size = sizeRam;
        ram->data = dataRam.release();

        nand->size = sizeNand;
        nand->data = dataNand.release();

        return true;
    }
}  // namespace

bool sessionFile_read(const Buffer sessionData, Buffer* nor, Buffer* nand, Buffer* ram,
                      Buffer* savestate) {
    size_t cursor = 0;
    bool err = false;

    const uint32_t magic = read32(sessionData, cursor, err);
    const uint32_t version = read32(sessionData, cursor, err);

    if (err) {
        cerr << "bad file header" << endl;
        return false;
    }

    if (magic != MAGIC) {
        cerr << "bad magic" << endl;
        return false;
    }

    if (version == 0) {
        savestate->size = 0;
        savestate->data = nullptr;

        return sessionFile_read_v0(sessionData, nor, nand, ram);
    }

    cerr << "invalid session image version " << version << endl;
    return false;
}

bool isSessionFile(const struct Buffer sessionData) {
    size_t cursor = 0;
    bool err = false;

    const uint32_t magic = read32(sessionData, cursor, err);
    const uint32_t version = read32(sessionData, cursor, err);

    return !err && magic == MAGIC && version == 0;
}
