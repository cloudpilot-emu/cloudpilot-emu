#include "GunzipContext.h"

#include <gtest/gtest.h>

#include <iostream>

#include "GzipFixtures.h"

using namespace std;

class GunzipContextTest : public ::testing::Test {
   protected:
    ~GunzipContextTest() {
        if (context) delete context;
        if (fixtureData) delete fixtureData;
    }

    void SetupFixture(const uint8_t* data, size_t size) {
        fixtureData = new uint8_t[size];
        fixtureSize = size;

        memcpy(fixtureData, data, size);
    }

    void Set8(size_t index, uint8_t value) { fixtureData[index] = value; }

    void Set16(size_t index, uint16_t value) {
        fixtureData[index++] = value;
        fixtureData[index] = value >> 8;
    }

    void Set32(size_t index, uint32_t value) {
        fixtureData[index++] = value;
        fixtureData[index++] = value >> 8;
        fixtureData[index++] = value >> 16;
        fixtureData[index] = value >> 24;
    }

    void CreateContext(size_t sliceSize = GunzipContext::DEFAULT_SLICE_SIZE) {
        context = new GunzipContext(fixtureData, fixtureSize, sliceSize);
    }

    GunzipContext::State RunToCompletion() {
        while (context->Continue() == static_cast<int>(GunzipContext::State::more)) {
        }

        return static_cast<GunzipContext::State>(context->GetState());
    }

   protected:
    uint8_t* fixtureData{nullptr};
    size_t fixtureSize;

    GunzipContext* context{nullptr};
};

namespace {
    TEST_F(GunzipContextTest, itUnzipsASimpleGz) {
        SetupFixture(fixture::hello_world_gz, fixture::hello_world_gz_size);
        CreateContext();

        ASSERT_EQ(RunToCompletion(), GunzipContext::State::done);

        ASSERT_EQ(string(reinterpret_cast<const char*>(context->GetUncompressedData()),
                         context->GetUncompressedSize()),
                  "hello world");
    }

    TEST_F(GunzipContextTest, itErrorsIfUncompressedSizeIsTooSmall) {
        SetupFixture(fixture::hello_world_gz, fixture::hello_world_gz_size);
        Set32(fixtureSize - 4, 3);
        CreateContext();

        ASSERT_EQ(RunToCompletion(), GunzipContext::State::error);
    }

    TEST_F(GunzipContextTest, itErrorsIfUncompressedSizeIsTooLarge) {
        SetupFixture(fixture::hello_world_gz, fixture::hello_world_gz_size);
        Set32(fixtureSize - 4, 64);
        CreateContext();

        ASSERT_EQ(RunToCompletion(), GunzipContext::State::error);
    }

    TEST_F(GunzipContextTest, itErrorsIfCRCDoesNotMatch) {
        SetupFixture(fixture::hello_world_gz, fixture::hello_world_gz_size);
        Set32(fixtureSize - 8, 0xffffffff);
        CreateContext();

        ASSERT_EQ(RunToCompletion(), GunzipContext::State::error);
    }

    TEST_F(GunzipContextTest, itErrorsIfMagicIsWrong) {
        SetupFixture(fixture::hello_world_gz, fixture::hello_world_gz_size);
        Set8(0, 0);
        CreateContext();

        ASSERT_EQ(RunToCompletion(), GunzipContext::State::error);
    }

    TEST_F(GunzipContextTest, itErrorsIfCompressionMethodIsInvalid) {
        SetupFixture(fixture::hello_world_gz, fixture::hello_world_gz_size);
        Set8(2, 0x01);
        CreateContext();

        ASSERT_EQ(RunToCompletion(), GunzipContext::State::error);
    }

    TEST_F(GunzipContextTest, itErrorsIfFHCRCIsSetWithoutCRC16) {
        SetupFixture(fixture::hello_world_gz, fixture::hello_world_gz_size);
        Set8(3, 0x02);
        CreateContext();

        ASSERT_EQ(RunToCompletion(), GunzipContext::State::error);
    }

    TEST_F(GunzipContextTest, itErrorsIfFHEXTRAIsSetWithoutExtraFields) {
        SetupFixture(fixture::hello_world_gz, fixture::hello_world_gz_size);
        Set8(3, 0x04);
        CreateContext();

        ASSERT_EQ(RunToCompletion(), GunzipContext::State::error);
    }

    TEST_F(GunzipContextTest, itErrorsIfFHNAMEIsSetWithoutName) {
        SetupFixture(fixture::hello_world_gz, fixture::hello_world_gz_size);
        Set8(3, 0x08);
        CreateContext();

        ASSERT_EQ(RunToCompletion(), GunzipContext::State::error);
    }

    TEST_F(GunzipContextTest, itErrorsIfFHCOMMENTIsSetWithoutComment) {
        SetupFixture(fixture::hello_world_gz, fixture::hello_world_gz_size);
        Set8(3, 0x10);
        CreateContext();

        ASSERT_EQ(RunToCompletion(), GunzipContext::State::error);
    }

    TEST_F(GunzipContextTest, itCorrectlyUnpacksWithNameSet) {
        SetupFixture(fixture::random_gz, fixture::random_gz_len);
        CreateContext(1024 * 1024);

        ASSERT_EQ(RunToCompletion(), GunzipContext::State::done);
    }

    TEST_F(GunzipContextTest, itCorrectlyUnpacksInMultipleSlices) {
        SetupFixture(fixture::random_gz, fixture::random_gz_len);
        CreateContext(128 * 1024);

        ASSERT_EQ(RunToCompletion(), GunzipContext::State::done);
    }

    TEST_F(GunzipContextTest, itCorrectlyUnpacksWithAFractionalFinalSlice) {
        SetupFixture(fixture::random_gz, fixture::random_gz_len);
        CreateContext(128 * 1024 - 1);

        ASSERT_EQ(RunToCompletion(), GunzipContext::State::done);
    }

    TEST_F(GunzipContextTest, itHandlesExtraFields) {
        SetupFixture(fixture::extra_fields_gz, fixture::extra_fields_gz_len);
        CreateContext();

        ASSERT_EQ(RunToCompletion(), GunzipContext::State::done);
    }
}  // namespace
