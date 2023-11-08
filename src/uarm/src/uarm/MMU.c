//(c) uARM project    https://github.com/uARM-Palm/uARM    uARM@dmitry.gr

#include "MMU.h"

#include <stdlib.h>
#include <string.h>

#include "mem.h"
#include "util.h"

struct TlbEntry {
    uint32_t pa;

    uint32_t revision : 16;
    uint32_t ap : 2;
    uint32_t domain : 4;
    uint32_t c : 1;
    uint32_t section : 1;

    struct ArmMemRegion *region;
};

struct ArmMmu {
    struct ArmMem *mem;
    uint32_t transTablPA;
    uint8_t S : 1;
    uint8_t R : 1;
    uint8_t xscale : 1;
    uint32_t domainCfg;

    struct TlbEntry *tlb[256];
    uint16_t revision;
};

void mmuTlbFlush(struct ArmMmu *mmu) {
    mmu->revision++;

    if (mmu->revision == 0) {
        mmu->revision = 1;

        for (size_t i = 0; i < 256; i++) {
            struct TlbEntry *entries = mmu->tlb[i];
            if (!entries) continue;

            for (size_t i = 0; i < 4096; i++) entries[i].revision = 0;
        }
    }
}

void mmuReset(struct ArmMmu *mmu) {
    mmu->transTablPA = MMU_DISABLED_TTP;
    mmuTlbFlush(mmu);
}

struct ArmMmu *mmuInit(struct ArmMem *mem, bool xscaleMode) {
    struct ArmMmu *mmu = (struct ArmMmu *)malloc(sizeof(*mmu));

    if (!mmu) ERR("cannot alloc MMU");

    memset(mmu, 0, sizeof(*mmu));
    mmu->mem = mem;
    mmu->xscale = xscaleMode;
    mmuReset(mmu);

    return mmu;
}

bool mmuIsOn(struct ArmMmu *mmu) { return mmu->transTablPA != MMU_DISABLED_TTP; }

static inline struct TlbEntry *getTlbEntry(struct ArmMmu *mmu, uint32_t va) {
    const uint8_t i = va >> 24;
    struct TlbEntry *lvl1 = mmu->tlb[i];

    if (!lvl1) {
        lvl1 = mmu->tlb[i] = (struct TlbEntry *)malloc(4096 * sizeof(struct TlbEntry));
        memset(lvl1, 0, 4096 * sizeof(struct TlbEntry));
    }

    return lvl1 + ((va >> 12) & 0xfff);
}

static inline bool checkPermissions(struct ArmMmu *mmu, uint_fast8_t ap, uint_fast8_t domain,
                                    bool section, bool priviledged, bool write,
                                    uint_fast8_t *fsrP) {
    switch ((mmu->domainCfg >> (domain * 2)) & 3) {
        case 0:  // NO ACCESS:
        case 2:  // RESERVED: unpredictable	(treat as no access)
            if (fsrP)
                *fsrP = (section ? 0x08 : 0xB) | (domain << 4);  // section or page domain fault
            return false;

        case 1:  // CLIENT: check permissions
            break;

        case 3:  // MANAGER: allow all access
            return true;
    }

    // check permissions

    switch (ap) {
        case 0:
            if (write || (!mmu->R && (!priviledged || !mmu->S))) break;
            return true;

        case 1:
            if (!priviledged) break;
            return true;

        case 2:
            if (!priviledged && write) break;
            return true;

        case 3:
            return true;
    }

    // perm_err:
    if (fsrP)
        *fsrP = (section ? 0x0D : 0x0F) | (domain << 4);  // section or subpage permission fault
    return false;
}

