#include "GzipContext.h"

#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "GunzipContext.h"

using namespace std;

class GzipContextTest : public ::testing::Test {
   protected:
    void TearDown() override {
        if (gunzipContext) delete gunzipContext;
    }

    void RunToCompletion(GzipContext& context) {
        context.Continue();
        while (context.Continue() == static_cast<int>(GzipContext::State::more)) {
        }
    }

    void Unzip(GzipContext& context) {
        gunzipContext = new GunzipContext(context.GetGzipData(), context.GetGzipSize());

        while (gunzipContext->Continue() == static_cast<int>(GunzipContext::State::more)) {
        }

        ASSERT_EQ(gunzipContext->GetState(), static_cast<int>(GunzipContext::State::done));
    }

    uint32_t Random() {
        // LCG, glibc style
        rng = (uint64_t)1103515245 * (uint64_t)rng + (uint64_t)12345;
        return rng;
    }

   protected:
    GunzipContext* gunzipContext{nullptr};

    uint32_t rng = 0x98834124;
};

TEST_F(GzipContextTest, itProperlyCompressesAShortString) {
    const string fixture("Hello world");

    GzipContext context(fixture.length() + 1, fixture.c_str());

    RunToCompletion(context);
    ASSERT_EQ(context.GetState(), static_cast<int>(GzipContext::State::done));

    Unzip(context);

    ASSERT_TRUE(fixture == string(static_cast<const char*>(gunzipContext->GetUncompressedData())));
}

TEST_F(GzipContextTest, itProperlyStoresFilename) {
    const string fixture("Hello world");
    const char* filenameFixture = "funky_file.txt";

    GzipContext context(fixture.length() + 1, fixture.c_str());
    context.SetFilename(filenameFixture);

    RunToCompletion(context);
    ASSERT_EQ(context.GetState(), static_cast<int>(GzipContext::State::done));

    ASSERT_EQ(strcmp(filenameFixture, reinterpret_cast<const char*>(context.GetGzipData() + 10)),
              0);

    Unzip(context);

    ASSERT_TRUE(fixture == string(static_cast<const char*>(gunzipContext->GetUncompressedData())));
}

TEST_F(GzipContextTest, itProperlyStoresMtime) {
    const string fixture("Hello world");

    GzipContext context(fixture.length() + 1, fixture.c_str());
    context.SetMtime(0x12345678);

    RunToCompletion(context);
    ASSERT_EQ(context.GetState(), static_cast<int>(GzipContext::State::done));

    size_t i = 4;
    uint32_t mtime = 0;
    mtime |= context.GetGzipData()[i++];
    mtime |= context.GetGzipData()[i++] << 8;
    mtime |= context.GetGzipData()[i++] << 16;
    mtime |= context.GetGzipData()[i++] << 24;

    ASSERT_EQ(mtime, 0x12345678u);
}

TEST_F(GzipContextTest, itGrowsTheOutputBuffer) {
    constexpr size_t fixtureSize = 1 * 1024 * 1024;

    auto fixture = make_unique<uint8_t[]>(fixtureSize);
    memset(fixture.get(), 0, fixtureSize);
    for (size_t i = 0; i < fixtureSize >> 2; i++)
        reinterpret_cast<uint32_t*>(fixture.get())[i] = Random();

    GzipContext context(fixtureSize, fixture.get(), 256 * 1024);

    RunToCompletion(context);
    ASSERT_EQ(context.GetState(), static_cast<int>(GzipContext::State::done));

    Unzip(context);

    ASSERT_EQ(memcmp(fixture.get(), gunzipContext->GetUncompressedData(), fixtureSize), 0);
}
