#ifndef _ELF_PARSER_H_
#define _ELF_PARSER_H_

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

class ElfParser {
   public:
    class EInvalidElf {
        friend ElfParser;

       public:
        const std::string& GetReason() const;

       private:
        EInvalidElf(const std::string& reason);

       private:
        std::string reason;
    };

    struct Section {
        uint32_t nameOffset;
        std::string name;

        uint32_t sectionType;

        uint32_t virtualAddress;
        uint32_t size;

        uint32_t offset;
    };

   public:
    ElfParser() = default;

    void Parse(const uint8_t* elfData, size_t size);

    const uint8_t* GetData() const;
    size_t GetSize() const;

    uint16_t GetMachine() const;
    uint32_t GetEntrypoint() const;

    const std::vector<Section>& GetSections() const;
    const std::optional<Section> GetSection(const std::string& name) const;

   private:
    uint8_t Read8(uint32_t offset);
    uint16_t Read16(uint32_t offset);
    uint32_t Read32(uint32_t offset);

    Section ReadSection(uint32_t offset);

   private:
    const uint8_t* data{nullptr};
    size_t size;

    bool bigEndian{true};

    uint16_t machine;
    uint32_t entrypoint;

    std::vector<Section> sections;
};

#endif  // _ELF_PARSER_H_