static inline bool translateAndCache(struct ArmMmu *mmu, uint32_t adr, bool priviledged, bool write,
                                     uint32_t *paP, uint_fast8_t *fsrP, uint8_t *mappingInfoP,
                                     struct ArmMemRegion **region) {
    bool c = false;
    bool section = false, coarse = true, pxa_tex_page = false;
    uint32_t va, pa = 0, sz, t;
    int_fast16_t i;
    uint_fast8_t dom, ap = 0;

    // read first level table
    if (mmu->transTablPA & 3) {
        if (fsrP) *fsrP = 0x01;  // alignment fault
        return false;
    }

    if (!memAccess(mmu->mem, mmu->transTablPA + ((adr & 0xFFF00000ul) >> 18), 4,
                   MEM_ACCESS_TYPE_READ, &t)) {
        if (fsrP) *fsrP = 0x0C;  // translation external abort first level
        return false;
    }

    dom = (t >> 5) & 0x0F;
    switch (t & 3) {
        case 0:  // fault

            if (fsrP) *fsrP = 0x5;  // section translation fault
            return false;

        case 1:  // coarse pagetable

            t &= 0xFFFFFC00UL;
            t += (adr & 0x000FF000UL) >> 10;
            break;

        case 2:  // 1MB section

            pa = t & 0xFFF00000UL;
            va = adr & 0xFFF00000UL;
            sz = 1UL << 20;
            ap = (t >> 10) & 3;
            c = !!(t & 0x08);
            section = true;
            goto translated;

        case 3:  // fine page table

            coarse = false;
            t &= 0xFFFFF000UL;
            t += (adr & 0x000FFC00UL) >> 8;
            break;
    }

    // read second level table
    if (!memAccess(mmu->mem, t, 4, MEM_ACCESS_TYPE_READ, &t)) {
        if (fsrP) *fsrP = 0x0E | (dom << 4);  // translation external abort second level
        return false;
    }

    c = !!(t & 0x08);

    switch (t & 3) {
        case 0:  // fault

            if (fsrP) *fsrP = 0x07 | (dom << 4);  // page translation fault
            return false;

        case 1:  // 64K mapping

            pa = t & 0xFFFF0000UL;
            va = adr & 0xFFFF0000UL;
            sz = 65536UL;
            ap = (adr >> 14) & 3;  // in "ap" store which AP we need [of the 4]

            break;

        case 2:  // 4K mapping (1K effective thenks to having 4 AP fields)

            ap = (adr >> 10) & 3;  // in "ap" store which AP we need [of the 4]

        page_size_4k:
            pa = t & 0xFFFFF000UL;
            va = adr & 0xFFFFF000UL;
            sz = 4096;
            break;

        case 3:  // 1K mapping

            if (coarse) {
                if (mmu->xscale) {
                    pxa_tex_page = true;
                    ap = 0;
                    goto page_size_4k;
                } else {
                    fprintf(stderr, "invalid coarse page table entry\r\n");
                    if (fsrP) *fsrP = 7;
                }
            }

            pa = t & 0xFFFFFC00UL;
            va = adr & 0xFFFFFC00UL;
            ap = (t >> 4) & 3;  // in "ap" store the actual AP [and skip quarter-page resolution
                                // later using the goto]
            sz = 1024;
            goto translated;
    }

    // handle 4 AP sections

    i = (t >> 4) & 0xFF;
    if (pxa_tex_page ||
        ((i & 0x0F) == (i >> 4) &&
         (i & 0x03) == ((i >> 2) & 0x03)))  // if all domains are the same, add the whole thing
        ap = (t >> 4) & 3;

    else {  // take the quarter that is the one we need

        sz /= 4;
        pa += ((uint32_t)ap) * sz;
        va += ((uint32_t)ap) * sz;
        ap = (t >> (4 + 2 * ap)) & 3;
    }

translated:
    if (sz > 1024) {
        for (uint32_t offset = 0; offset < sz; offset += 4096) {
            struct TlbEntry *tlbEntry = getTlbEntry(mmu, va + offset);

            tlbEntry->ap = ap;
            tlbEntry->c = c;
            tlbEntry->domain = dom;
            tlbEntry->section = section;
            tlbEntry->pa = pa + offset;
            tlbEntry->revision = mmu->revision;
            tlbEntry->region = memRegionFind(mmu->mem, tlbEntry->pa, 4096);

            if (adr >= tlbEntry->pa && adr - tlbEntry->pa < 4096) *region = tlbEntry->region;
        }
    }

    if (!checkPermissions(mmu, ap, dom, section, priviledged, write, fsrP)) return false;

    *paP = (adr - va) + pa;
    if (mappingInfoP) *mappingInfoP = c ? MMU_MAPPING_CACHEABLE : 0;

    return true;
}

