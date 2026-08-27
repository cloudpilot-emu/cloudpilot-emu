#include "pv_display.h"

#include <cstdio>

#include "CPEndian.h"
#include "Nibbler.h"
#include "RAM.h"
#include "ROM.h"
#include "cputil.h"
#include "mem.h"
#include "memory_buffer.h"
#include "savestate/savestateAll.h"

#define DISPLAY_BASE 0x30000200
#define DISPLAY_SIZE 20
#define DISPLAY_OFFSET_BASE 0
#define DISPLAY_OFFSET_STRIDE 4
#define DISPLAY_OFFSET_DEPTH 8
#define DISPLAY_OFFSET_CLUT 12
#define DISPLAY_OFFSET_SPEC 16

#define RAM_BASE 0x20000000

#define SAVESTATE_VERSION 0

struct PvDisplay {
    uint32_t base{0};
    uint32_t stride{0};

    uint8_t depth{0};
    bool dirty{true};

    uint32_t width;
    uint32_t height;
    uint32_t density;

    MemoryBuffer* bufferClut{nullptr};
    ArmRam* ram{nullptr};
    ArmRom* rom{nullptr};

    template <typename T>
    void DoSaveLoad(T& chunkHelper) {
        chunkHelper.Do32(base).Do32(stride).Do8(depth).Do32(width).Do32(height).Do32(density);
    }
};

static uint32_t unpack_rgb16(uint16_t rgb16) {
    uint8_t r = (rgb16 >> 11) & 0x1f;
    uint8_t g = (rgb16 >> 5) & 0x3f;
    uint8_t b = (rgb16 >> 0) & 0x1f;

    r = (r << 3) | (r >> 2);
    g = (g << 2) | (g >> 4);
    b = (b << 3) | (b >> 2);

    return 0xff000000 | (b << 16) | (g << 8) | r;
}

static void updateFramebufferLocation(PvDisplay* display) {
    if (display->base > RAM_BASE && display->stride > 0) {
        ramSetFramebuffer(display->ram, display->base, display->stride * display->height);
    } else {
        ramSetFramebuffer(display->ram, 0, 0);
    }
}

static bool pvDisplayPrvMemAccessF(void* userData, uint32_t pa, uint_fast8_t size, bool write,
                                   void* buf) {
    if (size != 4) {
        fprintf(stderr, "invalid access to display\n");
        return false;
    }

    auto display = reinterpret_cast<PvDisplay*>(userData);
    uint32_t& value = *reinterpret_cast<uint32_t*>(buf);

    switch ((pa - DISPLAY_BASE) >> 2) {
        case DISPLAY_OFFSET_BASE >> 2:
            if (write) {
                display->base = value & ~0x03;
                display->dirty = true;
                updateFramebufferLocation(display);
            } else {
                value = display->base;
            }

            break;

        case (DISPLAY_OFFSET_STRIDE >> 2):
            if (write) {
                display->stride = value & ~0x03;
                display->dirty = true;
                updateFramebufferLocation(display);

            } else {
                value = display->stride;
            }

            break;

        case (DISPLAY_OFFSET_DEPTH >> 2):
            if (write) {
                if (value <= 4) {
                    display->depth = value;
                    display->dirty = true;
                } else {
                    fprintf(stderr, "invalid display depth %u\n", value);
                    return false;
                }
            } else {
                display->depth = value;
            }

            break;

        case (DISPLAY_OFFSET_CLUT >> 2): {
            if (!write) return false;

            const uint8_t index = value >> 24;
            auto clut = reinterpret_cast<uint32_t*>(display->bufferClut->buffer);

            clut[index] = bswap32((value << 8) | 0xff);
            MEMORY_BUFFER_MARK_DIRTY(*(display->bufferClut), index << 2);

            break;
        }

        case (DISPLAY_OFFSET_SPEC >> 2):
            if (write) return false;

            value = (((display->width >> 2) - 1) & 0x1ff) |
                    ((((display->height >> 2) - 1) & 0x1ff) << 9) |
                    (((display->density / 36 - 1) & 0x07) << 18);

            break;

        default:
            return false;
    }

    return true;
}

PvDisplay* pvDisplayInit(ArmMem* mem, ArmRam* ram, ArmRom* rom, MemoryBuffer* bufferClut,
                         uint32_t width, uint32_t height, uint32_t density) {
    auto display = new PvDisplay();

    display->width = width;
    display->height = height;
    display->density = density;

    display->ram = ram;
    display->rom = rom;
    display->bufferClut = bufferClut;

    memRegionAdd(mem, DISPLAY_BASE, DISPLAY_SIZE, pvDisplayPrvMemAccessF, display);

    return display;
}

