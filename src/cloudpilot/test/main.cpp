#include <gtest/gtest.h>

#include "Logging.h"

int main(int argc, char** argv) {
    class Listener : public testing::EmptyTestEventListener {
       public:
        void OnTestStart(const testing::TestInfo&) override { logging::disable(); }
    };

    testing::InitGoogleTest(&argc, argv);
    testing::UnitTest::GetInstance()->listeners().Append(new Listener());

    return RUN_ALL_TESTS();
}