bool mmuTranslate(struct ArmMmu *mmu, uint32_t adr, bool priviledged, bool write, uint32_t *paP,
                  uint_fast8_t *fsrP, uint8_t *mappingInfoP, struct ArmMemRegion **region) {
    *region = NULL;

    if (mmu->transTablPA == MMU_DISABLED_TTP) {
        *paP = adr;
        if (mappingInfoP) *mappingInfoP = 0;

        return true;
    }

    struct TlbEntry *tlbEntry = getTlbEntry(mmu, adr);

    if (tlbEntry->revision != mmu->revision)
        return translateAndCache(mmu, adr, priviledged, write, paP, fsrP, mappingInfoP, region);

    if (!checkPermissions(mmu, tlbEntry->ap, tlbEntry->domain, tlbEntry->section, priviledged,
                          write, fsrP))
        return false;

    *region = tlbEntry->region;

    *paP = (adr & 0xfff) + tlbEntry->pa;
    if (mappingInfoP) *mappingInfoP = tlbEntry->c ? MMU_MAPPING_CACHEABLE : 0;

    return true;
}

uint32_t mmuGetTTP(struct ArmMmu *mmu) { return mmu->transTablPA; }

void mmuSetTTP(struct ArmMmu *mmu, uint32_t ttp) {
    mmuTlbFlush(mmu);
    mmu->transTablPA = ttp;
}

void mmuSetS(struct ArmMmu *mmu, bool on) { mmu->S = on; }

void mmuSetR(struct ArmMmu *mmu, bool on) { mmu->R = on; }

bool mmuGetS(struct ArmMmu *mmu) { return mmu->S; }

bool mmuGetR(struct ArmMmu *mmu) { return mmu->R; }

uint32_t mmuGetDomainCfg(struct ArmMmu *mmu) { return mmu->domainCfg; }

void mmuSetDomainCfg(struct ArmMmu *mmu, uint32_t val) { mmu->domainCfg = val; }

///////////////////////////  debugging helpers  ///////////////////////////

static uint32_t mmuPrvDebugRead(struct ArmMmu *mmu, uint32_t addr) {
    uint32_t t;

    if (!memAccess(mmu->mem, addr, 4, MEM_ACCESS_TYPE_READ, &t)) t = 0xFFFFFFF0UL;

    return t;
}

static void mmuPrvDumpUpdate(uint32_t va, uint32_t pa, uint32_t len, uint8_t dom, uint8_t ap,
                             bool c, bool b, bool valid) {
    static bool wasValid = false;
    static uint32_t expectPa = 0;
    static uint32_t startVa = 0;
    static uint32_t startPa = 0;
    static uint8_t wasDom = 0;
    static uint8_t wasAp = 0;
    static bool wasB = 0;
    static bool wasC = 0;
    uint32_t va_end;

    va_end = (va || len) ? va - 1 : 0xFFFFFFFFUL;

    if (!wasValid && !valid) return;  // no need to bother...

    if (valid != wasValid || dom != wasDom || ap != wasAp || c != wasC || b != wasB ||
        expectPa != pa) {  // not a continuation of what we've been at...

        if (wasValid)
            fprintf(stderr, "0x%08lx - 0x%08lx -> 0x%08lx - 0x%08lx don%u ap%u %c %c\n",
                    (unsigned long)startVa, (unsigned long)va_end, (unsigned long)startPa,
                    (unsigned long)(startPa + (va_end - startVa)), wasDom, wasAp,
                    (char)(wasC ? 'c' : ' '), (char)(wasB ? 'b' : ' '));

        wasValid = valid;
        if (valid) {  // start of a new range

            wasDom = dom;
            wasAp = ap;
            wasC = c;
            wasB = b;
            startVa = va;
            startPa = pa;
            expectPa = pa + len;
        }
    } else  // continuation of what we've been up to...
        expectPa += len;
}

