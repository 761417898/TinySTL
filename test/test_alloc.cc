#include <gtest/gtest.h>
#include "../src/alloc.h"

#include <stdlib.h>
#include <stdio.h>

namespace tinystl {
    namespace test_alloc {
        TEST(allocate, allocate_0_bytes) {
            EXPECT_EQ(nullptr, alloc::allocate(size_t(0)));
        }

        TEST(allocate, allocate_more_than_128_bytes) {
            int *p = (int*)alloc::allocate(sizeof(int) * 30);
            for (int i = 0; i < 30; ++i) {
                p[i] = i;
            }
            for (int i = 0; i < 30; ++i) {
                EXPECT_EQ(i, p[i]);
            }
        }

        TEST(allocate, allocate_less_than_128_bytes) {
            long long *p = (long long*)alloc::allocate(sizeof(long long));
            *p = 761417898;
            EXPECT_EQ(761417898, *p);
        }


    }
}


int main(int argc, char **argv)
{


    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