template <int bpp>
static bool pvDisplayRenderFramebufferIndexed(PvDisplay* display, uint32_t* target) {
    const size_t framebufferSize = display->stride * display->height;

    auto framebuffer =
        reinterpret_cast<uint8_t*>(ramResolveAddress(display->ram, display->base, framebufferSize));

    if (!framebuffer)
        framebuffer = reinterpret_cast<uint8_t*>(
            romResolveAddress(display->rom, display->base, framebufferSize));

    if (!framebuffer) return false;

    const auto clut = reinterpret_cast<uint32_t*>(display->bufferClut->buffer);

    const uint32_t lineBytes = (display->width * bpp) / 8;
    if (lineBytes < display->stride) return false;

    const uint32_t pitchDelta = display->stride - lineBytes;

    Nibbler<bpp, false, true> nibbler;
    if constexpr (bpp != 8) {
        nibbler.reset(framebuffer, 0);
    }

    for (uint32_t y = 0; y < display->height; y++) {
        for (uint32_t x = 0; x < display->width; x++) {
            if constexpr (bpp != 8) {
                *(target++) = clut[nibbler.nibble()];
            } else {
                *(target++) = clut[*(framebuffer++)];
            }
        }

        if constexpr (bpp != 8) {
            nibbler.skipBytes(pitchDelta);
        } else {
            framebuffer += pitchDelta;
        }
    }

    return true;
}

bool pvDisplayRenderFramebuffer(PvDisplay* display, uint32_t* target) {
    if (display->base == 0 || display->stride == 0) return false;

    switch (display->depth) {
        case 0:
            return pvDisplayRenderFramebufferIndexed<1>(display, target);

        case 1:
            return pvDisplayRenderFramebufferIndexed<2>(display, target);

        case 2:
            return pvDisplayRenderFramebufferIndexed<4>(display, target);

        case 3:
            return pvDisplayRenderFramebufferIndexed<8>(display, target);

        case 4: {
            const uint32_t lineBytes = display->width << 1;
            if (lineBytes < display->stride || display->stride & 0x01) return false;

            const uint32_t pitchDelta = (display->stride - lineBytes) >> 1;

            auto framebuffer = reinterpret_cast<uint16_t*>(
                ramResolveAddress(display->ram, display->base, display->stride * display->height));
            if (!framebuffer) return false;

            for (uint32_t y = 0; y < display->height; y++) {
                for (uint32_t x = 0; x < display->width; x++) {
                    *(target++) = unpack_rgb16(*(framebuffer++));
                }

                framebuffer += pitchDelta;
            }

            break;
        }

        default:
            break;
    }

    return true;
}

bool pvIsDirty(PvDisplay* display) { return display->dirty; }

void pvDisplayClearDirty(PvDisplay* display) { display->dirty = false; }

template <typename T>
void pvDisplaySave(struct PvDisplay* display, T& savestate) {
    auto chunk = savestate.GetChunk(ChunkType::pvDisplay, SAVESTATE_VERSION);
    if (!chunk) ERR("unable to allocate chunk");

    SaveChunkHelper helper(*chunk);
    display->DoSaveLoad(helper);
}

template <typename T>
void pvDisplayLoad(struct PvDisplay* display, T& loader) {
    const uint32_t oldWidth = display->width;
    const uint32_t oldHeight = display->height;
    const uint32_t oldDensity = display->density;

    auto chunk = loader.GetChunkOrFail(ChunkType::pvDisplay, SAVESTATE_VERSION, "pvDisplay");
    if (!chunk) return;

    LoadChunkHelper helper(*chunk);
    display->DoSaveLoad(helper);

    if (display->width != oldWidth || display->height != oldHeight ||
        display->density != oldDensity) {
        ERR("display geometry does not match after load \n");
    }
}

template void pvDisplaySave<Savestate<ChunkType>>(PvDisplay* display,
                                                  Savestate<ChunkType>& savestate);
template void pvDisplaySave<SavestateProbe<ChunkType>>(PvDisplay* display,
                                                       SavestateProbe<ChunkType>& savestate);
template void pvDisplayLoad<SavestateLoader<ChunkType>>(PvDisplay* display,
                                                        SavestateLoader<ChunkType>& loader);