void __attribute__((used)) mmuDump(struct ArmMmu *mmu) {
    uint32_t i, j, t, sla, va, psz;
    bool coarse = false;
    uint_fast8_t dom;

    for (i = 0; i < 0x1000; i++) {
        t = mmuPrvDebugRead(mmu, mmu->transTablPA + (i << 2));
        va = i << 20;
        dom = (t >> 5) & 0x0F;
        switch (t & 3) {
            case 0:  // done
                mmuPrvDumpUpdate(va, 0, 1UL << 20, 0, 0, false, false, false);
                continue;

            case 1:  // coarse page table
                coarse = true;
                t &= 0xFFFFFC00UL;
                break;

            case 2:  // section
                mmuPrvDumpUpdate(va, t & 0xFFF00000UL, 1UL << 20, dom, (t >> 10) & 3, !!(t & 8),
                                 !!(t & 4), true);
                continue;

            case 3:  // fine page table
                t &= 0xFFFFF000UL;
                break;
        }

        sla = t;
        psz = coarse ? 4096 : 1024;
        for (j = 0; j < ((1UL << 20) / psz); j++) {
            t = mmuPrvDebugRead(mmu, sla + (j << 2));
            va = (i << 20) + (j * psz);
            switch (t & 3) {
                case 0:  // invalid
                    mmuPrvDumpUpdate(va, 0, psz, 0, 0, false, false, false);
                    break;

                case 1:  // large 64k page
                    mmuPrvDumpUpdate(va + 0 * 16384UL, (t & 0xFFFF0000UL) + 0 * 16384UL, 16384, dom,
                                     (t >> 4) & 3, !!(t & 8), !!(t & 4), true);
                    mmuPrvDumpUpdate(va + 1 * 16384UL, (t & 0xFFFF0000UL) + 1 * 16384UL, 16384, dom,
                                     (t >> 6) & 3, !!(t & 8), !!(t & 4), true);
                    mmuPrvDumpUpdate(va + 2 * 16384UL, (t & 0xFFFF0000UL) + 2 * 16384UL, 16384, dom,
                                     (t >> 8) & 3, !!(t & 8), !!(t & 4), true);
                    mmuPrvDumpUpdate(va + 3 * 16384UL, (t & 0xFFFF0000UL) + 3 * 16384UL, 16384, dom,
                                     (t >> 10) & 3, !!(t & 8), !!(t & 4), true);
                    j += coarse ? 15 : 63;
                    break;

                case 2:  // small 4k page
                    mmuPrvDumpUpdate(va + 0 * 1024, (t & 0xFFFFF000UL) + 0 * 1024, 1024, dom,
                                     (t >> 4) & 3, !!(t & 8), !!(t & 4), true);
                    mmuPrvDumpUpdate(va + 1 * 1024, (t & 0xFFFFF000UL) + 1 * 1024, 1024, dom,
                                     (t >> 6) & 3, !!(t & 8), !!(t & 4), true);
                    mmuPrvDumpUpdate(va + 2 * 1024, (t & 0xFFFFF000UL) + 2 * 1024, 1024, dom,
                                     (t >> 8) & 3, !!(t & 8), !!(t & 4), true);
                    mmuPrvDumpUpdate(va + 3 * 1024, (t & 0xFFFFF000UL) + 3 * 1024, 1024, dom,
                                     (t >> 10) & 3, !!(t & 8), !!(t & 4), true);
                    if (!coarse) j += 3;
                    break;

                case 3:  // tiny 1k page or TEX page on pxa
                    if (coarse)
                        mmuPrvDumpUpdate(va, t & 0xFFFFF000UL, 4096, dom, (t >> 4) & 3, !!(t & 8),
                                         !!(t & 4), true);
                    else
                        mmuPrvDumpUpdate(va, t & 0xFFFFFC00UL, 1024, dom, (t >> 4) & 3, !!(t & 8),
                                         !!(t & 4), true);
                    break;
            }
        }
    }
    mmuPrvDumpUpdate(0, 0, 0, 0, 0, false, false, false);  // finish things off
}
