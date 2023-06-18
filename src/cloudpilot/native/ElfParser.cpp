#include "ElfParser.h"

#include <cstring>

using namespace std;

namespace {
    constexpr uint32_t ELF_MAGIC = 0x7f454c46;
    constexpr uint8_t ELF_CLASS_32 = 1;
    constexpr uint8_t ELF_ENDIAN_BE = 2;
    constexpr uint8_t ELF_VERSION = 1;
    constexpr uint32_t SECTION_TYPE_STRTAB = 0x03;
}  // namespace

ElfParser::EInvalidElf::EInvalidElf(const string& reason) : reason(reason) {}

const string& ElfParser::EInvalidElf::GetReason() const { return reason; }

void ElfParser::Parse(const uint8_t* elfData, size_t size) {
    data = elfData;
    this->size = size;

    bigEndian = true;
    sections.resize(0);

    try {
        if (Read32(0x00) != ELF_MAGIC) throw EInvalidElf("bad magic");
        if (Read8(0x04) != ELF_CLASS_32) throw EInvalidElf("not 32bit ELF");
        if (Read8(0x06) != ELF_VERSION || Read32(0x14) != ELF_VERSION)
            throw EInvalidElf("invalid ELF version");

        bigEndian = Read8(0x05) == ELF_ENDIAN_BE;
        machine = Read16(0x12);
        entrypoint = Read32(0x18);

        uint32_t sectionTableOffset = Read32(0x20);
        uint16_t sectionHeaderSize = Read16(0x2e);
        uint16_t numSections = Read16(0x30);

        sections.reserve(numSections);

        for (uint32_t i = 0; i < numSections; i++)
            sections.push_back(ReadSection(sectionTableOffset + i * sectionHeaderSize));

        uint16_t shstrtabIndex = Read16(0x32);
        if (shstrtabIndex >= sections.size() ||
            sections[shstrtabIndex].sectionType != SECTION_TYPE_STRTAB)
            throw EInvalidElf("unable to identify .shstrtab");

        const Section& shrstrtab(sections[shstrtabIndex]);

        for (Section& section : sections) {
            uint32_t nameOffset = shrstrtab.offset + section.nameOffset;
            if (nameOffset >= size) throw EInvalidElf("bad section name");

            const char* name = reinterpret_cast<const char*>(data + nameOffset);

            if (data[nameOffset + strnlen(name, shrstrtab.size - section.nameOffset)] != '\0')
                throw new EInvalidElf("unterminated section name");

            section.name = name;
        }
    } catch (const EInvalidElf& e) {
        throw EInvalidElf("failed to parse ELF: " + e.GetReason());
    }
}

const uint8_t* ElfParser::GetData() const { return data; }

size_t ElfParser::GetSize() const { return size; }

uint16_t ElfParser::GetMachine() const { return machine; }

uint32_t ElfParser::GetEntrypoint() const { return entrypoint; }

const std::vector<ElfParser::Section>& ElfParser::GetSections() const { return sections; }

uint8_t ElfParser::Read8(uint32_t offset) {
    if (offset >= size) throw EInvalidElf("reference beyond bounds");

    return data[offset];
}

uint16_t ElfParser::Read16(uint32_t offset) {
    return bigEndian ? ((Read8(offset) << 8) | Read8(offset + 1))
                     : ((Read8(offset + 1) << 8) | Read8(offset));
}

uint32_t ElfParser::Read32(uint32_t offset) {
    return bigEndian ? ((Read8(offset) << 24) | (Read8(offset + 1) << 16) |
                        (Read8(offset + 2) << 8) | Read8(offset + 3))
                     : ((Read8(offset + 3) << 24) | (Read8(offset + 2) << 16) |
                        (Read8(offset + 1) << 8) | Read8(offset));
}

ElfParser::Section ElfParser::ReadSection(uint32_t offset) {
    Section section;

    try {
        section.nameOffset = Read32(offset);
        section.sectionType = Read32(offset + 0x04);
        section.virtualAddress = Read32(offset + 0x0c);
        section.size = Read32(offset + 0x14);
        section.offset = Read32(offset + 0x10);

        if (section.offset + section.size >= size) throw EInvalidElf("section exceeds bounds");
    } catch (const EInvalidElf& e) {
        throw EInvalidElf("failed to parse section: " + e.GetReason());
    }

    return section;
}
