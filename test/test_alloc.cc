#include <gtest/gtest.h>
#include "../src/alloc.h"

namespace tinystl {
    namespace test_alloc {
        TEST(allocate, allocate_0_bytes) {
            EXPECT_EQ(nullptr, alloc::allocate(size_t(0)));
        }

    }
}


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
